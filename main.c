#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "Panic.h"
#include "Types.h"
#include "Util.h"

size_t nodeTotalMoves(Node *n)
{
    size_t total = 0;
    for(uint i = 0; i < CAR_NUM; i++){
        if(!cars[i].enabled)
            continue;
        for(int val = -1; val < 2; val+=2){
            total += carCanShift(n->state, cars[i].letter, val);
        }
    }
    return total;
}

int main(int argc, char **argv)
{
    Node *root = start(argc, argv);

    for(uint i = 0; i < CAR_NUM; i++){
        printf("car %c: %s\n", cars[i].letter, cars[i].enabled?"enabled":"disabled");
    }

    printf("\nstarting state\n");
    statePrint(root->state);
    printf("%zu possible moves\n", nodeTotalMoves(root));

    // printf("carShift(root->state, 'Q', -1)\n");
    // root->state = carShift(root->state, 'Q', -1);
    // statePrint(root->state);
    // printf("carShift(root->state, 'Q', +1)\n");
    // root->state = carShift(root->state, 'Q', +1);
    // statePrint(root->state);
    // printf("carShift(root->state, 'Q', +1)\n");
    // root->state = carShift(root->state, 'Q', +1);
    // statePrint(root->state);
    //
    // printf("carShift(root->state, 'R', +1)\n");
    // root->state = carShift(root->state, 'R', +1);
    // statePrint(root->state);
    // printf("carShift(root->state, 'R', -1)\n");
    // root->state = carShift(root->state, 'R', -1);
    // statePrint(root->state);
    // printf("carShift(root->state, 'R', -1)\n");
    // root->state = carShift(root->state, 'R', -1);
    // statePrint(root->state);
    // printf("carShift(root->state, 'R', -1)\n");
    // root->state = carShift(root->state, 'R', -1);
    // statePrint(root->state);
    return 0;
}
