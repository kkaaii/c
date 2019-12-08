#include <string.h>

#define NROW    8
#define NCOL    8
#define NTEAM   2
#define NP      12

#define MAX_NPATH   10

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

TID turn = eDark;

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
