#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <vector>
#include <set>
#include <cstring>
#include <queue>
#include <math.h>
using namespace std;

#define INF 0x3f3f3f3f3f

#define MAXNP 10000

// #define VERBOSE

int nb, nl, nst;
int bounds[MAXNP * 2];
int posters[MAXNP][2];
int st[MAXNP * 8];//这个是因为，可能的结点数做最后一层的叶结点，而满树的最后一层的最多叶结点可能是bounds大小的两倍，从而树的总结点数是bound大小的四倍

inline int mid(int st, int ed) { return st + (ed - st) / 2; }

void updateInfo(int cur, int ss, int se, int qs, int qe, int pid)
{
#ifdef VERBOSE
    printf("updateInfo, nodeId = %d with interval [%d, %d], poster part [%d, %d] = %c\n", cur, ss, se, qs, qe, 'A' + pid - 1);
#endif
    //递归 呈 长度不断缩小之势，当长度小于0时就可以终止
    if (cur >= nst || ss > se || qs > qe)
        return;
    //不相交
    if (qe < ss || qs > se)
        return;
    //相等
    if (ss == qs && se == qe)
    {
        st[cur] = pid;
        return;
    }
    //包含
    //值下压
    if (st[cur] != 0)
    {
        st[cur * 2 + 1] = st[cur];
        st[cur * 2 + 2] = st[cur];
        st[cur] = 0;
    }
    int sm = mid(ss, se);

    if (qs >= ss && qs <= sm)
        updateInfo(cur * 2 + 1, ss, sm, qs, qe >= sm ? sm : qe, pid);
    if (qe >= sm + 1 && qe <= se)
        updateInfo(cur * 2 + 2, sm + 1, se, qs < sm + 1 ? sm + 1 : qs, qe, pid);
}

int bfs()
{
    set<int> vp;
    queue<int> qq, lvl_qq;
    qq.push(0);
    while (!qq.empty() && qq.front() < nst)
    {
        while (!qq.empty() && qq.front() < nst)
        {
#ifdef VERBOSE
            cerr << st[qq.front()] << " ";
#endif
            if (st[qq.front()] != 0)
            {
                vp.insert(st[qq.front()]);
            }
            else
            {
                lvl_qq.push(qq.front() * 2 + 1);
                lvl_qq.push(qq.front() * 2 + 2);
            }
            qq.pop();
        }
#ifdef VERBOSE
        cerr << endl;
#endif
        swap(qq, lvl_qq);
    }
    return vp.size();
}

int main()
{
    ios_base::sync_with_stdio(0), cin.tie(0);
    int kk, nn;
    cin >> kk;
    while (kk--)
    {
        memset(st, 0, sizeof(st));
        cin >> nn;
        int lb, rb;
        int cnt = 0;
        for (int ii = 0; ii < nn; ii++)
        {
            cin >> lb >> rb;
            posters[ii][0] = lb;
            posters[ii][1] = rb;
            bounds[cnt++] = lb;
            bounds[cnt++] = rb;
        }
        sort(bounds, bounds + cnt);
        nb = unique(bounds, bounds + 2 * nn) - bounds;
        int tmp = nb;
        nl = 1;
        while(tmp)
        {
            nl = nl << 1;
            tmp = tmp >> 1;
        }
        // nl = pow(2, ceil(log2(nb))); //最后一层的结点数
        nst = nl - 1 + nb;   //总共有多少结点
#ifdef VERBOSE
        for (int ii = 0; ii < nb; ii++)
            cerr << bounds[ii] << " ";
        cerr << endl;
#endif
        for (int ii = 0; ii < nn; ii++)
        {
            int st = lower_bound(bounds, bounds + nb, posters[ii][0]) - bounds;
            int ed = lower_bound(bounds, bounds + nb, posters[ii][1]) - bounds;
            updateInfo(0, 0, nl - 1, st, ed, ii + 1);
#ifdef VERBOSE
            bfs();
            cerr << string(100, '-') << endl;
#endif
        }
        cout << bfs() << endl;
    }
}