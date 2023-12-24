#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "Panic.h"
#include "Types.h"
#include "Util.h"

Node* newNode(Node *parent, const char car, const int val)
{
    Node *node = calloc(1, sizeof(Node));
    if(!node)
        panic("couldn't allocate newNode for car '%c', val %i", car, val);
    assertExpr(parent);
    node->car = car;
    node ->val = val;
    node->state = parent->state;
    return node;
}

int main(int argc, char **argv)
{
    Node *root = start(argc, argv);

    for(uint i = 0; i < CAR_NUM; i++){
        printf("car %c: %s\n", cars[i].letter, cars[i].enabled?"enabled":"disabled");
    }

    printf("\nstarting state\n");
    statePrint(root->state);
    printf("carShift(root->state, 'Q', -1)\n");
    root->state = carShift(root->state, 'Q', -1);
    statePrint(root->state);
    printf("carShift(root->state, 'Q', +1)\n");
    root->state = carShift(root->state, 'Q', +1);
    statePrint(root->state);
    printf("carShift(root->state, 'Q', +1)\n");
    root->state = carShift(root->state, 'Q', +1);
    statePrint(root->state);

    printf("carShift(root->state, 'R', +1)\n");
    root->state = carShift(root->state, 'R', +1);
    statePrint(root->state);
    printf("carShift(root->state, 'R', -1)\n");
    root->state = carShift(root->state, 'R', -1);
    statePrint(root->state);
    printf("carShift(root->state, 'R', -1)\n");
    root->state = carShift(root->state, 'R', -1);
    statePrint(root->state);
    printf("carShift(root->state, 'R', -1)\n");
    root->state = carShift(root->state, 'R', -1);
    statePrint(root->state);
    return 0;
}
