//最大流
// 还没有理解的地方：
// 1. bfs里，算法是不允许出现环的，为什么
// 2. 对residual graph的深入理解，以及与上一个问题，也就是环的联系
// 3. 本问题里，对adapter设置成INF的运行原理
// 4. 这里用了bfs，但教程里是不对bfs和dfs作要求的，两个都可以。那区别体现在哪里？

// 22:42开始写 00:20在比赛提供的所有10个样例上进行了测试，答案都一样，但是在poj上会RE.
// 奇怪，没改代码，poj又过了。
// 这道题，第一次写好，运行不对死循环的原因，只是自己粗心，在第一次循环得到flow以后，在进行对rgraph的更新前，又把flow = INF的代码贴了过来。
// c++编译器似乎是比g++编译器要宽容

// for ii in {1..10}
// do
// ./a.out < C/C.$ii.dat && cat C/C.$ii.diff
// done

#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <map>
using namespace std;

#define INF 0x3f3f3f3f

int nSk_t, nSk_a, nDv, nAd, nn;
vector<string> sockets;
vector<string> devices;
vector<string> device_skts;
map<string, int> name2id;
vector<vector<int> > graph, rGraph;
vector<int> pred;
int ans;

bool bfs()
{
    pred.clear(), pred.resize(nn, -1);
    queue<int> qq;
    qq.push(0);
    pred[0] = 0;
    while (!qq.empty())
    {
        if (qq.front() == nn - 1)
            return true;
        for (int ii = 0; ii < nn; ii++)
        {
            if (pred[ii] == -1 && rGraph[qq.front()][ii])
            {
                pred[ii] = qq.front();
                qq.push(ii);
            }
        }
        qq.pop();
    }
    return false;
}

void maxFlow()
{
    rGraph = graph;
    ans = 0;
    int vv, uu, flow;
    while (bfs())
    {
        flow = INF;
        for (vv = nn - 1; vv != 0; vv = pred[vv])
        {
            uu = pred[vv];
            flow = min(flow, rGraph[uu][vv]);
        }
        ans += flow;

        for (vv = nn - 1; vv != 0; vv = pred[vv])
        {
            uu = pred[vv];
            rGraph[uu][vv] -= flow;
            rGraph[vv][uu] += flow;
        }
    }
}

int main()
{
    string sk, dv, ad1, ad2;
    
    cin >> nSk_t;
    nSk_a = nSk_t;
    for (int ii = 0; ii < nSk_t; ii++)
    {
        cin >> sk;
        sockets.push_back(sk);
        name2id[sk] = ii + 1;
    }

    cin >> nDv;
    for (int ii = 0; ii < nDv; ii++)
    {
        cin >> dv >> sk;
        devices.push_back(dv);
        device_skts.push_back(sk);
        if (name2id.find(sk) == name2id.end())
            sockets.push_back(sk), name2id[sk] = ++nSk_a;
    }

    nn = nSk_a;
    for (int ii = 0; ii < nDv; ii++)
    {
        name2id[devices[ii]] = ++nn;
    }

    nn += 2;
    graph.resize(nn, vector<int>(nn, 0));

    cin >> nAd;
    for (int ii = 0; ii < nAd; ii++)
    {
        cin >> ad1 >> ad2;
        graph[name2id[ad1]][name2id[ad2]] = INF;
    }

    for (int ii = 1; ii <= nSk_t; ii++)
        graph[ii][nn - 1] = 1;
    
    for (int ii = nSk_a + 1; ii < nn - 1; ii++)
        graph[0][ii] = 1;

    for (int ii = 0; ii < devices.size(); ii++)
        graph[name2id[devices[ii]]][name2id[device_skts[ii]]] = 1;

    maxFlow();
    cout << nDv - ans << endl;
}