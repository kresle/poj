#include <iostream>
#include <stdio.h>
#include <queue>
#include <vector>
#include <cstring>
#include <bitset>
using namespace std;

const int permCnt = 362880; //9!
int fact[] = {1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880};

int parent[permCnt];                               //父状态
char fVal[permCnt], hVal[permCnt], gVal[permCnt]; //对应f,h,g值
char direction[permCnt];                           //怎么从父状态而来
char perms[permCnt][9];                            //对应的拼图
char xpos[permCnt];                                //‘x’或‘9’在拼图中的位置
bitset<permCnt> isOpen, isClosed;

int initState, goalState = 0;

char moves[] = "lrud";
int ip[] = {-1, 1, -3, 3};

int perm2state(char perm[9])
{
    int ans = 0, smallerCnt, ii, jj;
    for (ii = 0; ii < 9; ii++)
    {
        smallerCnt = 0;
        for(jj=ii+1;jj<9;jj++)
            smallerCnt+= perm[jj]<perm[ii];
        ans += smallerCnt * fact[8 - ii];
    }
    return ans;
}

void state2perm(int state)
{
    if (perms[state][0] != '?')
        return;
    vector<int> digits(9);
    for (int ii = 0; ii < 9; ii++)
        digits[ii] = ii + 1;
    int num = state;
    for (int ii = 0; ii < 9; ii++)
    {
        int id = num / fact[8 - ii];
        perms[state][ii] = digits[id];
        if (digits[id]==9)
            xpos[state] = ii;
        num = num % fact[8 - ii];
        digits.erase(digits.begin() + id);
    }
}

inline int myAbs(int nn) { return nn>=0?nn:-nn;}
int ManhattanDist(int state)
{
    int ans = 0;
    int rightr, rightc, nowr, nowc, jj;
    for (int ii = 0; ii < 9; ii++)
    {
        nowr = ii / 3;
        nowc = ii % 3;
        jj = perms[state][ii] - 1;
        rightr = jj / 3;
        rightc = jj % 3;
        ans += myAbs(nowr - rightr) + myAbs(nowc - rightc);
    }
    return ans;
}

void initParam()
{
    memset(parent, -1, sizeof(parent));
    memset(fVal, 0, sizeof(fVal));
    memset(gVal, 0, sizeof(gVal));
    memset(hVal, 0, sizeof(hVal));
    memset(direction, '?', sizeof(direction));
    memset(perms, '?', permCnt*9);
    memset(xpos, '?', sizeof(xpos));
    xpos[0] = 8;
    for(int ii=0;ii<9;ii++) perms[0][ii] = ii;
}

void readInitState()
{
    char tmperm[9], tmpxp;
    for(int ii=0;ii<9;ii++)
    {
        cin >> tmperm[ii];
        if (tmperm[ii]=='x')
        {   
            tmperm[ii] = 9;
            tmpxp = ii;
        }
        else
            tmperm[ii]-='0';
    }
    initState = perm2state(tmperm);
    memcpy(perms[initState], tmperm, 9);
    xpos[initState] = tmpxp;
    hVal[initState] = ManhattanDist(initState);
    fVal[initState] = hVal[initState];
}

struct fcmp
{
    bool operator()(int ll, int rr)
    {
        return fVal[ll] > fVal[rr];
    }
};

int generateNewState(int state, int mv)
{
    int rr, cc, nr, nc, pp, np;
    pp = xpos[state];
    if ((pp%3==2 && moves[mv]=='r') || (pp%3==0 && moves[mv]=='l') || (pp/3==0 && moves[mv]=='u') || (pp/3==2 && moves[mv]=='d'))
        return -1;
    char perm[9];
    memcpy(perm, perms[state], 9);
    np = pp + ip[mv];
    swap(perm[pp], perm[np]);
    int nstate = perm2state(perm);
    if (xpos[nstate]!='?') return nstate;
    xpos[nstate] = np;
    memcpy(perms[nstate], perm, 9);
    return nstate;
}

void updateState(int state, int par, int ii)
{
    parent[state] = par;
    direction[state] = ii;
    gVal[state] = gVal[par] + 1;
    fVal[state] = gVal[state] + hVal[state];
}

void AStar()
{
    isOpen.reset();
    isClosed.reset();
    priority_queue<int, vector<int>, fcmp> openSet;
    openSet.push(initState);
    isOpen[initState] = true;
    int state, nstate;
    while(!openSet.empty())
    {
        state = openSet.top();
        openSet.pop();
        if (state==goalState)
            break;
        if (!isOpen[state]) continue;
        isOpen[state] = false;
        isClosed[state] = true;
        for(int ii=0;ii<4;ii++)
        {
            nstate = generateNewState(state, ii);
            if (nstate==-1) continue;
            if (isOpen[nstate])
            {
                if (gVal[nstate] > gVal[state] + 1)
                {
                    updateState(nstate, state, ii);
                    openSet.push(nstate);
                }
            }
            else if (isClosed[nstate])
            {
                if (gVal[nstate] > gVal[state] + 1)
                {
                    updateState(nstate, state, ii);
                    isOpen[nstate] = true, openSet.push(nstate); //加入开表
                    isClosed[nstate] = false;                         //从闭表中删除
                }
            }
            else
            {
                hVal[nstate] = ManhattanDist(nstate);
                updateState(nstate, state, ii);
                isOpen[nstate] = true, openSet.push(nstate); //加入开表
            }
            if (nstate==goalState) break;
        }
    }
}

void printAns(int state)
{
    if (state == initState)
        return;
    printAns(parent[state]);
    cout << moves[direction[state]];
}

int main()
{
    initParam();
    readInitState();
    AStar();
    if (parent[goalState] != -1)
    {
        printAns(goalState);
        cout << endl;
    }
    else
        cout << "unsolvable" << endl;
}