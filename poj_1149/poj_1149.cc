//17：46 - 00:06(AC)

/**
 * 一开始靠自己想，怎么都想不出解决钥匙的办法来，尝试了两三种自己想的思路，都不对，都是类似于贪心的思路，
 * 都会影响到后续状态而没法找到最优解。
 * 在看了别人的思路后，2b非要用并查集的思路来解，却忘了phid和cid是非常有可能有重叠的，这样用parent[phid]==phid来判断是不是第一次遇到的时候，
 * 就可能遇到之前在设置parent[phid]=cid时，phid==cid的情况，而在后面再遇到的时候，还当成第一次遇到。
 * 就找这个bug找了可能有两三个钟头（2019-03-14 21:49:38，WA开始就是这个问题，也就是2小时10多分钟）。
 * 真的是，就是pat第一题考砸影响心情导致后面也做得不好一样，都是因为想做没必要的炫技使用复杂方法结果搞砸了的。
 * 方法还是，越简单越好，能实现功能先，优化什么的，等TLE再说。
 * 越简单，出错的概率越低，鲁棒性越高。
 * 此题别的方面，maxFlow以及bfs算法上，并没有出问题，写得还是比较快的。
 * 可以看看运行时间统计，后面的样例比从别人那里copy过来的代码要慢不少好像。
 * 好多别的地方提到用的是Dinic算法，有机会学习一下。
 */

#include <iostream>
#include <vector>
#include <queue>
using namespace std;

#define INF 0x3f3f3f3f
bool verbose = false;

int mm, nn; // pighouses, customers
int sid, tid;   //0: source, sid；      nn+1: sink, tid
vector<vector<int> > graph; //1..nn: customers
vector<vector<int> > rGraph;

int maxflow;
vector<int> pred;

bool bfs()
{
    fill(pred.begin(), pred.end(), -1);
    queue<int> qq;
    qq.push(0);
    pred[0] = 0;
    while (!qq.empty())
    {
        if (qq.front() == tid)
            return true;
        for (int ii = 0; ii <= tid; ii++)
            if (pred[ii] == -1 && rGraph[qq.front()][ii])
            {
                pred[ii] = qq.front();
                qq.push(ii);
            }
        qq.pop();
    }
    return false;
}

void maxFlow()
{
    rGraph = graph;
    pred.reserve(tid + 1), pred.resize(tid + 1);
    maxflow = 0;
    int uu, vv, flow;
    while (bfs()) //先针对当前用户，注入最多到终点
    {
        flow = INF;
        for (vv = tid; vv != sid; vv = pred[vv])
        {
            uu = pred[vv];
            if (verbose) cerr << vv << "("<<rGraph[uu][vv] << ")<-";
            flow = min(flow, rGraph[uu][vv]);
        }
        if (verbose) cerr << sid << ": flow=" << flow << endl;
        maxflow += flow;
        for (vv = tid; vv != sid; vv = pred[vv])
        {
            uu = pred[vv];
            rGraph[uu][vv] -= flow;
            rGraph[vv][uu] += flow;
        }
    }
}

int main()
{
    int cid, kk, phid;
    cin >> mm >> nn;
    sid = 0, tid = nn + 1;
    graph.resize(tid + 1, vector<int>(tid + 1, 0));
    vector<int> pigQuotas(mm+1);
    vector<int> parent(mm+1,0);//类似于并查集的思路
    for (phid = 1; phid <= mm; phid++)
        cin >> pigQuotas[phid];
    for (cid = 1; cid <= nn; cid++) //[1,nn]
    {
        cin >> kk;
        while (kk--)
        {
            cin >> phid;
            if (parent[phid]==0)//第一次打开
            {
                graph[0][cid]+= pigQuotas[phid];
                parent[phid] = cid;
            }
            else    //非第一次打开
                graph[parent[phid]][cid] = INF;
        }
        cin >> graph[cid][tid];
    }
    maxFlow();
    cout << maxflow << endl;
    return 0;
}