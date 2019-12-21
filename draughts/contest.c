#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define R   0
#define W   1

#define LINE_LEN    80

#define MAX_ROUND   60

typedef struct {
    pid_t   pid;
    int     dsp[2]; /* down-stream pipe */
    int     usp[2]; /* up-stream pipe */
} Channel;

void contestant(Channel *channel, const char *path)
{
    if (pipe(channel->dsp) < 0) {
        fputs("create pipe error", stderr);
        exit(0);
    }

    if (pipe(channel->usp) < 0) {
        fputs("create pipe error", stderr);
        exit(0);
    }

    channel->pid = fork();
    if (channel->pid < 0) {
        fputs("create process error", stderr);
        exit(0);
    }

    if (channel->pid > 0) {
        close(channel->dsp[R]);
        close(channel->usp[W]);
    } else {
        close(channel->dsp[W]);
        close(channel->usp[R]);

        if (channel->dsp[R] != STDIN_FILENO) {
            dup2(channel->dsp[R], STDIN_FILENO);
            close(channel->dsp[R]);
        }

        if (channel->usp[W] != STDOUT_FILENO) {
            dup2(channel->usp[W], STDOUT_FILENO);
            close(channel->usp[W]);
        }

        execl(path, NULL);
    }
}

int waitfor(int fd, char line[], const char *expected, int size)
{
    FILE *fp = fdopen(fd, "r");

    for (;;) {
        fgets(line, LINE_LEN - 1, fp);
        if (0 == strncmp(line, expected, size))
            return strlen(line);

        if (0 == strncmp(line, "DEBUG", 5)) {
            fprintf(stderr, "%d: %s", getpid(), line);
        }
    }

    return 0;
}

char start1[] = "START 1\n";
char start2[] = "START 2\n";
char turn[] = "TURN\n";
char end1[] = "END\n";

int main(int argc, char *argv[])
{
    Channel channelA, channelB;
    char line[LINE_LEN + 1];
    char round;
    int  linelen;

    if (argc < 3) {
        fprintf(stderr, "Usage: %s {contestant 1} {contestant 2}\n", argv[0]);
        return -1;
    }

    contestant(&channelA, argv[1]);
    if (0 == channelA.pid)
        return 0;

    contestant(&channelB, argv[2]);

    if (0 == channelB.pid)
        return 0;

    write(channelA.dsp[W], start1, sizeof start1 - 1);
    waitfor(channelA.usp[R], line, "OK\n", 3);

    write(channelB.dsp[W], start2, sizeof start2 - 1);
    waitfor(channelB.usp[R], line, "OK\n", 3);

    for (round = 0; round < MAX_ROUND; ++round) {
        write(channelA.dsp[W], turn, sizeof turn - 1);
        linelen = waitfor(channelA.usp[R], line, "PLACE", 5);
        write(channelB.dsp[W], line, linelen);

        write(channelB.dsp[W], turn, sizeof turn - 1);
        linelen = waitfor(channelB.usp[R], line, "PLACE", 5);
        write(channelA.dsp[W], line, linelen);
    }

    write(channelA.dsp[W], end1, sizeof end1 - 1);
    write(channelB.dsp[W], end1, sizeof end1 - 1);

    waitpid(channelA.pid, NULL, 0);
    waitpid(channelB.pid, NULL, 0);
    return 0;
}
