#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define SEQUENCE_LIMIT 1000

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Please enter the value of n.\n");
        return 0;
    }
    int n;
    n = atoi(argv[1]);
    if (n < 0)
    {
        printf("n must be a integer integer greater than or equal to 1.\n");
        return 0;
    }
    // printf("n = %d\n", n);
    pid_t pid;
    pid = fork();
    if (pid < 0)
    {
        fprintf(stderr, "Fork Failed\n");
        return 1;
    }
    else if (pid == 0)
    {
        int seq[SEQUENCE_LIMIT];
        seq[0] = n;
        for (int i = 1; i < SEQUENCE_LIMIT; i++)
        {
            int prev = seq[i - 1];
            if (prev == 1)
            {
                break;
            }
            if (prev % 2 == 0)
            {
                seq[i] = prev / 2;
            }
            else
            {
                seq[i] = 3 * prev + 1;
            }
        }
        for (int i = 0; i < SEQUENCE_LIMIT; i++)
        {
            if (seq[i] == 1){
                printf("%d\n", seq[i]);
                break;
            }
            else{
                printf("%d, ", seq[i]);
            }
        }
    }
    else
    {
        wait(NULL);
    }
    return 0;
}