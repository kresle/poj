#include <iostream>
#include <bitset>
#include <set>
#include <queue>
#include <vector>
#include <numeric>
#include <algorithm>
using namespace std;

const int permCnt = 362880; //9!
vector<int> fact(10, 1);     // fact[ii] = ii!

struct octNode
{
    int parent = -1;
    int xp = -1;
    char move = 0;
    int fVal;
    int gVal;
    int hVal;
    void setH(int ih)
    {
        hVal = ih;
        fVal = gVal + hVal;
    }
    void setG(int ig)
    {
        gVal = ig;
        fVal = gVal + hVal;
    }
    void setGH(int ig, int ih)
    {
        gVal = ig;
        hVal = ih;
        fVal = gVal + hVal;
    }
};

vector<octNode> allStates(permCnt);
struct stateCmp
{
    bool operator()(int ll, int rr)
    {
        return allStates[ll].fVal > allStates[rr].fVal;
    }
};

bitset<permCnt> isOpen;
bitset<permCnt> isClosed;
vector<int> initPerm(9), goalPerm(9);
priority_queue<int, vector<int>, stateCmp> openSet;
int initState;
int goalState = 0;
char moves[] = "lrud";
int ir[] = {0, 0, -1, 1};
int ic[] = {-1, 1, 0, 0};
int ip[] = {-1, 1, -3, 3};

int getPermutationNum(vector<int> &ss)
{
    int ans = 0, smallerCnt, ii, jj;
    for (ii = 0; ii < 9; ii++)
    {
        smallerCnt = 0;
        for(jj=ii+1;jj<9;jj++)
            smallerCnt+= ss[jj]<ss[ii];
        ans += smallerCnt * fact[8 - ii];
    }
    return ans;
}

void getPermutationFromNum(int num, vector<int> &ss)
{
    vector<int> digits(9);
    for (int ii = 0; ii < 9; ii++)
        digits[ii] = ii + 1;
    for (int ii = 0; ii < 9; ii++)
    {
        int id = num / fact[8 - ii];
        ss[ii] = digits[id];
        num = num % fact[8 - ii];
        digits.erase(digits.begin() + id);
    }
}

void initParam()
{
    for (int ii = 1; ii < 10; ii++)
        fact[ii] = fact[ii - 1] * ii;
    isClosed.reset();
    isOpen.reset();
    getPermutationFromNum(goalState, goalPerm);
    allStates[goalState].xp = 8;
}

void readIn(vector<int> &perm)
{
    char cc;
    int xp = -1;
    for (int ii = 0; ii < 9; ii++)
    {
        cin >> cc;
        perm[ii] = cc == 'x' ? 9 : cc - '0';
        xp = cc == 'x' ? ii : xp;
    }
    initState = getPermutationNum(initPerm);
    allStates[initState].xp = xp;
}

int misPlacedCnt(int state)
{
    vector<int> perm(9);
    getPermutationFromNum(state, perm);
    int hv = 0;
    for (int ii = 0; ii < 9; ii++)
        hv += perm[ii] != goalPerm[ii];
    return hv;
}

inline int myAbs(int nn)
{
    return nn >= 0 ? nn : -nn;
}

int ManhattanDist(int state)
{
    vector<int> perm(9);
    getPermutationFromNum(state, perm);
    int ans = 0;
    int rightr, rightc, nowr, nowc, jj;
    for (int ii = 0; ii < 9; ii++)
    {
        nowr = ii / 3;
        nowc = ii % 3;
        jj = perm[ii] - 1;
        rightr = jj / 3;
        rightc = jj % 3;
        ans += myAbs(nowr - rightr) + myAbs(nowc - rightc);
    }
    return ans;
}

int genNewState(vector<int> perm, int mv)
{
    int rr, cc, nr, nc, pp, np;
    for (pp = 0; pp < 9 && perm[pp] != 9; pp++)
        ;
    if ((pp%3==2 && moves[mv]=='r') || (pp%3==0 && moves[mv]=='l') || (pp/3==0 && moves[mv]=='u') || (pp/3==2 && moves[mv]=='d'))
        return -1;
    np = pp + ip[mv];
    swap(perm[pp], perm[np]);
    return getPermutationNum(perm);
}

void printState(int state)
{
    vector<int> perm(9);
    getPermutationFromNum(state, perm);
    for (int ii = 0; ii < 3; ii++)
    {
        for (int jj = 0; jj < 3; jj++)
            cerr << (char)(perm[ii * 3 + jj] == 9 ? 'x' : '0' + perm[ii * 3 + jj]);
        cerr << endl;
    }
    cerr << "\t\t" << state << " " << allStates[state].gVal << " " << allStates[state].hVal << " " << allStates[state].fVal << endl;
}

bool cmp(int ll, int rr)
{
    return allStates[ll].fVal > allStates[rr].fVal;
}

void printAns(int state)
{
    if (state==initState) return;
    printAns(allStates[state].parent);
    cout << moves[allStates[state].move];
}

int main()
{
    initParam();
    readIn(initPerm);
    allStates[initState].setGH(0, ManhattanDist(initState));
    openSet.push(initState), isOpen[initState] = true;
    int state, nstate;
    vector<int> perm(9);
    bool solvable = false;
    while (!openSet.empty())
    {
        state = openSet.top();
        cerr << state << " " << allStates[state].gVal << " " << allStates[state].hVal << " " << allStates[state].fVal << endl;

        if (state == goalState)
        {
            solvable = true;
            break;
        }
        openSet.pop();
        if (!isOpen[state]) continue;   //用priority_queue时，老旧的数据。
        isOpen[state] = false, isClosed[state] = true; //从开表中删除当前结点。本来应该是访问后删除的，但没区别。

        getPermutationFromNum(state, perm);
        for (int ii = 0; ii < 4; ii++)
        {
            nstate = genNewState(perm, ii);
            if (nstate == -1)
                continue;
            if (isOpen[nstate])
            {
                if (allStates[nstate].gVal > allStates[nstate].gVal + 1)
                {
                    allStates[nstate].setG(allStates[state].gVal + 1);
                    allStates[nstate].parent = state;
                    allStates[nstate].move = ii;
                    openSet.push(nstate);
                }
            }
            else if (isClosed[nstate])
            {
                if (allStates[nstate].gVal > allStates[nstate].gVal + 1)
                {
                    allStates[nstate].setG(allStates[state].gVal + 1);
                    allStates[nstate].parent = state;
                    allStates[nstate].move = ii;
                    isOpen[nstate] = true, openSet.push(nstate); //加入开表
                    isClosed[nstate] = false;                         //从闭表中删除
                }
            }
            else
            {
                allStates[nstate].setGH(allStates[state].gVal + 1, ManhattanDist(nstate));
                allStates[nstate].parent = state;
                allStates[nstate].move = ii;
                isOpen[nstate] = true, openSet.push(nstate); //加入开表
            }
        }
    }
    if (solvable)
        printAns(goalState);
    else
        cout << "unsolvable" << endl;

    return 0;
}