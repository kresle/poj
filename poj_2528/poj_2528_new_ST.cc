#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <set>
#include <cstring>
#include <queue>
using namespace std;

#define INF 0x3f3f3f3f3f

#define MAXNP 10000

int nn;
int nb, nl, nst;// 出现的端点个数，满树情况下最后一层的个数，线段树的结点个数
int bounds[MAXNP * 2];
int posters[MAXNP][2];
// st是线段树对应结点内的海报信息。
// 为0表示子树不统一，需要进一步考察子树情况。
// 不为0表示以此结点为根的子树都是同一海报
// 大小是MAXNP*8的原因：所有出现的端点做最后一层的叶结点，而满树的最后一层的最多叶结点可能是端点个数的两倍，从而树的总结点数是端点个数的四倍
int st[MAXNP * 8]; 
bool visible[MAXNP+1]; //对应编号的海报是否可见。

inline int mid(int st, int ed) { return st + (ed - st) / 2; }

// 对下面updateInfo的改进，这个是参考了csdn上代码的。
void update(int cur, int sl, int sr, int ql, int qr, int pid)
{
    //如果两区间不相交
    if (cur >= nst || sr<ql || sl>qr)
        return;
    //结点区间被更新区间完全覆盖
    if (sl>=ql && sr<=qr)
    {
        st[cur] = pid;
        return;
    }
    //结点区间不能被更新区间覆盖
    //下压
    if (st[cur] != 0)
    {
        st[cur * 2 + 1] = st[cur];
        st[cur * 2 + 2] = st[cur];
        st[cur] = 0;
    }
    int sm = mid(sl, sr);
    if (sm>=ql) update(cur*2+1, sl, sm, ql, qr, pid);
    if (sm<qr) update(cur*2+2, sm+1, sr, ql, qr, pid);
}


// cur: 当前结点
// [ss,se]：当前结点负责的区间
// [qs,qe]：要更新的区间
// 将区间更新成pid海报
void updateInfo(int cur, int ss, int se, int qs, int qe, int pid)
{
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
    //父结点的值下压到子结点
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
    // set<int> vp;//集合的速度还是太慢
    queue<int> qq, lvl_qq;
    if (st[0] != 0) 
        // vp.insert(st[0]);
        visible[st[0]] = true;
    else
        qq.push(0);
    while (!qq.empty() && qq.front() < nst)
    {
        while (!qq.empty() && qq.front() < nst)
        {
            if (st[qq.front()] != 0)
                // vp.insert(st[qq.front()]);
                visible[st[qq.front()]] = true;
            else
            {
                lvl_qq.push(qq.front() * 2 + 1);
                lvl_qq.push(qq.front() * 2 + 2);
            }
            qq.pop();
        }
        swap(qq, lvl_qq);
    }
    int cnt =0;
    for(int ii=1;ii<=nn;ii++) cnt+=visible[ii];
    return cnt;
    // return vp.size();
}

int main()
{
    ios_base::sync_with_stdio(0), cin.tie(0);
    int kk;
    cin >> kk;
    // scanf("%d", &kk);
    while (kk--)
    {
        memset(st, 0, sizeof(st));
        memset(visible, 0, sizeof(visible));
        cin >> nn;
        // scanf("%d", &nn);
        int lb, rb;
        int cnt = 0;
        for (int ii = 0; ii < nn; ii++)
        {
            cin >> lb >> rb;
            // scanf("%d%d", &lb, &rb);
            posters[ii][0] = lb;
            posters[ii][1] = rb;
            bounds[cnt++] = lb;
            bounds[cnt++] = rb;
        }
        sort(bounds, bounds + cnt);
        nb = unique(bounds, bounds + 2 * nn) - bounds;
        int tmp = nb;
        nl = 1;
        while (tmp)
        {
            nl = nl << 1;
            tmp = tmp >> 1;
        }
        nst = nl - 1 + nb; //总共有多少结点
        for (int ii = 0; ii < nn; ii++)
        {
            int st = lower_bound(bounds, bounds + nb, posters[ii][0]) - bounds;
            int ed = lower_bound(bounds, bounds + nb, posters[ii][1]) - bounds;
            // updateInfo(0, 0, nl - 1, st, ed, ii + 1);
            update(0, 0, nl - 1, st, ed, ii + 1);
        }
        cout << bfs() << endl;
        // printf("%d\n", bfs());
    }
}