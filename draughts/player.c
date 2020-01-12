#include <string.h>

#define	PLAYER

#define NROW    8
#define NCOL    8

#define NTEAM   2
#define NP      12

#define NPATH   10
#define MIN_NSTEP   2

#define	NNODE	((((NPATH + 1) * NPATH + 1) * NPATH + 1) * NPATH + 1)

#define	FALSE	0
#define	TRUE	(!FALSE)

typedef char	BOOL;

#define PID_NIL 0

typedef char    PID;    /* piece id */
typedef char    DID;    /* direction id */
typedef char	TID;	/* team id */

typedef struct piece {
    PID     prev;
    PID     next;
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

typedef struct data {
    PIECE   pieces[1 + NTEAM * NP];
    PID	    board[NROW][NCOL];
    PID	    teams[1 + NTEAM];
    char    npath;
    PATH    longest[NPATH];
} DATA;

typedef struct node NODE;

struct node {
    NODE    *next;
    NODE    *children[NPATH];
    DATA    data;
};

const DIR dirs[] = {{-1, -1}, {-1, 1}, {1, 1}, {1, -1}};
TEAM teams[1 + NTEAM];

NODE nodes[NNODE];
NODE *head;
PATH current;

static inline BOOL is_in_board(char row, char col)
{
    return 0 <= row && row < NROW && 0 <= col && col < NCOL;
}

static inline PID get_first(DATA *data, TID tid)
{
    return data->teams[tid];
}

static inline PID get_next(DATA *data, PID pid)
{
    return data->pieces[pid].next;
}

static inline void path_add_step(PATH *path, char row, char col)
{
    POS *step = &path->steps[path->nstep++];

    step->row = row;
    step->col = col;
}

static inline void path_add_move(DATA *data, char row_orig, char col_orig, char row_new, char col_new)
{
    PATH *path = &data->longest[data->npath++];

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

void update_longest_path(DATA *data)
{
    if (current.nstep < data->longest[0].nstep)
        return;

    if (current.nstep > data->longest[0].nstep)
        data->npath = 0;

    if (data->npath < NPATH)
        memcpy(&data->longest[data->npath++], &current, sizeof (PATH));
}

static inline void piece_drop(DATA *data, PID pid, char row, char col)
{
    PIECE *piece = &data->pieces[pid];

    piece->row = row;
    piece->col = col;
    data->board[row][col] = pid;
}

static inline void piece_crown(DATA *data, PID pid)
{
    PIECE *piece = &data->pieces[pid];

    if (piece->row == teams[piece->tid].row_king)
        piece->king = TRUE;
}

static void piece_kill(DATA *data, PID pid)
{
    PIECE *piece = &data->pieces[pid];
    PID prev = piece->prev;
    PID next = piece->next;

    data->board[piece->row][piece->col] = PID_NIL;

    if (PID_NIL != prev)
        data->pieces[prev].next = next;
    else
        data->teams[piece->tid] = next;

    if (PID_NIL != next)
        data->pieces[next].prev = prev;
}

void piece_jump(DATA *data, PATH *path)
{
    POS *step = &path->steps[0];
    char row = step->row;
    char col = step->col;
    PID  pid = data->board[row][col];

    data->board[row][col] = PID_NIL;
    while (++step < &path->steps[path->nstep]) {
        piece_kill(data, data->board[(row + step->row) / 2][(col + step->col) / 2]);
        row = step->row;
        col = step->col;
    }

    piece_drop(data, pid, row, col);
    piece_crown(data, pid);
}

void piece_move(DATA *data, PATH *path)
{
    char row = path->steps[0].row;
    char col = path->steps[0].col;
    PID  pid = data->board[row][col];

    data->board[row][col] = PID_NIL;

    row = path->steps[1].row;
    col = path->steps[1].col;
    piece_drop(data, pid, row, col);
    piece_crown(data, pid);
}

void piece_place(DATA *data, PATH *path)
{
    if (path->nstep > 2 || 0 == ((path->steps[0].row ^ path->steps[1].row) & 1))
	piece_jump(data, path);
    else
	piece_move(data, path);
}

int find_moveable(DATA *data, TID tid)
{
    PID pid;
    char row_from, row_to;
    char col_from, col_to;
    char i, imax;

    data->npath = 0;

    for (pid = get_first(data, tid); PID_NIL != pid; pid = get_next(data, pid)) {
        PIECE *piece = &data->pieces[pid];
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

            if (is_in_board(row_to, col_to) && PID_NIL == data->board[row_to][col_to])
                path_add_move(data, row_from, col_from, row_to, col_to);
        }
    }

    return data->npath;
}

static void dfs(DATA *data, PID pid);

int find_jumpable(DATA *data, TID tid)
{
    PID pid;

    data->npath = 0;
    data->longest[0].nstep = MIN_NSTEP;

    for (pid = get_first(data, tid); PID_NIL != pid; pid = get_next(data, pid)) {
        current.nstep = 0;
        dfs(data, pid);
    }

    return data->npath;
}

static void dfs(DATA *data, PID pid)
{
    char row0 = data->pieces[pid].row, row1, row2;
    char col0 = data->pieces[pid].col, col1, col2;
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

        m = data->board[row1][col1];
        if (PID_NIL == m || PID_NIL != data->board[row2][col2] || data->pieces[m].tid == data->pieces[pid].tid)
            continue;

        ++sum;

        data->board[row0][col0] = PID_NIL;
        data->board[row1][col1] = PID_NIL;
        piece_drop(data, pid, row2, col2);

        dfs(data, pid);

        piece_drop(data, pid, row0, col0);
        data->board[row1][col1] = m;
        data->board[row2][col2] = PID_NIL;
    }

    if (0 == sum) {
        update_longest_path(data);
    }

    --current.nstep;
}

static inline void node_free(NODE *node)
{
    node->next = head;
    head = node;
}

static inline NODE *node_alloc(void)
{
    NODE *node = head;
    head = node->next;
    node->next = NULL;
    return node;
}

NODE *init_nodes(void)
{
    NODE *node;

    memset(nodes, 0, sizeof nodes);

    head = NULL;
    for (node = &nodes[NNODE - 1]; node >= &nodes[0]; --node)
        node_free(node);

    return node_alloc();
}

void delete_tree(NODE *root)
{
    char i;

    for (i = 0; i < root->data.npath; ++i) {
        NODE *node = root->children[i];
	if (NULL != node)
            delete_tree(node);
    }

    node_free(root);
}

void build_tree(NODE *root, TID tid, int iteration)
{
    DATA *data = &root->data;
    char i;

    if (0 == data->npath)
        find_jumpable(data, tid);

    if (0 == data->npath)
        find_moveable(data, tid);

    for (i = 0; i < data->npath; ++i) {
        NODE *node = node_alloc();
        root->children[i] = node;
	memcpy(&node->data, data, sizeof (DATA));
	piece_place(&node->data, &data->longest[i]);
	node->data.npath = 0;
        if (iteration > 0)
            build_tree(node, tid ^ 3, iteration - 1);
    }
}

void init_pieces(DATA *data)
{
    PID pid;
    TID tid = 1;
    PIECE *piece;

    memset(data->pieces, 0, sizeof data->pieces);
    for (pid = 1, piece = &data->pieces[1]; pid <= NTEAM * NP; ++pid, ++piece) {
        piece->prev = pid - 1;
        piece->next = pid + 1;
        piece->tid  = 1 + (pid > NP);
    }

    data->pieces[ 1].prev = PID_NIL;
    data->pieces[NP].next = PID_NIL;
    data->teams[tid++] = 1;

    data->pieces[NP +  1].prev = PID_NIL;
    data->pieces[NP + NP].next = PID_NIL;
    data->teams[tid] = NP + 1;
}

void init_board(DATA *data)
{
    char row;
    char col;
    PID  pid = 1;
    TEAM *team = &teams[1];

    for (row = NROW - NP * 2 / NCOL; row < NROW; ++row) {
        for (col = (row & 1) ^ 1; col < NCOL; col += 2) {
            piece_drop(data, pid, row, col);
            piece_drop(data, NP + pid, NROW - 1 - row, NCOL - 1 - col);
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

void place(DATA *data, const char *s)
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

    piece_place(data, path);
}

void print_board(DATA *data)
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

            piece = data->pieces + data->board[row][col];
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

const char START[] = "START";
const char PLACE[] = "PLACE";
const char TURN[] = "TURN\n";
const char END[] = "END\n";
const char OK[] = "OK";

void print_path(PATH *path)
{
    POS *step;

    printf("%s %d", PLACE, path->nstep);
    for (step = &path->steps[0]; step < &path->steps[path->nstep]; ++step)
        printf(" %d,%d", step->row, step->col);
    putchar('\n');
}

NODE *turn(NODE *root, TID tid)
{
    DATA *data = &root->data;
    NODE *node = NULL;

    build_tree(root, tid, 0);

    if (0 != data->npath) {
        char i = data->npath / 2; // TODO
        char j = data->npath - 1;
	print_path(&data->longest[i]);
        node = root->children[i];
	if (i != j)
            root->children[i] = root->children[j];
	--data->npath;
	delete_tree(root);
    }

    return node;
}

int main(void)
{
    char line[LINE_LEN + 1];
    int  tid;
    NODE *root = init_nodes();

    init_pieces(&root->data);
    init_board(&root->data);

    setlinebuf(stdin);
    setlinebuf(stdout);

    for (;;) {
	if (NULL == fgets(line, sizeof line - 1, stdin)) {
	    continue;
	}

	if (0 == strncmp(line, END, sizeof END - 1)) {
	    break;
    	}

	if (0 == strncmp(line, START, sizeof START - 1)) {
	    sscanf(&line[sizeof START - 1], "%d", &tid);
            puts(OK);
        } else if (0 == strncmp(line, PLACE, sizeof PLACE - 1)) {
            place(&root->data, &line[sizeof PLACE - 1]);
        } else if (0 == strncmp(line, TURN, sizeof TURN - 1)) {
	    root = turn(root, tid);
        } else {
            printf("DEBUG: Unknown command \"%s\"\n", line);
        }	    
    }

    return 0;
}

