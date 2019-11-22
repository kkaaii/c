#ifndef _DRAUGHTS_DEF_H
#define _DRAUGHTS_DEF_H

#define NR  8
#define NC  8
#define NP  12

struct piece {
    char prev;
    char next;
    char row;
    char col;
    char tint;
    char king;
};

#endif  /* _DRAUGHTS_DEF_H */
