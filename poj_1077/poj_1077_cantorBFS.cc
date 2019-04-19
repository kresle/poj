#include <iostream>
#include <bitset>
#include <set>
#include <queue>
#include <vector>
#include <numeric>
#include <algorithm>
using namespace std;

const int permCnt = 362880; //9!
// vector<int> fact(10, 1);    // fact[ii] = ii!
vector<int> fact = {1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880};

struct octNode
{
    int parent;
    char move;
};

vector<octNode> allStates(permCnt);

bitset<permCnt> visited;
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
    visited.reset();
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

void printAns(int state)
{
    if (state == initState)
        return;
    printAns(allStates[state].parent);
    cout << allStates[state].move;
}

void bfs()
{
    queue<int> qq;
    qq.push(initState);
    allStates[initState].parent = -1;
    visited[initState] = true;
    int state, nstate;
    vector<int> perm(9);
    while (!qq.empty())
    {
        state = qq.front();
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
            if (nstate == goalState)
                return;
            qq.push(nstate);
        }
    }
}

int main()
{
    initParam();
    readIn(initPerm, initState);
    bfs();
    if (visited[goalState])
        printAns(goalState);
    else
        cout << "unsolvable" << endl;
    return 0;
}