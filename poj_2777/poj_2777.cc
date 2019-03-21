/**
 * 虽然过了，但是时间卡得很紧（970ms），但自己怎么看，都觉得没什么优化空间了。
 * 写的过程中，大的方向上没有出问题，但总在一些莫名其妙的小地方卡住。
 * 这题是，刚开始树的类型就选的char,然后不对。
 * 换成int后，没问题。
 * 又换回来，还是不对。也不知道怎么查到，是读入的时候不对，color类型就是char的话，
 * 是不用cin直接读进来，比如22, 只能读进一个2来，还会用‘2’的ascii码值来作为颜色。
 * 如果是scanf的话，应该不会有问题。更新：试过了，也不行。
 * 
 * 最深层的原因还是基础功不扎实，不了解类型转换是怎么做的。
 */ 

#include <iostream>
#include <stdio.h>
#include <cstring>
using namespace std;

const char INF = -1;
int hh;
int nl;
int nst;
int ll, tt, oo;
const int maxL = 100000;
const int maxT = 30;

char *tree;
bool colorApp[maxT+1];

void initParam()
{
    int hh = 0;
    int tmp = ll;
    while(tmp)
    {
        tmp = tmp >> 1;
        hh++;
    }
    nl = 1<<hh;
    if (nl==ll*2){
        hh--;
        nl = ll;
    }
    nst = nl-1+ll;
    tree = new char[nst];
    memset(tree, 1, nst);
}

void update(int cur, int sl, int sr, int ql, int qr, char color)
{
    if (cur>=nst || sr<ql || sl>qr) return;
    if (sl>=ql && sr<=qr)
    {
        tree[cur] = color;
        return;
    }
    if (tree[cur]!=INF)
    {
        tree[cur*2+1] = tree[cur];
        tree[cur*2+2] = tree[cur];
        tree[cur] = INF;
    }
    int sm = (sl+sr)/2;
    if (sm>=ql) update(cur*2+1, sl, sm, ql, qr, color);
    if (sm<qr) update(cur*2+2, sm+1, sr, ql, qr, color);
}

void query(int cur, int sl, int sr, int ql, int qr)
{
    if (cur>=nst || sr<ql || sl>qr) return;//一开始忘了后面两个条件。但对于poj的样例，区别不大
    if (tree[cur]!=INF) 
    {
        colorApp[tree[cur]] = true;
    }
    else
    {
        int sm = (sl+sr)/2;
        if (sm>=ql) query(cur*2+1, sl, sm, ql, qr);
        if (sm<qr) query(cur*2+2, sm+1, sr, ql, qr);
    }
}

int main()
{
    ios_base::sync_with_stdio(0), cin.tie(0);
    cin >> ll >> tt >> oo;
    initParam();
    char cmd;
    int aa, bb, cc;
    // char cc;
    while(oo--)
    {
        cin >> cmd;
        if (cmd=='C')
        {
            cin >> aa >> bb >> cc;//这里如果cc类型是char,那就会把输入的‘2’当成是‘2’的ascii码
            // scanf("%d%d%d", &aa, &bb, &cc);//cc是char型时也不行
            update(0, 1, nl, aa, bb, cc);
        }
        else
        {
            memset(colorApp, 0, maxT+1);
            cin >> aa >> bb;
            query(0, 1, nl, aa, bb);
            int ans = 0;
            for(int ii=0;ii<=tt;ii++)
                ans+=colorApp[ii];
            cout << ans << endl;
        }
    }
}