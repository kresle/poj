#include <iostream>
#include <bitset>
#include <set>
#include <queue>
#include <vector>
#include <numeric>
#include <algorithm>
#include <math.h>
using namespace std;

// vector<int> fact(10, 1);    // fact[ii] = ii!
const int fact[] = {1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880};
const int permCnt = 362880; //9!

struct octNode
{
    int parent;
    char move;
    int hVal;
};

vector<octNode> sStates(permCnt);
vector<octNode> tStates(permCnt);

bitset<permCnt> sVisited, tVisited;

// vector<bool> visited(permCnt, false);
vector<int> initPerm(9), goalPerm(9);
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
        for (jj = ii + 1; jj < 9; jj++)
            smallerCnt += ss[jj] < ss[ii];
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
    // visited.reset();
    getPermutationFromNum(goalState, goalPerm);
}

void readIn(vector<int> &perm, int &state)
{
    char cc;
    for (int ii = 0; ii < 9; ii++)
    {
        cin >> cc;
        perm[ii] = cc == 'x' ? 9 : cc - '0';
    }
    state = getPermutationNum(perm);
}

int genNewState(vector<int> perm, int mv)
{
    int rr, cc, nr, nc, pp, np;
    for (pp = 0; pp < 9 && perm[pp] != 9; pp++)
        ;
    if ((pp % 3 == 2 && moves[mv] == 'r') || (pp % 3 == 0 && moves[mv] == 'l') || (pp / 3 == 0 && moves[mv] == 'u') || (pp / 3 == 2 && moves[mv] == 'd'))
        return -1;
    np = pp + ip[mv];
    swap(perm[pp], perm[np]);
    return getPermutationNum(perm);
}

vector<int> tmpPerm(9);
void printAns(int state, bool reverseMode = true)
{

    if (reverseMode)
    {
        if (state == initState)
            return;
        printAns(sStates[state].parent, reverseMode);
        cout << sStates[state].move;
    }
    else
    {
        if (state == goalState)
            return;
        cout << tStates[state].move;
        printAns(tStates[state].parent, reverseMode);
    }
}

void bfs(priority_queue<int> &qq, bitset<permCnt> &visited, vector<octNode> &allStates)
{
    int state, nstate;
    vector<int> perm(9);
    state = qq.top();
    // cerr << state << endl;
    qq.pop();
    getPermutationFromNum(state, perm);
    for (int ii = 0; ii < 4; ii++)
    {
        nstate = genNewState(perm, ii);
        if (nstate == -1 || visited[nstate])
            continue;
        allStates[nstate].parent = state;
        allStates[nstate].move = moves[ii];
        visited[nstate] = true;
        qq.push(nstate);
    }
}

int isIntersecting()
{
    // unsigned long long tmp = (sVisited & tVisited).to_ullong();
    bitset<permCnt> stInt = sVisited & tVisited;
    // cerr << "\t" << sVisited.to_ullong() << " " << tVisited.to_ullong() << " "  << tmp << endl;
    if (stInt.any())
    {
        for (int ii = 0; ii < permCnt; ii++)
            if (stInt[ii])
                return ii;
    }
    else
        return -1;
}

struct sCmp
{
    bool operator()(int ll, int rr)
    {
        return sStates[ll].hVal > sStates[rr].hVal;
    }
};

struct tCmp
{
    bool operator()(int ll, int rr)
    {
        return sStates[ll].hVal > sStates[rr].hVal;
    }
};

int biBFS(int s = initState, int t = goalState)
{
    sVisited.reset(), tVisited.reset();
    // queue<int> sQ, tQ;
    priority_queue<int, vector<int>, sCmp> sQ;
    priority_queue<int, vector<int>, tCmp> tQ;
    int intersectState = -1;

    sQ.push(s);
    sVisited[s] = true;
    sStates[s].parent = -1;

    tQ.push(t);
    tVisited[t] = true;
    tStates[t].parent = -1;

    while (!sQ.empty() && !tQ.empty())
    {
        bfs(sQ, sVisited, sStates);
        bfs(tQ, tVisited, tStates);
        sVisited._Find_first

        intersectState = isIntersecting();
        // cerr << string(100, '-') << endl;

        if (intersectState != -1)
        {
            printAns(intersectState);
            printAns(intersectState, false);
            exit(0);
        }
    }
    return -1;
}

int main()
{
    initParam();
    readIn(initPerm, initState);
    if (biBFS(initState, goalState) == -1)
        cout << "unsolvable" << endl;
    return 0;
}