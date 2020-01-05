#include <string.h>

#define NROW    8
#define NCOL    8
#define NTEAM   2
#define NP      12

#define MAX_NPATH   10
#define MIN_NSTEP   2

#define PID_NIL 0

typedef char    PID;    /* piece identifier */
typedef char    DID;    /* direction identifier */

typedef enum {
    FALSE = 0,
    TRUE = !FALSE
} BOOL;

typedef enum {
    eDark = 1,
    eLight = 2
} TID;  /* team identifier */

struct piece {
    PID     prev;
    PID     next;
    TID     tid;
    char    row;
    char    col;
    BOOL    king;
};

typedef struct dir {
    char    drow;
    char    dcol;
} DIR;
 
typedef struct team {
    PID     head;
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

const DIR dirs[] = {{-1, -1}, {-1, 1}, {1, 1}, {1, -1}};

PID board[NROW][NCOL];
struct piece pieces[1 + NTEAM * NP];
TEAM teams[1 + NTEAM];

char npath = 0;
PATH longest[MAX_NPATH];
PATH current;

static inline BOOL is_in_board(char row, char col)
{
    return 0 <= row && row < NROW && 0 <= col && col < NCOL;
}

static inline PID get_first(TID tid)
{
    return teams[tid].head;
}

static inline PID get_next(PID pid)
{
    return pieces[pid].next;
}

static inline void path_add_step(PATH *path, char row, char col)
{
    POS *step = &path->steps[path->nstep++];

    step->row = row;
    step->col = col;
}

static inline void path_add_move(char row_orig, char col_orig, char row_new, char col_new)
{
    PATH *path = &longest[npath++];

    path->steps[0].row = row_orig;
    path->steps[0].col = col_orig;

    path->steps[1].row = row_new;
    path->steps[1].col = col_new;

    path->nstep = 2;
}

static inline void path_del_last(PATH *path)
{
    --path->nstep;
}

void update_longest_path(void)
{
    if (current.nstep < longest[0].nstep)
        return;

    if (current.nstep > longest[0].nstep)
        npath = 0;

    if (npath < MAX_NPATH)
        memcpy(&longest[npath++], &current, sizeof (PATH));
}

static inline void drop(PID pid, char row, char col)
{
    struct piece *piece = &pieces[pid];

    piece->row = row;
    piece->col = col;
    board[row][col] = pid;
}

static inline void crown(PID pid)
{
    struct piece *piece = &pieces[pid];

    if (piece->row == teams[piece->tid].row_king)
        piece->king = TRUE;
}

static void killp(PID pid)
{
    struct piece *piece = &pieces[pid];
    PID prev = piece->prev;
    PID next = piece->next;
    char row = piece->row;
    char col = piece->col;

    board[row][col] = PID_NIL;

    if (PID_NIL != prev)
        pieces[prev].next = next;
    else
        teams[piece->tid].head = next;

    if (PID_NIL != next)
        pieces[next].prev = prev;
}

void jump(PATH *path)
{
    char row = path->steps[0].row;
    char col = path->steps[0].col;
    PID  pid = board[row][col];
    POS *step;

    board[row][col] = PID_NIL;
    for (step = &path->steps[1]; step < &path->steps[path->nstep]; ++step) {
        killp(board[(row + step->row) / 2][(col + step->col) / 2]);
        row = step->row;
        col = step->col;
    }

    drop(pid, row, col);
    crown(pid);
}

void move(PATH *path)
{
    char row = path->steps[0].row;
    char col = path->steps[0].col;
    PID  pid = board[row][col];

    board[row][col] = PID_NIL;

    row = path->steps[1].row;
    col = path->steps[1].col;
    drop(pid, row, col);
    crown(pid);
}

#if defined(PLAYER)
int find_moveable(TID tid)
{
    PID pid;
    char row_from, row_to;
    char col_from, col_to;
    char i, imax;

    npath = 0;

    for (pid = get_first(tid); PID_NIL != pid; pid = get_next(pid)) {
        struct piece *piece = &pieces[pid];
        if (piece->king) {
            i = 0;
            imax = 3;
        } else {
            i = teams[piece->tid].dir_first;
            imax = teams[piece->tid].dir_last;
        }

        row_from = piece->row;
        col_from = piece->col;
        for (; i <= imax; ++i) {
            row_to = row_from + dirs[i].drow;
            col_to = col_from + dirs[i].dcol;

            if (is_in_board(row_to, col_to) && PID_NIL == board[row_to][col_to])
                path_add_move(row_from, col_from, row_to, col_to);
        }
    }

    return npath;
}

static void dfs(PID pid);

int find_jumpable(TID tid)
{
    PID pid;

    npath = 0;
    longest[0].nstep = MIN_NSTEP;

    for (pid = get_first(tid); PID_NIL != pid; pid = get_next(pid)) {
        current.nstep = 0;
        dfs(pid);
    }

    return npath;
}

static void dfs(PID pid)
{
    char row0 = pieces[pid].row, row1, row2;
    char col0 = pieces[pid].col, col1, col2;
    PID  m;
    char sum = 0;
    const DIR *dir;

    path_add_step(&current, row0, col0);
    for (dir = &dirs[0]; dir < &dirs[4]; ++dir) {
        row1 = row0 + dir->drow;
        col1 = col0 + dir->dcol;

        row2 = row1 + dir->drow;
        col2 = col1 + dir->dcol;

        if (!is_in_board(row2, col2))
            continue;

        m = board[row1][col1];
        if (PID_NIL == m || PID_NIL != board[row2][col2] || pieces[m].tid == pieces[pid].tid)
            continue;

        ++sum;

        board[row0][col0] = PID_NIL;
        board[row1][col1] = PID_NIL;
        drop(pid, row2, col2);

        dfs(pid);

        drop(pid, row0, col0);
        board[row1][col1] = m;
        board[row2][col2] = PID_NIL;
    }

    if (0 == sum) {
        update_longest_path();
        current.nstep = 0;
    }
}
#endif

void init_pieces(void)
{
    PID pid;
    TID tid = eDark;
    struct piece *piece;

    memset(pieces, 0, sizeof pieces);
    for (pid = 1, piece = &pieces[1]; pid <= NTEAM * NP; ++pid, ++piece) {
        piece->prev = pid - 1;
        piece->next = pid + 1;
        piece->tid  = eDark + (pid > NP);
    }

    pieces[NP].next = PID_NIL;
    teams[tid++].head = 1;

    pieces[NP +  1].prev = PID_NIL;
    pieces[NP + NP].next = PID_NIL;
    teams[tid].head = NP + 1;
}

void init_board(void)
{
    char row;
    char col;
    PID  pid = 1;
    TEAM *team = &teams[eDark];

    for (row = NROW - NP * 2 / NCOL; row < NROW; ++row) {
        for (col = (row & 1) ^ 1; col < NCOL; col += 2) {
            drop(pid, row, col);
            drop(NP + pid, NROW - 1 - row, NCOL - 1 - col);
            ++pid;
        }
    }

    team->dir_first = 0;
    team->dir_last = 1;
    team->row_king = 0;

    ++team;
    team->dir_first = 2;
    team->dir_last = 3;
    team->row_king = NROW - 1;
}

#include <stdio.h>
#include <stdlib.h>

#define	LINE_LEN    80

void place(const char *s)
{
    PATH *path = &current;
    int i;
    int row, col;

    if (sscanf(s, "%d", &i)) {
	path->nstep = i;
	while (' ' == *s) ++s;
	while (' ' != *s) ++s;
    }

    for (i = 0; i < path->nstep; ++i) {
	if (2 == sscanf(s, "%d,%d", &row, &col)) {
            path->steps[i].row = row;
            path->steps[i].col = col;
	    while (' ' == *s) ++s;
	    while (',' == *s) ++s;
	    while (' ' != *s) ++s;
	}
    }

    if (path->nstep > 2 || 0 == ((path->steps[0].row ^ path->steps[1].row) & 1)) {
	jump(path);
    } else {
	move(path);
    }
}

#if defined(JUDGER)
void print_board(void)
{
    char row;
    char col;
    struct piece *piece;

    for (row = 0; row < NROW; ++row) {
        for (col = 0; col < NCOL; ++col) {
            if (0 == ((row ^ col) & 1)) {
                putchar(' ');
                continue;
            }

            piece = pieces + board[row][col];
            switch (piece->tid) {
            case eDark:
                putchar(piece->king ? 'K' : 'M');
                break;
            case eLight:
                putchar(piece->king ? 'k' : 'm');
                break;
            default:
                putchar('0');
                break;
            }
        }
        putchar('\n');
    }
}
#endif

#if defined(PLAYER)
void print_path(PATH *path)
{
    POS *step;

    printf("PLACE %d", path->nstep);
    for (step = &path->steps[0]; step < &path->steps[path->nstep]; ++step)
        printf(" %d,%d", step->row, step->col);
    putchar('\n');
}

PATH *select_jumpable(void)
{
    /* TODO */
    return &longest[rand() % npath];
}

PATH *select_moveable(void)
{
    /* TODO */
    return &longest[rand() % npath];
}

BOOL run(TID tid)
{
    PATH *path;

    if (0 != find_jumpable(tid)) {
        path = select_jumpable();
        print_path(path);
        jump(path);
    } else if (0 != find_moveable(tid)) {
        path = select_moveable();
        print_path(path);
        move(path);
    } else {
        return 0;
    }

    return 1;
}

int main(void)
{
    char line[LINE_LEN + 1];
    int  tid;

    init_pieces();
    init_board();

    setlinebuf(stdin);
    setlinebuf(stdout);

    for (;;) {
	if (NULL == fgets(line, sizeof line - 1, stdin)) {
	    continue;
	}

	if (0 == strncmp(line, "END", 3)) {
	    break;
    	}

        if (0 == strncmp(line, "TURN", 4)) {
	    run(tid);
        } else if (0 == strncmp(line, "PLACE", 5)) {
            place(&line[5]);
	} else if (0 == strncmp(line, "START", 5)) {
	    sscanf(&line[5], "%d", &tid);
            puts("OK");
        } else {
            printf("DEBUG: Unknown command \"%s\"\n", line);
        }	    
    }

    return 0;
}
#endif

#if defined(JUDGER)

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

        execl(path, "");
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

    init_pieces();
    init_board();
    print_board();

    for (round = 0; round < MAX_ROUND; ++round) {
        write(channelA.dsp[W], turn, sizeof turn - 1);
        linelen = waitfor(channelA.usp[R], line, "PLACE", 5);
        write(channelB.dsp[W], line, linelen);

        printf("\nPLAYER 1 - ROUND %d: %s", round, line);
        place(&line[5]);
        print_board();

        write(channelB.dsp[W], turn, sizeof turn - 1);
        linelen = waitfor(channelB.usp[R], line, "PLACE", 5);
        write(channelA.dsp[W], line, linelen);

        printf("\nPLAYER 2 - ROUND %d: %s", round, line);
        place(&line[5]);
        print_board();
    }

    write(channelA.dsp[W], end1, sizeof end1 - 1);
    write(channelB.dsp[W], end1, sizeof end1 - 1);

    waitpid(channelA.pid, NULL, 0);
    waitpid(channelB.pid, NULL, 0);
    return 0;
}
#endif
