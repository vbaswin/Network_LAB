#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

typedef struct {
	int a;
	int *b;

} thread_args;

pthread_mutex_t mutex;

void *
func1(void *arg) {
	thread_args *args = (thread_args *)arg;
	for (int i = 0; i < 10; ++i) {
		printf("fun1\n");
		pthread_mutex_lock(&mutex);
		++*(args->b);
		sleep(5);
		pthread_mutex_unlock(&mutex);
	}
}

void *func2(void *arg) {
	thread_args *args = (thread_args *)arg;
	for (int i = 0; i < 10; ++i) {
		pthread_mutex_lock(&mutex);
		printf("func2\targ1: %d\targ2: %d\n", args->a, ++*(args->b));
		pthread_mutex_unlock(&mutex);
		sleep(1);
	}
}

int main() {
	int a = 1, b = 5;
	pthread_t t1, t2;
	thread_args args;
	args.a = a;
	args.b = &b;

	//  you need to initialize the mutex using pthread_mutex_init before
	// using it to protect the critical section
	pthread_mutex_init(&mutex, NULL);

	pthread_create(&t1, NULL, func1, (void *)&args);
	pthread_create(&t2, NULL, func2, (void *)&args);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	printf("b: %d\n", b);

	pthread_mutex_destroy(&mutex);
	return 0;
}
