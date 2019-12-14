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

static void dfs(PID pid);

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

static void kill(PID pid)
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
        kill(board[(row + step->row) / 2][(col + step->col) / 2]);
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
#include <time.h>

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

void print_path(PATH *path)
{
    POS *step;

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

BOOL run(int round, TID tid)
{
    PATH *path;

    printf("==== ROUND %d ==== %d:", round, tid);

    if (0 != find_jumpable(tid)) {
        path = select_jumpable();
        print_path(path);
        jump(path);
    } else if (0 != find_moveable(tid)) {
        path = select_moveable();
        print_path(path);
        move(path);
    } else {
        puts("LOSE!!!");
        return 0;
    }

    print_board();
    return 1;
}

#define MAX_ROUND   60

int main(void)
{
    char round;
    unsigned seed = time(NULL);

    printf("seed = %u\n", seed);

    init_pieces();
    init_board();
    print_board();

    srand(seed);
    for (round = 1; round <= MAX_ROUND; ++round) {
        if (!run(round, eDark)) break;
        if (!run(round, eLight)) break;
    }

    return 0;
}
