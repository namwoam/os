#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SEQUENCE_LIMIT 1000

int sequence[SEQUENCE_LIMIT];
int sequence_length = 0;

struct sort_param
{
    int start_pos;
    int size;
};

int cmp(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

void *sorting_runner(void *param_ptr)
{
    struct sort_param *param = param_ptr;
    qsort(&sequence[param->start_pos], param->size, sizeof(int), cmp);
    pthread_exit(0);
}

void *merge_runner(void *unused)
{
    int tmp[SEQUENCE_LIMIT];
    for (int i = 0; i < sequence_length; i++)
    {
        tmp[i] = sequence[i];
    }
    int front_count = sequence_length / 2, back_count = (sequence_length - sequence_length / 2);
    int front_pos = 0, back_pos = sequence_length / 2;
    for (int i = 0; i < sequence_length; i++)
    {
        if (front_count == 0 || (back_count > 0 && tmp[back_pos] < tmp[front_pos]))
        {
            sequence[i] = tmp[back_pos];
            back_pos += 1;
            back_count -= 1;
        }
        else
        {
            sequence[i] = tmp[front_pos];
            front_pos += 1;
            front_count -= 1;
        }
    }
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
    struct sort_param param_1, param_2;
    param_1.start_pos = 0;
    param_1.size = sequence_length / 2;
    param_2.start_pos = sequence_length / 2;
    param_2.size = sequence_length - sequence_length / 2;

    pthread_t seq1_tid, seq2_tid, merge_tid;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(&seq1_tid, &attr, sorting_runner, &param_1);
    pthread_create(&seq2_tid, &attr, sorting_runner, &param_2);

    pthread_join(seq1_tid, NULL);
    pthread_join(seq2_tid, NULL);
    pthread_create(&merge_tid, &attr, merge_runner, NULL);
    pthread_join(merge_tid, NULL);

    for (int i = 0; i < sequence_length; i++)
    {
        printf("%d", sequence[i]);
        if (i == sequence_length - 1)
        {
            putchar('\n');
        }
        else
        {
            putchar(',');
        }
    }

    return 0;
}
