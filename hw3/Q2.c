#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <math.h>

#define POINT_COUNT 1000
#define THREAD_COUNT 5

pthread_mutex_t mutex;

int cnt = 0;

void *thread_run(void *arg)
{
    for (int i = 0; i < POINT_COUNT; i++)
    {
        double x, y, dx, dy;
        x = (double)rand() / RAND_MAX;
        y = (double)rand() / RAND_MAX;
        dx = x - 0.5;
        dy = y - 0.5;
        if (sqrt(dx * dx + dy * dy) <= 0.5)
        {
            pthread_mutex_lock(&mutex);
            cnt += 1;
            pthread_mutex_unlock(&mutex);
        }
    }

    pthread_exit(0);
}

int main()
{
    cnt = 0;
    srand(time(NULL));
    pthread_mutex_init(&mutex, 0);
    pthread_t threads[THREAD_COUNT];
    for (int i = 0; i < THREAD_COUNT; i++)
    {
        pthread_create(&threads[i], NULL, thread_run, 0);
    }
    for (int i = 0; i < THREAD_COUNT; i++)
    {
        pthread_join(threads[i], NULL);
    }
    pthread_mutex_destroy(&mutex);
    double p = (double)cnt / (THREAD_COUNT * POINT_COUNT);
    double pi = p * 4;
    printf("pi= %f\n", pi);
    return 0;
}