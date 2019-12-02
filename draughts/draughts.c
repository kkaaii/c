#include <string.h>

#define NROW    8
#define NCOL    8
#define NTEAM   2
#define NP      12

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

typedef struct step {
    char    row;
    char    col;
} STEP;

typedef struct path {
    char    nstep;
    STEP    steps[1 + NP];
} PATH;
