#include <pthread.h>
#include <unistd.h>

extern long long STRENGTH;
extern pthread_mutex_t mutex;
extern double sum;
extern int THREAD_COUNT;
extern double delta;
extern long long taskSize;

void* calPi(void* i)
{
    long long idx = (long long)i;
    long long start = idx * taskSize;
    double mySum = 0;
    double x = start * delta;

    long long j = 0;
    while(j < taskSize)
    {
        double b = 4.0 / (1 + x * x);
        mySum += b * delta;
        x += delta;
        j++;
    }

    pthread_mutex_lock(&mutex);
    sum += mySum;
    pthread_mutex_unlock(&mutex);
    return 0;
}