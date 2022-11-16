#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

extern long long myAtoI(char* a);
extern void *calPi(void *i);

long long STRENGTH;
int THREAD_COUNT;
pthread_mutex_t mutex;
double sum;
double delta;
long long taskSize;

int main(int argc, char *argv[])
{
    char* step = argv[1];
    STRENGTH = myAtoI(step);  // strength means the pressure on the computer in this test
    struct timespec start, end;
    delta = 1.0 / (double)STRENGTH;

    THREAD_COUNT = sysconf(_SC_NPROCESSORS_ONLN);
    taskSize = STRENGTH / THREAD_COUNT;
    pthread_mutex_init(&mutex, NULL);

    pthread_t pids[128];
    clock_gettime(CLOCK_REALTIME, &start);

    for(int i = 0; i < THREAD_COUNT; i++)
    {
        pthread_create(&pids[i], NULL, calPi, (void *)(long)i);
    }

    for (int i = 0; i < THREAD_COUNT; i++)
    {
        pthread_join(pids[i], NULL);
    }
    clock_gettime(CLOCK_REALTIME, &end);
    long long timeSpent = end.tv_nsec - start.tv_nsec + 1000000000 * (end.tv_sec - start.tv_sec);
    double score = (double)STRENGTH / (double)timeSpent * 100;

    printf("pi: %.10lf\n", sum);
    printf("score: %lf\n", score);
    return 0;
}
