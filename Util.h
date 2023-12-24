#ifndef UTIL_H
#define UTIL_H

bool inGrid(const Coord pos)
{
    return pos.x >= 0 && pos.x < 6 && pos.y >= 0 && pos.y < 6;
}

int imax(const int a, const int b)
{
    return a > b ? a : b;
}

int imin(const int a, const int b)
{
    return a < b ? a : b;
}

Coord iC(const int x, const int y)
{
    return (Coord){x, y};
}

void carsPrintEnabled(void)
{
    for(uint i = 0; i < CAR_NUM; i++){
        printf("car %c: %s\n", cars[i].letter, cars[i].enabled?"enabled":"disabled");
    }
}

size_t carCharIndex(const char c)
{
    if(c == '-')
        return CAR_NUM;
    for(size_t i = 0; i < CAR_NUM; i++){
        if(cars[i].letter == c)
            return i;
    }
    panic("CarIndex for char '%c' does not exist!", c);
    return 0;
}

State stateEmpty(void)
{
    return (State){
        .grid = {
            {'-','-','-','-','-','-'},
            {'-','-','-','-','-','-'},
            {'-','-','-','-','-','-'},
            {'-','-','-','-','-','-'},
            {'-','-','-','-','-','-'},
            {'-','-','-','-','-','-'}
        }
    };
}

void errAxis(const char c)
{
    panic("Could not derive car axis for car '%c'", c);
}

void statePrint(const State s)
{
    for(uint y = 0; y < 6; y++){
        for(uint x = 0; x < 6; x++){
            printf("%c ", s.grid[x][y]);
        }
        printf("\n");
    }
    printf("\n");
}

void statePrintInd(const State s, const uint lvl)
{
    for(uint y = 0; y < 6; y++){
        for(uint i = 0; i < lvl; i++)
            printf("\t");
        for(uint x = 0; x < 6; x++){
            printf("%c ", s.grid[x][y]);
        }
        printf("\n");
    }
    printf("\n");
}

Axis axisInv(const Axis a)
{
    assertExpr(a > A_NULL && a <= A_Y);
    return a == A_Y ? A_X : A_Y;
}

Axis axisGet(const State s, const Coord pos)
{
    for(int x = imax(0, pos.x); imin(6, x < pos.x+1); x++){
        for(int y = imax(0, pos.y); imin(6, y < pos.y+1); y++){
            if(x == pos.x && y == pos.y)
                continue;
            if(s.grid[pos.x][pos.y] == s.grid[x][y])
                return x == pos.x ? A_Y : A_X;
        }
    }
    errAxis(s.grid[pos.x][pos.y]);
    return 0;
}

Node* start(const int argc, char **argv)
{
    File *f = NULL;
    if(argc != 2)
        panic("Program requires 1 argument");
     if(!(f = fopen(argv[1], "r")))
        panic("could not open level file \"%s\"", argv[1]);

    Node *node = calloc(1, sizeof(Node));
    if(!node)
        panic("couldn't allocate newNode");
    int c;
    for(uint y = 0; y < 6; y++){
        for(uint x = 0; x < 6; x++){
            c = fgetc(f);
            node->state.grid[x][y] = c;
        }
        c = fgetc(f);
    }
    assertExpr(c == '\n');
    fclose(f);
    printf("read level file \"%s\"\n", argv[1]);

    for(uint y = 0; y < 6; y++){
        for(uint x = 0; x < 6; x++){
            const int i = carCharIndex(node->state.grid[x][y]);
            if(node->state.grid[x][y] != '-' && !cars[i].enabled){
                cars[i].enabled = true;
                if(x < 5 && node->state.grid[x+1][y] == node->state.grid[x][y])
                    cars[i].axis = A_X;
                else if(y < 5 && node->state.grid[x][y+1] == node->state.grid[x][y])
                    cars[i].axis = A_Y;
                else
                    errAxis(node->state.grid[x][y]);
            }
        }
    }

    return node;
    printf("parsed level file \"%s\"\n", argv[1]);
}

Coord carStart(const State s, const char car)
{
    for(int y = 0; y < 6; y++){
        for(int x = 0; x < 6; x++){
            if(s.grid[x][y] == car)
                return iC(x, y);
        }
    }
    panic("start of car '%c' not found", car);
    return iC(0, 0);
}

Coord carEnd(const State s, const char car)
{
    for(int y = 5; y >= 0; y--){
        for(int x = 5; x >= 0; x--){
            if(s.grid[x][y] == car)
                return iC(x, y);
        }
    }
    panic("end of car '%c' not found", car);
    return iC(0, 0);
}

Coord coordShift(const Coord pos, const Axis a, const int val)
{
    assertExpr(a > A_NULL && a <= A_Y);
    return a == A_X ? (Coord){.x=pos.x+val, .y=pos.y} : (Coord){.x=pos.x, .y=pos.y+val};
}

bool carCanShift(State s, const char car, const int val)
{
    assertExpr(abs(val) == 1);
    const Coord a = val > 0 ? carStart(s, car) : carEnd(s, car);
    assertExpr(inGrid(a));
    const Coord b = coordShift(a, cars[carCharIndex(car)].axis, cars[carCharIndex(car)].len*val);
    return inGrid(b) && s.grid[b.x][b.y] == '-';
}

State carShift(State s, const char car, const int val)
{
    assertExpr(carCanShift(s, car, val));
    const Coord a = val > 0 ? carStart(s, car) : carEnd(s, car);
    const Coord b = coordShift(a, cars[carCharIndex(car)].axis, cars[carCharIndex(car)].len*val);
    s.grid[b.x][b.y] = car;
    s.grid[a.x][a.y] = '-';
    return s;
}

Node* nodeNew(const uint lvl, const State state)
{
    Node *n = calloc(1, sizeof(Node));
    assertExpr(n);
    n->lvl = lvl;
    n->state = state;
    return n;
}

void movePrint(Move *m)
{
    assertExpr(m);
    for(uint i = 0; i < m->lvlbefore; i++)
        printf("\t");
    printf("%c %+i (lvl %u before)\n", m->car, m->val, m->lvlbefore);
}

Move* moveNew(const State s, const uint lvl, const char car, const int val)
{
    assertExpr(carCanShift(s, car, val));
    Move *m = calloc(1, sizeof(Move));
    assertExpr(m);
    m->car = car;
    m->val = val;
    m->lvlbefore = lvl;
    m->node = nodeNew(lvl+1, carShift(s, car, val));
    return m;
}

Move* moveAppend(Move *head, Move *tail)
{
    if(!head)
        return tail;
    Move *cur = head;
    while(cur->next)
        cur = cur->next;
    cur->next = tail;
    return head;
}

#endif /* end of include guard: UTIL_H */
