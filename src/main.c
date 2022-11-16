#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

extern long long myAtoI(char *a);
extern void *calPi(void *i);

long long STRENGTH = 1000000;
int THREAD_COUNT;
pthread_mutex_t mutex;
double sum;
double delta;
long long taskSize;

void init();
void runTest();
void beforeTestInit();
pthread_t pids[128];
int firstTest = 1;
int heavyLoadCounter;
double scoreSum;
double avg;
struct timespec start, end;
double score;
double prevScore;
double gap;
long approxTime;

int main()
{
    THREAD_COUNT = sysconf(_SC_NPROCESSORS_ONLN);
    pthread_mutex_init(&mutex, NULL);

    // approxTime is the approximate time of a test precedure
    // gap is current test score - previous test score
    while (firstTest || gap > 1 || gap < -1 || score < 1 || prevScore < 1 || approxTime < 60)
    {
        beforeTestInit();
        runTest();
        approxTime = end.tv_sec - start.tv_sec;
        

        if (approxTime < 60)
            STRENGTH *= 2;
        else
        {
            heavyLoadCounter++;
            scoreSum += score;
            avg = scoreSum / (double)heavyLoadCounter;
            if (score - avg < 1 && avg - score < 1 && heavyLoadCounter > 5)
                break;
        }
        printf("pi: %.10lf\n", sum);
        printf("score: %lf\n", score);
    }
    printf("---------------------------\n");
    if (end.tv_sec - start.tv_sec < 120)
        printf("final score: %lf\n", score);
    else
        printf("final score: %lf\n", avg);
    return 0;
}

void runTest()
{
    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < THREAD_COUNT; i++)
    {
        pthread_create(&pids[i], NULL, calPi, (void *)(long)i);
    }
    for (int i = 0; i < THREAD_COUNT; i++)
    {
        pthread_join(pids[i], NULL);
    }
    clock_gettime(CLOCK_REALTIME, &end);

    long long timeSpent = end.tv_nsec - start.tv_nsec + 1000000000 * (end.tv_sec - start.tv_sec);
    score = (double)STRENGTH / (double)timeSpent * 100;
    gap = score - prevScore;
}

void beforeTestInit()
{
    if (firstTest)
        firstTest = 0;

    prevScore = score;

    sum = 0;
    delta = 1.0 / (double)STRENGTH;
    taskSize = STRENGTH / THREAD_COUNT;
}