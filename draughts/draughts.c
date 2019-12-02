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

static inline void path_del_last(PATH *path)
{
    --path->nstep;
}
