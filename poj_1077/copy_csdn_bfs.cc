/**
 * Name: Eight 
 * P_ID: POJ 1077
 * Note: BFS + 康托展开
 * Date: 2016-05-18
 *
 */
#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <vector>
#include <stack>
#include <queue>
#include <set>
#include <map>
#include <string>
#include <cmath>
#include <cstdlib>
#include <ctime>
using namespace std;
typedef long long ll;
const int MOD = 1e9 + 7;
const int MAXN = 1e5 + 3;
const int cell = 362880;


int vis[cell];
int parent[cell];
char step[cell];

//cantor Base
const int fac[] = {1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880};
//dir
const int dir[4][2] = {{-1, 0}, {1,0}, {0, -1}, {0, 1}}; //u, d, l, r

struct node {
    char s[9];
    int space;
};

//康拓定理正向加密过程
int Hash(const char* str)
{
    int n = 9;
    int num = 0;
    int temp;
    for(int i=0; i<n-1; ++i)
    {
        temp=0;
        for(int j=i+1; j<n; j++)
        {
            if(str[j] < str[i])
                temp++;
        }
        num += fac[str[i]-1] * temp;
    }
    return num;
}

//康托定理逆向解码过程
void get_node(int num, node &temp)
{
    int n = 9;
    int a[9];
    for(int i=2; i<=n; ++i)
    {
        a[i-1] = num%i;
        num /= i;
        temp.s[i-1] = 0;
    }

    temp.s[0] = 0;
    int rn, i;
    for(int k=n; k>=2; --k)
    {
        rn = 0;
        for(i=n-1; i>=0; --i)
        {
            if(temp.s[i]!=0)
                continue;
            if(rn==a[k-1])
                break;
            ++rn;
        }
        temp.s[i] = k;
    }

    for(i=0; i<n; ++i)
    {
        if(temp.s[i]==0)
        {
            temp.s[i] = 1;
            break;
        }
    }
    temp.space = n - a[n-1] - 1;
}

//搜索
void bfs(const node& begin)
{
    memset(vis, 0, sizeof(vis));
    int u = Hash(begin.s);
    vis[u] = 1;
    parent[u] = -1;//便于后续输出路径，保存了每一步的父节点

    queue<int> myQue;
    myQue.push(u);

    node now, next;
    while(!myQue.empty())
    {
        u = myQue.front();
        myQue.pop();

        get_node(u, now);

        int k = now.space;
        int x = k/3;
        int y = k%3;

        for(int i=0; i<4; ++i)
        {
            int xx = x + dir[i][0];
            int yy = y + dir[i][1];
            if(xx>=0 && xx<=2 && yy>=0 && yy<=2)
            {
                next = now;
                next.space = xx * 3 + yy;
                swap(next.s[k], next.s[next.space]);//移动
                int v = Hash(next.s);
                if(!vis[v])
                {
                    step[v] = i;
                    vis[v] = 1;
                    parent[v] = u;
                    if(v==0)
                        return;//终止，到达了目标状态
                    myQue.push(v);
                }
            }
        }
    }
}

void print()
{
    int n, u;
    char path[1000];
    n = 1;
    path[0] = step[0];
    u = parent[0];
    while(parent[u]!=-1)
    {
        path[n] = step[u];
        ++n;
        u = parent[u];
    }

    for(int i=n-1; i>=0; --i)
    {
        if(path[i]==0)
            cout << "u";
        else if(path[i]==1)
            cout << "d";
        else if(path[i]==2)
            cout << "l";
        else
            cout << "r";
    }
}

int main()
{
    node start;
    char c;
    for(int i=0; i<9; ++i)
    {
        cin >> c;
        if(c=='x')
        {
            start.s[i] = 9;
            start.space = i;
        }
        else 
            start.s[i] = c - '0';
    }

    bfs(start);

    if(vis[0]==1)
        print();
    else 
        cout << "unsolvable";

    cout << endl;
    return 0;
}