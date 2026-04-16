#define _POSIX_C_SOURCE 200809L

#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

static long long g_counter = 0;
static pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;

struct thread_args {
	long long iters;
	int use_mutex;
};

static void die_perror(const char *msg) {
	perror(msg);
	exit(1);
}

static void usage(const char *prog) {
	fprintf(stderr,
	        "Uso:\n"
	        "  %s <T1|T2|P1|P2> <N> <TOTAL_INCREMENTS>\n\n"
	        "Experimentos:\n"
	        "  T1: threads sem sincronizacao\n"
	        "  T2: threads com pthread_mutex\n"
	        "  P1: processos (fork) com shmget/shmat, sem sincronizacao\n"
	        "  P2: processos (fork) com shmget/shmat + sem_open (semaforo)\n\n"
	        "Exemplo:\n"
	        "  %s T2 4 10000000\n",
	        prog, prog);
}

static int parse_int(const char *s, int *out) {
	char *end = NULL;
	errno = 0;
	long v = strtol(s, &end, 10);
	if (errno != 0 || end == s || *end != '\0' || v <= 0 || v > 1000000) {
		return 0;
	}
	*out = (int)v;
	return 1;
}

static int parse_ll(const char *s, long long *out) {
	char *end = NULL;
	errno = 0;
	long long v = strtoll(s, &end, 10);
	if (errno != 0 || end == s || *end != '\0' || v <= 0) {
		return 0;
	}
	*out = v;
	return 1;
}

static void *thread_worker(void *arg) {
	struct thread_args *a = (struct thread_args *)arg;
	for (long long i = 0; i < a->iters; i++) {
		if (a->use_mutex) {
			pthread_mutex_lock(&g_mutex);
			g_counter++;
			pthread_mutex_unlock(&g_mutex);
		} else {
			g_counter++;
		}
	}
	return NULL;
}

static void run_threads(int n, long long total_increments, int use_mutex) {
	pthread_t *threads = calloc((size_t)n, sizeof(*threads));
	struct thread_args *args = calloc((size_t)n, sizeof(*args));
	if (!threads || !args) {
		fprintf(stderr, "Memoria insuficiente\n");
		exit(1);
	}

	g_counter = 0;
	long long base = total_increments / n;
	long long rem = total_increments % n;

	for (int i = 0; i < n; i++) {
		args[i].use_mutex = use_mutex;
		args[i].iters = base + (i < rem ? 1 : 0);
		if (pthread_create(&threads[i], NULL, thread_worker, &args[i]) != 0) {
			die_perror("pthread_create");
		}
	}

	for (int i = 0; i < n; i++) {
		if (pthread_join(threads[i], NULL) != 0) {
			die_perror("pthread_join");
		}
	}

	printf("Esperado: %lld\n", total_increments);
	printf("Obtido:   %lld\n", g_counter);

	free(args);
	free(threads);
}

static void run_processes(int n, long long total_increments, int use_semaphore) {
	int shmid = shmget(IPC_PRIVATE, sizeof(long long), IPC_CREAT | 0600);
	if (shmid == -1) {
		die_perror("shmget");
	}

	long long *counter = (long long *)shmat(shmid, NULL, 0);
	if (counter == (void *)-1) {
		die_perror("shmat");
	}
	*counter = 0;

	char sem_name[64] = {0};
	sem_t *sem = NULL;
	if (use_semaphore) {
		snprintf(sem_name, sizeof(sem_name), "/sisop_sem_%ld", (long)getpid());
		sem = sem_open(sem_name, O_CREAT | O_EXCL, 0600, 1);
		if (sem == SEM_FAILED && errno == EEXIST) {
			sem_unlink(sem_name);
			sem = sem_open(sem_name, O_CREAT | O_EXCL, 0600, 1);
		}
		if (sem == SEM_FAILED) {
			die_perror("sem_open");
		}
	}

	pid_t *pids = calloc((size_t)n, sizeof(*pids));
	if (!pids) {
		fprintf(stderr, "Memoria insuficiente\n");
		exit(1);
	}

	long long base = total_increments / n;
	long long rem = total_increments % n;

	for (int i = 0; i < n; i++) {
		pid_t pid = fork();
		if (pid < 0) {
			die_perror("fork");
		}
		if (pid == 0) {
			sem_t *child_sem = NULL;
			if (use_semaphore) {
				child_sem = sem_open(sem_name, 0);
				if (child_sem == SEM_FAILED) {
					perror("sem_open(child)");
					_exit(2);
				}
			}

			long long iters = base + (i < rem ? 1 : 0);
			for (long long j = 0; j < iters; j++) {
				if (use_semaphore) {
					sem_wait(child_sem);
					(*counter)++;
					sem_post(child_sem);
				} else {
					(*counter)++;
				}
			}

			if (use_semaphore) {
				sem_close(child_sem);
			}
			shmdt(counter);
			_exit(0);
		}
		pids[i] = pid;
	}

	for (int i = 0; i < n; i++) {
		int status = 0;
		if (waitpid(pids[i], &status, 0) < 0) {
			die_perror("waitpid");
		}
		if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
			fprintf(stderr, "Processo %ld terminou com erro\n", (long)pids[i]);
		}
	}

	printf("Esperado: %lld\n", total_increments);
	printf("Obtido:   %lld\n", *counter);

	free(pids);
	shmdt(counter);
	shmctl(shmid, IPC_RMID, NULL);

	if (use_semaphore) {
		sem_close(sem);
		sem_unlink(sem_name);
	}
}

int main(int argc, char **argv) {
	if (argc != 4) {
		usage(argv[0]);
		return 2;
	}

	const char *mode = argv[1];
	int n = 0;
	long long total = 0;

	if (!parse_int(argv[2], &n) || !parse_ll(argv[3], &total)) {
		usage(argv[0]);
		return 2;
	}

	if (strcmp(mode, "T1") == 0) {
		run_threads(n, total, 0);
		return 0;
	}
	if (strcmp(mode, "T2") == 0) {
		run_threads(n, total, 1);
		return 0;
	}
	if (strcmp(mode, "P1") == 0) {
		run_processes(n, total, 0);
		return 0;
	}
	if (strcmp(mode, "P2") == 0) {
		run_processes(n, total, 1);
		return 0;
	}

	usage(argv[0]);
	return 2;
}
