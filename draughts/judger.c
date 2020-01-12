#include <string.h>

#define	JUDGER

#define NROW    8
#define NCOL    8

#define NTEAM   2
#define NP      12

#define NPATH   10
#define MIN_NSTEP   2

#define	FALSE	0
#define	TRUE	(!FALSE)

typedef char	BOOL;

#define PID_NIL 0

typedef char    PID;    /* piece id */
typedef char	TID;	/* team id */
typedef char    DID;    /* direction id */

typedef struct piece {
    TID     tid;
    char    row;
    char    col;
    BOOL    king;
} PIECE;

typedef struct dir {
    char    drow;
    char    dcol;
} DIR;
 
typedef struct team {
    DID     dir_first;
    DID     dir_last;
    char    row_king;
} TEAM;

typedef struct pos {
    char    row;
    char    col;
} POS;

typedef struct path {
    char    nstep;
    POS     steps[1 + NP];
} PATH;

typedef struct node {
    PIECE   pieces[1 + NTEAM * NP];
    PID     board[NROW][NCOL];
    PID     teams[1 + NTEAM];
    char    npath;
    PATH    longest[NPATH];
} NODE;

const DIR dirs[] = {{-1, -1}, {-1, 1}, {1, 1}, {1, -1}};
TEAM teams[1 + NTEAM];
PATH current;

static inline BOOL is_in_board(char row, char col)
{
    return 0 <= row && row < NROW && 0 <= col && col < NCOL;
}

static inline void path_add_step(PATH *path, char row, char col)
{
    POS *step = &path->steps[path->nstep++];

    step->row = row;
    step->col = col;
}

static inline void piece_drop(NODE *node, PID pid, char row, char col)
{
    PIECE *piece = &node->pieces[pid];

    piece->row = row;
    piece->col = col;
    node->board[row][col] = pid;
}

static inline void piece_crown(NODE *node, PID pid)
{
    PIECE *piece = &node->pieces[pid];

    if (piece->row == teams[piece->tid].row_king)
        piece->king = TRUE;
}

static void piece_kill(NODE *node, PID pid)
{
    PIECE *piece = &node->pieces[pid];
    char row = piece->row;
    char col = piece->col;

    node->board[row][col] = PID_NIL;
}

void piece_jump(NODE *node, PATH *path)
{
    POS *step = &path->steps[0];
    char row = step->row;
    char col = step->col;
    PID  pid = node->board[row][col];

    node->board[row][col] = PID_NIL;
    while (++step < &path->steps[path->nstep]) {
        piece_kill(node, node->board[(row + step->row) / 2][(col + step->col) / 2]);
        row = step->row;
        col = step->col;
    }

    piece_drop(node, pid, row, col);
    piece_crown(node, pid);
}

void piece_move(NODE *node, PATH *path)
{
    char row = path->steps[0].row;
    char col = path->steps[0].col;
    PID  pid = node->board[row][col];

    node->board[row][col] = PID_NIL;

    row = path->steps[1].row;
    col = path->steps[1].col;
    piece_drop(node, pid, row, col);
    piece_crown(node, pid);
}

void init_pieces(NODE *node)
{
    PID pid = 1;
    PIECE *piece = &node->pieces[1];

    memset(node->pieces, 0, sizeof node->pieces);
    for (; pid <= NTEAM * NP; ++pid, ++piece) {
        piece->tid  = 1 + (pid > NP);
    }
}

void init_board(NODE *node)
{
    char row;
    char col;
    PID  pid = 1;
    TEAM *team = &teams[1];

    memset(node->board, 0, sizeof node->board);
    for (row = NROW - NP * 2 / NCOL; row < NROW; ++row) {
        for (col = (row & 1) ^ 1; col < NCOL; col += 2) {
            piece_drop(node, pid, row, col);
            piece_drop(node, NP + pid, NROW - 1 - row, NCOL - 1 - col);
            ++pid;
        }
    }

    team->row_king = 0;

    ++team;
    team->row_king = NROW - 1;
}

#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/time.h>

#define	LINE_LEN    80

void place(NODE *node, const char *s)
{
    PATH *path = &current;
    int n;
    int row, col;

    if (sscanf(s, "%d", &n)) {
	path->nstep = n;
	while (' ' == *s) ++s;
	while (' ' != *s) ++s;
    }

    for (n = 0; n < path->nstep; ++n) {
	if (2 == sscanf(s, "%d,%d", &row, &col)) {
            path->steps[n].row = row;
            path->steps[n].col = col;
	    while (' ' == *s) ++s;
	    while (',' != *s) ++s;
	    while (' ' != *s) ++s;
	}
    }

    if (path->nstep > 2 || 0 == ((path->steps[0].row ^ path->steps[1].row) & 1)) {
	piece_jump(node, path);
    } else {
	piece_move(node, path);
    }
}

void print_board(NODE *node)
{
    char row;
    char col;
    PIECE *piece;

    for (row = 0; row < NROW; ++row) {
        for (col = 0; col < NCOL; ++col) {
            if (0 == ((row ^ col) & 1)) {
                putchar(' ');
                continue;
            }

            piece = node->pieces + node->board[row][col];
            switch (piece->tid) {
            case 0:
                putchar('0');
                break;
            case 1:
                putchar(piece->king ? 'K' : 'M');
                break;
            default:
                putchar(piece->king ? 'k' : 'm');
                break;
            }
        }
        putchar('\n');
    }
}

#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define R   0
#define W   1

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

        execl(path, "", NULL);
    }
}

const char START1[] = "START 1\n";
const char START2[] = "START 2\n";
const char OK[] = "OK\n";
const char TURN[] = "TURN\n";
const char END[] = "END\n";
const char PLACE[] = "PLACE";
const char DEBUG[] = "DEBUG";

int waitfor(int fd, char line[], const char *expected, int size)
{
    FILE *fp = fdopen(fd, "r");
    struct timeval tv;
    fd_set rfds;

    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);

    for (;;) {
        tv.tv_sec = 2;
        tv.tv_usec = 0;

        if (0 == select(fd + 1, &rfds, NULL, NULL, &tv))
            return 0;

        fgets(line, LINE_LEN, fp);
        if (0 == strncmp(line, expected, size))
            break;

        if (0 == strncmp(line, DEBUG, sizeof DEBUG - 1))
            printf("%s", line);
    }
    return strlen(line);
}

int main(int argc, char *argv[])
{
    NODE node, *root = &node;
    Channel channelA, channelB;
    char line[LINE_LEN + 1];
    char round;
    int  linelen;

    if (argc < 3) {
        fprintf(stderr, "Usage: %s {player 1} {player 2}\n", argv[0]);
        return -1;
    }

    contestant(&channelA, argv[1]);
    if (0 == channelA.pid)
        return 0;

    contestant(&channelB, argv[2]);
    if (0 == channelB.pid)
        return 0;

    write(channelA.dsp[W], START1, sizeof START1 - 1);
    waitfor(channelA.usp[R], line, OK, sizeof OK - 1);

    write(channelB.dsp[W], START2, sizeof START2 - 1);
    waitfor(channelB.usp[R], line, OK, sizeof OK - 1);

    init_pieces(root);
    init_board(root);
    print_board(root);

    for (round = 1; round <= MAX_ROUND; ++round) {
        write(channelA.dsp[W], TURN, sizeof TURN - 1);
        linelen = waitfor(channelA.usp[R], line, PLACE, sizeof PLACE - 1);
	if (0 == linelen) break;
        write(channelB.dsp[W], line, linelen);

        printf("\nPLAYER 1 - ROUND %d: %s", round, line);
        place(root, &line[sizeof PLACE - 1]);
        print_board(root);

        write(channelB.dsp[W], TURN, sizeof TURN - 1);
        linelen = waitfor(channelB.usp[R], line, PLACE, sizeof PLACE - 1);
	if (0 == linelen) break;
        write(channelA.dsp[W], line, linelen);

        printf("\nPLAYER 2 - ROUND %d: %s", round, line);
        place(root, &line[sizeof PLACE - 1]);
        print_board(root);
    }

    write(channelA.dsp[W], END, sizeof END - 1);
    write(channelB.dsp[W], END, sizeof END - 1);

    waitpid(channelA.pid, NULL, 0);
    waitpid(channelB.pid, NULL, 0);
    return 0;
}
