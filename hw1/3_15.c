#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define SEQUENCE_LIMIT 1000

int main(int argc, char **argv)
{
    const char *shared_object_name = "3_15";
    int n;
    pid_t pid;

    if (argc < 2)
    {
        printf("Please enter the value of n.\n");
        return 0;
    }
    n = atoi(argv[1]);
    if (n < 0)
    {
        printf("n must be a integer integer greater than or equal to 1.\n");
        return 0;
    }
    // printf("n = %d\n", n);
    pid = fork();
    if (pid < 0)
    {
        fprintf(stderr, "Fork Failed\n");
        return 1;
    }
    else if (pid == 0)
    {
        int segment_fd = shm_open(shared_object_name, O_CREAT | O_RDWR, 0666);
        ftruncate(segment_fd, sizeof(int) * SEQUENCE_LIMIT);
        int *ptr = mmap(0, sizeof(int) * SEQUENCE_LIMIT, PROT_WRITE, MAP_SHARED, segment_fd, 0);
        *ptr = n;
        int prev = *ptr;
        ptr++;
        for (int i = 1; i < SEQUENCE_LIMIT; i++)
        {
            if (prev == 1)
            {
                break;
            }
            if (prev % 2 == 0)
            {
                *ptr = prev / 2;
            }
            else
            {
                *ptr = 3 * prev + 1;
            }
            prev = *ptr;
            ptr++;
        }
    }
    else
    {
        wait(NULL);
        int segment_fd = shm_open(shared_object_name, O_CREAT | O_RDWR, 0666);
        int *ptr = mmap(0, sizeof(int) * SEQUENCE_LIMIT, PROT_READ, MAP_SHARED, segment_fd, 0);
        for (int i = 0; i < SEQUENCE_LIMIT; i++)
        {
            if (*ptr == 1)
            {
                printf("%d\n", *ptr);
                break;
            }
            else
            {
                printf("%d, ", *ptr);
            }
            ptr++;
        }
        shm_unlink(shared_object_name);
    }
    return 0;
}