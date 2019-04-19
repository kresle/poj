
/**
 * 八数码问题，就和八皇后问题一样，在高中的时候绝对做过。
 * 而且当时老师教的应该就是A*算法，十多年过去了，自己居然还是在原地踏步。
 * 
 */

#include <iostream>
#include <vector>
#include <bitset>
#include <string>
#include <cstring>
#include <algorithm>
using namespace std;

struct asNode
{
    // string state;
    char rr, cc;
    asNode *parent;
    char chgByPar; //[0..3]
    int steps;
    int gVal;
    asNode(char ir, char ic, asNode *np, char icb, int istp, int ig)
    {
        // state = ss;
        rr = ir;
        cc = ic;
        parent = np;
        chgByPar = icb;
        steps = istp;
        gVal = ig;
    }
};

const int goal = 123456789;
const string sGoal = "123456789";
// vector<char> status(987654322,-1); // -1未处理；0-closed;1-open
// bitset<987654322> isOpen, isClosed;
vector<bool> isOpen(987654322, false);
vector<bool> isClosed(987654322, false);
vector<asNode*> statePointers(987654322, NULL);

string path, ansPath;
char cmd[] = {'l', 'r', 'u', 'd'};
int ir[] = {0, 0, -1, 1};
int ic[] = {-1, 1, 0, 0};

int hVal(string state)
{
    int ans = 0;
    for (int ii = 0; ii < 9; ii++)
        ans += (sGoal[ii] != state[ii]);
    return ans;
}

// int ManhattanDist(string state)
// {
//     int ans = 0;
//     int rtr, rtc, cr, cc, jj;
//     for(int ii=0;ii<9;ii++)
//     {
//         cr = ii/3;
//         cc = ii%3;
//         jj = state[ii]-'1';
//         rtr = jj/3;
//         rtc = jj%3;
//         ans += abs(cr-rtr)+abs(cc-rtc);
//     }
//     return ans;
// }

// int hVal(string state)
// {
//     return misPlacedCnt(state);
//     // return ManhattanDist(state);
// }

void printState(string& state)
{
    for(int ii=0;ii<3;ii++)
    {
        for(int jj=0;jj<3;jj++)
            cerr << (state[ii*3+jj]=='9'?'x':state[ii*3+jj]);
        cerr << endl;
    }
    cerr << endl;
}

int main()
{
    string state, startState;
    // isOpen.reset();
    // isClosed.reset();
    char rr, cc, pos, num;
    int dState;
    for (char ii = 0; ii < 9; ii++)
    {
        cin >> num;
        if (num == 'x')
            num = '9', pos = ii;
        state += num;
    }
    startState = state;
    rr = pos / 3;
    cc = pos % 3;
    dState = stoi(state);
    statePointers[dState] = new asNode(rr, cc, NULL, -1, 0, hVal(state));
    vector<int> openSet;
    openSet.push_back(dState), isOpen[dState] = true;
    char nr, nc, np, pp;
    int steps, curId;
    bool solvable = false;
    asNode *cur;
    while (!openSet.empty())
    {
        if (openSet.back()==goal)
        {
            solvable = true;
            break;
        }
        curId = openSet.back();
        cur = statePointers[openSet.back()]; //默认openset是按f值大小排序的
        isOpen[openSet.back()] = false, isClosed[openSet.back()] = true, openSet.pop_back();

        steps = cur->steps + 1;
        pp = cur->rr * 3 + cur->cc;
        for (char ii = 0; ii < 4; ii++) //其所有子结点
        {
            nr = cur->rr + ir[ii];
            nc = cur->cc + ic[ii];
            if (nr < 0 || nr >= 3 || nc < 0 || nc >= 3)
                continue;
            np = nr * 3 + nc;
            string state = to_string(curId);
            swap(state[pp], state[np]);
            dState = stoi(state);
            if (isOpen[dState]) //此子结点为open
            {
                if (statePointers[dState]->steps > steps)
                { // gg[dState] = steps;
                    statePointers[dState]->steps = steps;
                    statePointers[dState]->parent = cur;
                    statePointers[dState]->chgByPar = ii;
                }
            }
            else if (isClosed[dState]) //此子结点为closed
            {
                if (statePointers[dState]->steps > steps)
                {
                    statePointers[dState]->steps = steps;
                    statePointers[dState]->parent = cur;
                    statePointers[dState]->chgByPar = ii;
                    isOpen[dState] = true;
                    isClosed[dState] = false;
                    openSet.push_back(dState);
                }
            }
            else //此子结点为未处理过的
            {
                statePointers[dState] = new asNode(nr, nc, cur, ii, steps, hVal(state));
                isOpen[dState] = true;
                openSet.push_back(dState);
            }
        }
        
        sort(openSet.begin(), openSet.end(), [] (int ll, int rr) 
        {
            return statePointers[ll]->steps+statePointers[ll]->gVal > statePointers[rr]->steps + statePointers[rr]->gVal;
        });
    }
    string path;
    if (solvable)
    {
        asNode *cur = statePointers[goal];
        asNode *ed = statePointers[stoi(startState)];
        while(cur&&cur->chgByPar!=-1)
        {
            // printState(cur->state);
            path.push_back(cmd[cur->chgByPar]);
            cur = cur->parent;
        }
        // printState(cur->state);
        reverse(path.begin(), path.end());
        cout << path << endl;
        // cout << "solvable" << endl;

    }
    else
        cout << "unsolvable" << endl;
    return 0;
}