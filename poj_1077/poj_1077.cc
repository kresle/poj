
/**
 * 八数码问题，就和八皇后问题一样，在高中的时候绝对做过。
 * 而且当时老师教的应该就是A*算法，十多年过去了，自己居然还是在原地踏步。
 * 
 */

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
using namespace std;

const int goal = 123456789;
const string sGoal = "123456789";
bool visited[987654322];
int  funcG[987654322];
string path, ansPath;
char cmd[] = {'u', 'd', 'l', 'r'};
int ir[] = {0, 0, -1, 1};
int ic[] = {-1, 1, 0, 0};

int funcH(const string& state)
{
    int ans = 0;
    for(int ii=0;ii<9;ii++)
        ans+= (sGoal[ii]!=state[ii]);
    return ans;
}

//简单dfs会栈溢出。
bool dfs(string &state, int rr, int cc, int steps = 0)
{
    cout << state << endl;
    if (rr < 0 || rr >= 3 || cc < 0 || cc >= 3)
        return false;
    if (state == sGoal)
    {
        ansPath = path;
        return true;
    }
    int dstate = stoi(state);
    visited[dstate] = true;
    funcG[dstate] = steps;
    int nr, nc, np, pp = rr * 3 + cc;
    for (int kk = 0; kk < 4; kk++)
    {
        nr = rr + ir[kk];
        nc = cc + ic[kk];
        if (nr < 0 || nr >= 3 || nc < 0 || nc >= 3)
            continue;
        np = nr * 3 + nc;
        swap(state[pp], state[np]);
        if (!visited[stoi(state)])
        {
            path.push_back(cmd[kk]);
            if (dfs(state, nr, nc, steps+1))
                return true;
            path.pop_back();
        }
        swap(state[pp], state[np]);
    }
    return false;
}

int main()
{
    cout << "hello" << endl;
    memset(visited, 0, sizeof(visited));
    memset(funcG, 0, sizeof(funcG));
    string state;
    int rr, cc, pos, digit;
    char num;
    for (int ii = 0; ii < 9; ii++)
    {
        cin >> num;
        if (num == 'x')
            num = '9', pos = ii;
        state += num;
    }
    rr = pos / 3;
    cc = pos % 3;
    cerr << "Before DFS" << endl;
    if (dfs(state, rr, cc))
        cout << ansPath << endl;
    else
        cout << "unsolvable" << endl;
}