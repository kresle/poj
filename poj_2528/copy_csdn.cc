#include<cstdio>
#include<iostream>
#include<cmath>
#include<cstring>
#include<cstdlib>
#include<queue>
#include<map>
#include<algorithm>
#include<set>
#include<stack>
const int MAXN=100005;//线段数
int n;
int info[MAXN*4];//线段树维护的信息
int lenth;
int bj[MAXN*2];
int num[MAXN*2];//用于保存离散化信息
int cnt;
using namespace std;
struct LINE
{
     int a;
     int b;
}line[MAXN];
void Init()
{
      cnt=0;
      lenth=0;
      memset(bj,0,sizeof(bj));
      memset(info,0,sizeof(info));
}
void Pushdown(int rt)//下推延迟信息，该节点的信息已更新
{
      if(info[rt])
      {
            info[rt*2]=info[rt];
            info[rt*2+1]=info[rt];
            info[rt]=0;
      }
}
void Update(int L,int R,int C,int l,int r,int rt)//保证(L,R)区间的数将会更新为C
{
    if(L<=l && r<=R){info[rt]=C;return ;}
    int m=(l+r)/2;
    Pushdown(rt);
    if(L<=m)Update(L,R,C,l,m,rt*2);
    if(R>m)Update(L,R,C,m+1,r,rt*2+1);
}
int Query(int x,int l,int r,int rt)//查询第x个数是什么
{
    if(l==r)return info[rt];

    int m=(l+r)/2;
    Pushdown(rt);
    if(x<=m)return Query(x,l,m,rt*2);
    else if(x>m)return Query(x,m+1,r,rt*2+1);
}
void Solve()
{
    int t;
    int ans=0;
    for(int i=1;i<=lenth;i++)
    {
          t=Query(i,1,lenth,1);
          bj[t]=1;
    }
    for(int i=1;i<=lenth;i++)ans+=bj[i];
    printf("%d\n",ans);
}
int main()
{
    int T;
    int a,b;
    scanf("%d",&T);
    while(T--)
    {
          Init();
          scanf("%d",&n);
          for(int i=1;i<=n;i++)
          {
              scanf("%d%d",&a,&b);
              num[++cnt]=a;
              num[++cnt]=b;
              line[i].a=a;line[i].b=b;
          }
          sort(num+1,num+cnt+1);
          int m=unique(num+1,num+cnt+1)-num-1;//unique返回去重后尾地址，但那个地址不含数字，m表示去重后的个数
          int t=m;
          for(int i=2;i<=t;i++)//如果两个数相差大于1则在他们之间插入一个数
          {
                if(num[i]-num[i-1]>1)
                      num[++m]=num[i-1]+1;
          }
          sort(num+1,num+m+1);
          int x,y;
          lenth=m;
          for(int i=1;i<=n;i++)
          {
               x=lower_bound(num+1,num+m+1,line[i].a)-num;//lower_bound返回第1个不小于line[i].a的值的指针
               y=lower_bound(num+1,num+m+1,line[i].b)-num;
               Update(x,y,i,1,lenth,1);
          }
          Solve();
    }
    return 0;
}