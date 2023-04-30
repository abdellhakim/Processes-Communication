#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define FIFO_NAME "myfifo"

void *write_thread(void *arg){
    int fd = open(FIFO_NAME, O_WRONLY);
    char buf[1024];

    while (1) {
        /* Read from stdin */
        fgets(buf, sizeof(buf), stdin);

        /* Write to the pipe */
        write(fd, buf, strlen(buf) + 1);

        /* Check for the "quit" command */
        if (strncmp(buf, "quit", 4) == 0) {
            break;
        }
    }

    /* Clean up and exit */
    close(fd);
    pthread_exit(NULL);
}

void *read_thread(void *arg){
    int fd = open(FIFO_NAME, O_RDONLY);
    char buf[1024];

    while (1) {
        /* Read from the pipe */
        read(fd, buf, sizeof(buf));

        /* Print the message */
        printf("Received message: %s", buf);

        /* Check for the "quit" command */
        if (strncmp(buf, "quit", 4) == 0) {
            break;
        }
    }

    /* Clean up and exit */
    close(fd);
    unlink(FIFO_NAME);
    pthread_exit(NULL);
}

int main()
{
    /* Create the named pipe */
    mkfifo(FIFO_NAME, 0666);

    /* Start the read and write threads */
    pthread_t id1, id2;
    pthread_create(&id1, NULL, read_thread, NULL);
    pthread_create(&id2, NULL, write_thread, NULL);

    /* Wait for threads to finish */
    pthread_join(id1, NULL);
    pthread_join(id2, NULL);

    return 0;
}
