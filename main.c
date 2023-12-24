#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "Panic.h"
#include "Types.h"
#include "Util.h"

Move* moveFreeList(Move *m);
// frees node and any decendant branches of moves and nodes
Node* nodeFreeBranch(Node *n)
{
    if(n)
        n->move = moveFreeList(n->move);
    free(n);
    return NULL;
}

// frees all moves in list and all their decendant branches
Move* moveFreeList(Move *m)
{
    while(m){
        Move *next = m->next;
        m->node = nodeFreeBranch(m->node);
        free(m);
        m = next;
    }
    return NULL;
}

// returns number of all possible moves from a given state
size_t stateNumMoves(const State s)
{
    size_t total = 0;
    for(uint i = 0; i < CAR_NUM; i++){
        if(!cars[i].enabled)
            continue;
        for(int val = -1; val < 2; val+=2){
            total += carCanShift(s, cars[i].letter, val);
        }
    }
    return total;
}

bool stateSame(const State a, const State b)
{
    return !memcmp(&a, &b, sizeof(State));
}

// returns a move that is a decendant of root whose resulting state matches s
// does not check the state of root
Move* moveResultStateExists(Node *root, const State s)
{
    assertExpr(root);
    Move *m = root->move;
    while(m){
        assertExpr(m->node);
        if(stateSame(s, m->node->state)){
            printf("m\n");
            return m;
        }
        Move *decendant = moveResultStateExists(m->node, s);
        if(decendant)
            printf("decendant\n");
            return decendant;
        m = m->next;
    }
    return NULL;
}

// generates list of moves that, when applied,
// result in a state that does not occur in any decendants of root
Move* stateMoves(Node *root, const State s, const uint lvl)
{
    (void)root;
    const size_t total = stateNumMoves(s);
    if(!total)
        return NULL;
    Move *m = NULL;
    for(uint i = 0, count = 0; i < CAR_NUM && count < total; i++){
        if(!cars[i].enabled)
            continue;
        for(int val = -1; val < 2; val+=2){
            const char car = cars[i].letter;
            if(carCanShift(s, car, val) && !moveResultStateExists(root, carShift(s, car, val))){
                count++;
                m = moveAppend(m, moveNew(s, lvl, car, val));
            }
        }
    }
    return m;
}

int main(int argc, char **argv)
{
    Node *root = start(argc, argv);
    printf("\nstarting state\n");
    statePrint(root->state);

    printf("moves\n\n");
    root->move = stateMoves(root, root->state, root->lvl);
    for(Move *m = root->move; m; m = m->next){
        for(Move *mm = root->move; mm; mm = mm->next){
            mm->node->move = stateMoves(mm->node, mm->node->state, mm->node->lvl+1);
            movePrint(mm);
            statePrintInd(m->node->state, 1);
            statePrintInd(mm->node->state, 1);
            printf("\n");
        }
        movePrint(m);
        statePrint(root->state);
        statePrint(m->node->state);
        printf("\n");
    }

    printf("\n---------------\n\n");
    // assertExpr(!moveResultStateExists(root, root->state));
    for(Move *m = root->move; m; m = m->next){
        Move *mm = NULL;
        if((mm = moveResultStateExists(m->node, root->state))){
            statePrint(root->state);
            printf("==\n");
            movePrint(mm);
            statePrint(mm->node->state);
        }

    }

    nodeFreeBranch(root);

    return 0;
}
