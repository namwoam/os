#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define BUFFER_SIZE 256
#define READ_END 0
#define WRITE_END 1

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Please provide path of input and destination\n");
        return 0;
    }

    char *input_file_path = argv[1];
    char *destination_file_path = argv[2];
    char buffer[BUFFER_SIZE];
    int fd[2];
    FILE *destination_stream, *input_stream;
    int bytes_read, bytes_written;

    pid_t pid;

    if (pipe(fd) == -1)
    {
        fprintf(stderr, "Failed to create pipe");
        return 1;
    }

    destination_stream = fopen(destination_file_path, "w");
    if (destination_stream == NULL)
    {
        fprintf(stderr, "Failed to open destination file\n");
        return 1;
    }
    input_stream = fopen(input_file_path, "r");
    if (input_stream == NULL)
    {
        fprintf(stderr, "Failed to open input file\n");
        return 1;
    }

    pid = fork();
    if (pid < 0)
    {
        fprintf(stderr, "Fork Failed\n");
        return 1;
    }
    if (pid == 0)
    {
        close(fd[WRITE_END]);
        while ((bytes_read = read(fd[READ_END], buffer, BUFFER_SIZE)) > 0)
        {
            bytes_written = fwrite(buffer, sizeof(char), bytes_read, destination_stream);
            if (bytes_written != bytes_read)
            {
                fprintf(stderr, "Write to destination error %d %d\n", bytes_written, bytes_read);
                return 1;
            }
        }
        close(fd[READ_END]);
        fclose(input_stream);
    }
    else
    {
        close(fd[READ_END]);
        while ((bytes_read = fread(buffer, sizeof(char), BUFFER_SIZE, input_stream)) > 0)
        {
            bytes_written = write(fd[WRITE_END], buffer, bytes_read);
            if (bytes_written != bytes_read)
            {
                fprintf(stderr, "Read from input error\n");
                return 1;
            }
        }
        close(fd[WRITE_END]);
        fclose(input_stream);
        wait(NULL);
    }
    return 0;
}