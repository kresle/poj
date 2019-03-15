// 10:28 -> 11:01 (一发AC！！)

#include <iostream>
#include <vector>
#include <queue>
using namespace std;

#define INF 0x3f3f3f3f

int nn, np, nc, ne;
vector<vector<int>> graph, rGraph;
int sid, tid;

vector<int> pred;

bool bfs()
{
    fill(pred.begin(), pred.end(), -1);
    queue<int> qq;
    qq.push(sid);
    pred[sid] = sid;
    while(!qq.empty())
    {
        if (qq.front()==tid)
            return true;
        for(int ii=0;ii<=tid;ii++)
        {
            if (pred[ii]==-1 && rGraph[qq.front()][ii])
            {
                pred[ii] = qq.front();
                qq.push(ii);
            }
        }
        qq.pop();
    }
    return false;
}

int maxFlow()
{
    pred.resize(nn + 2);
    rGraph = graph;
    int ans = 0;
    int uu, vv, flow;
    while (bfs())
    {
        flow = INF;
        for (vv = tid; vv != sid; vv = pred[vv])
        {
            uu = pred[vv];
            flow = min(flow, rGraph[uu][vv]);
        }
        ans += flow;
        for (vv = tid; vv != sid; vv = pred[vv])
        {
            uu = pred[vv];
            rGraph[uu][vv] -= flow;
            rGraph[vv][uu] += flow;
        }
    }
    return ans;
}

bool verbose = false;
int main()
{
    char cc;
    while (cin >> nn >> np >> nc >> ne)
    {
        graph.clear(), graph.resize(nn + 2, vector<int>(nn + 2, 0));
        sid = 0, tid = nn + 1;
        int uu, vv;
        for (int ii = 0; ii < ne; ii++)
        {
            cin >> cc >> uu >> cc >> vv >> cc; //(uu,vv)
            uu++;
            vv++;
            cin >> graph[uu][vv];
            if (verbose) cerr << uu << "," << vv << " " << graph[uu][vv] << endl;
        }
        for (int ii = 0; ii < np; ii++)
        {
            cin >> cc >> uu >> cc;
            uu++;
            cin >> graph[sid][uu];
            if (verbose) cerr << "powerStation " << uu << " : " << graph[sid][uu] << endl;
        }
        for (int ii = 0; ii < nc; ii++)
        {
            cin >> cc >> uu >> cc;
            uu++;
            cin >> graph[uu][tid];
            if (verbose) cerr << "consumer " << uu << " : " << graph[uu][tid] << endl;
        }
        cout << maxFlow() << endl;
    }
}