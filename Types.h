#ifndef TYPES_H
#define TYPES_H

#define CAR_NUM 16

typedef unsigned int uint;
typedef FILE File;

typedef enum {A_NULL, A_X, A_Y}Axis;
typedef struct{
    int x; int y;
}Coord;

typedef struct{
    Axis axis;
    bool enabled;
    char letter;
    uint len;
}Car;

Car cars[CAR_NUM] = {
    {
        .letter = 'A',
        .len = 2
    },{
        .letter = 'B',
        .len = 2
    },{
        .letter = 'C',
        .len = 2
    },{
        .letter = 'D',
        .len = 2
    },{
        .letter = 'E',
        .len = 2
    },{
        .letter = 'F',
        .len = 2
    },{
        .letter = 'G',
        .len = 2
    },{
        .letter = 'H',
        .len = 2
    },{
        .letter = 'I',
        .len = 2
    },{
        .letter = 'J',
        .len = 2
    },{
        .letter = 'K',
        .len = 2
    },{
        .letter = 'O',
        .len = 3
    },{
        .letter = 'P',
        .len = 3
    },{
        .letter = 'Q',
        .len = 3
    },{
        .letter = 'R',
        .len = 3
    },{
        .letter = 'X',
        .len = 2
    }
};

typedef struct{
    char grid[6][6];
}State;

typedef struct Move{
    char car;
    int val;
    struct Node *node;
    struct Move *next;
}Move;

typedef struct Node{
    uint lvl;
    State state;
    Move *move;
}Node;

#endif /* end of include guard: TYPES_H */
