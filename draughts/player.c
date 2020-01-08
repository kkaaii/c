#include <string.h>

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

#define	NNODE	((((NPATH + 1) * NPATH + 1) * NPATH + 1) * NPATH + 1)

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

typedef struct node NODE;

struct node {
    NODE    *next;
    PIECE   pieces[1 + NTEAM * NP];
    PID	    board[NROW][NCOL];
    PID	    teams[1 + NTEAM];
    char    npath;
    PATH    longest[NPATH];
    NODE    *children[NPATH];
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

static inline PID get_first(NODE *node, TID tid)
{
    return node->teams[tid];
}

static inline PID get_next(NODE *node, PID pid)
{
    return node->pieces[pid].next;
}

static inline void path_add_step(PATH *path, char row, char col)
{
    POS *step = &path->steps[path->nstep++];

    step->row = row;
    step->col = col;
}

static inline void path_add_move(NODE *node, char row_orig, char col_orig, char row_new, char col_new)
{
    PATH *path = &node->longest[node->npath++];

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

void update_longest_path(NODE *node)
{
    if (current.nstep < node->longest[0].nstep)
        return;

    if (current.nstep > node->longest[0].nstep)
        node->npath = 0;

    if (node->npath < NPATH)
        memcpy(&node->longest[node->npath++], &current, sizeof (PATH));
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
    PID prev = piece->prev;
    PID next = piece->next;

    node->board[piece->row][piece->col] = PID_NIL;

    if (PID_NIL != prev)
        node->pieces[prev].next = next;
    else
        node->teams[piece->tid] = next;

    if (PID_NIL != next)
        node->pieces[next].prev = prev;
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

void piece_place(NODE *node, PATH *path)
{
    if (path->nstep > 2 || 0 == ((path->steps[0].row ^ path->steps[1].row) & 1))
	piece_jump(node, path);
    else
	piece_move(node, path);
}

int find_moveable(NODE *node, TID tid)
{
    PID pid;
    char row_from, row_to;
    char col_from, col_to;
    char i, imax;

    node->npath = 0;

    for (pid = get_first(node, tid); PID_NIL != pid; pid = get_next(node, pid)) {
        PIECE *piece = &node->pieces[pid];
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

            if (is_in_board(row_to, col_to) && PID_NIL == node->board[row_to][col_to])
                path_add_move(node, row_from, col_from, row_to, col_to);
        }
    }

    return node->npath;
}

static void dfs(NODE *node, PID pid);

int find_jumpable(NODE *node, TID tid)
{
    PID pid;

    node->npath = 0;
    node->longest[0].nstep = MIN_NSTEP;

    for (pid = get_first(node, tid); PID_NIL != pid; pid = get_next(node, pid)) {
        current.nstep = 0;
        dfs(node, pid);
    }

    return node->npath;
}

static void dfs(NODE *node, PID pid)
{
    char row0 = node->pieces[pid].row, row1, row2;
    char col0 = node->pieces[pid].col, col1, col2;
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

        m = node->board[row1][col1];
        if (PID_NIL == m || PID_NIL != node->board[row2][col2] || node->pieces[m].tid == node->pieces[pid].tid)
            continue;

        ++sum;

        node->board[row0][col0] = PID_NIL;
        node->board[row1][col1] = PID_NIL;
        piece_drop(node, pid, row2, col2);

        dfs(node, pid);

        piece_drop(node, pid, row0, col0);
        node->board[row1][col1] = m;
        node->board[row2][col2] = PID_NIL;
    }

    if (0 == sum) {
        update_longest_path(node);
        current.nstep = 0;
    }
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

    for (i = 0; i < root->npath; ++i) {
        NODE *node = root->children[i];
	if (NULL != node)
            delete_tree(node);
    }

    node_free(root);
}

void build_tree(NODE *root, TID tid, int iteration)
{
    char i;

    if (0 == root->npath)
        find_jumpable(root, tid);

    if (0 == root->npath)
        find_moveable(root, tid);

    for (i = 0; i < root->npath; ++i) {
        NODE *node = node_alloc();
        root->children[i] = node;
	memcpy(node, root, sizeof *node);
	piece_place(node, &root->longest[i]);
	node->npath = 0;
        if (iteration > 0)
            build_tree(node, tid ^ 3, iteration - 1);
    }
}

void init_pieces(NODE *node)
{
    PID pid;
    TID tid = 1;
    PIECE *piece;

    memset(node->pieces, 0, sizeof node->pieces);
    for (pid = 1, piece = &node->pieces[1]; pid <= NTEAM * NP; ++pid, ++piece) {
        piece->prev = pid - 1;
        piece->next = pid + 1;
        piece->tid  = 1 + (pid > NP);
    }

    node->pieces[ 1].prev = PID_NIL;
    node->pieces[NP].next = PID_NIL;
    node->teams[tid++] = 1;

    node->pieces[NP +  1].prev = PID_NIL;
    node->pieces[NP + NP].next = PID_NIL;
    node->teams[tid] = NP + 1;
}

void init_board(NODE *node)
{
    char row;
    char col;
    PID  pid = 1;
    TEAM *team = &teams[1];

    for (row = NROW - NP * 2 / NCOL; row < NROW; ++row) {
        for (col = (row & 1) ^ 1; col < NCOL; col += 2) {
            piece_drop(node, pid, row, col);
            piece_drop(node, NP + pid, NROW - 1 - row, NCOL - 1 - col);
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

void place(NODE *node, const char *s)
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

    piece_place(node, path);
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

NODE *turn(NODE *root, TID tid)
{
    NODE *node = NULL;

    build_tree(root, tid, 0);

    if (0 != root->npath) {
        char i = root->npath / 2; // TODO
        char j = root->npath - 1;
        node = root->children[i];
	if (i != j)
            root->children[i] = root->children[j];
	--root->npath;
	delete_tree(root);
    }

    return node;
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

int main(void)
{
    char line[LINE_LEN + 1];
    int  tid;
    NODE *root = init_nodes();

    init_pieces(root);
    init_board(root);

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
            place(root, &line[sizeof PLACE - 1]);
        } else if (0 == strncmp(line, TURN, sizeof TURN - 1)) {
	    root = turn(root, tid);
        } else {
            printf("DEBUG: Unknown command \"%s\"\n", line);
        }	    
    }

    return 0;
}

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
