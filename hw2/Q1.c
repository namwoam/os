#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SEQUENCE_LIMIT 1000

int sequence[SEQUENCE_LIMIT];
int sequence_length = 0;

int max_value, min_value, average_value;

void *average_runner(void *unused)
{
    int sum = 0;
    for (int i = 0; i < sequence_length; i++)
    {
        sum += sequence[i];
    }
    average_value = sum / sequence_length;
    pthread_exit(0);
}

void *max_runner(void *unused)
{
    int max_idx = 0;
    for (int i = 0; i < sequence_length; i++)
    {
        if (sequence[max_idx] < sequence[i])
        {
            max_idx = i;
        }
    }
    max_value = sequence[max_idx];
    pthread_exit(0);
}

void *min_runner(void *unused)
{
    int min_idx = 0;
    for (int i = 0; i < sequence_length; i++)
    {
        if (sequence[min_idx] > sequence[i])
        {
            min_idx = i;
        }
    }
    min_value = sequence[min_idx];
    pthread_exit(0);
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Please enter a list of numbers.\n");
        return 0;
    }
    sequence_length = 0;
    for (int i = 1; i < argc; i++)
    {
        int n = atoi(argv[i]);
        sequence[sequence_length] = n;
        sequence_length += 1;
        if (sequence_length > SEQUENCE_LIMIT)
        {
            printf("Sequence too big!\n");
            return 0;
        }
    }

    pthread_t average_tid, max_tid, min_tid;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(&average_tid, &attr, average_runner, NULL);
    pthread_create(&max_tid, &attr, max_runner, NULL);
    pthread_create(&min_tid, &attr, min_runner, NULL);

    pthread_join(average_tid, NULL);
    pthread_join(max_tid, NULL);
    pthread_join(min_tid, NULL);

    printf("The average value is %d\n", average_value);
    printf("The minimum value is %d\n", min_value);
    printf("The maximum value is %d\n", max_value);
    return 0;
}
