#include <iostream>
#include <vector>
#include <queue>
#include <cstring>
using namespace std;

int size_st, height_st;

inline int mid(int a, int b) { return a + (b - a) / 2; }

int buildST_until(int arr[], int ss, int se, int *st, int si)
{
    if (ss == se)
        st[si] = arr[ss];
    else
        st[si] = buildST_until(arr, ss, mid(ss, se), st, 2 * si + 1) +
                 buildST_until(arr, mid(ss, se) + 1, se, st, 2 * si + 2);
    return st[si];
}

int *buildST(int arr[], int n)
{
    int tmpn = n;
    height_st = 0;
    while (tmpn)
    {
        height_st++;
        tmpn = tmpn >> 1;
    }
    height_st++;
    size_st = (1 << height_st) - 1;
    cerr << size_st << endl;
    int *st = new int[size_st];
    memset(st, 0, size_st*sizeof(int));
    st[0] = buildST_until(arr, 0, n - 1, st, 0);
    return st;
}

int main()
{
    int arr[] = {1, 3, 5, 7, 9, 11};
    int *st = buildST(arr, sizeof(arr) / sizeof(arr[0]));
    for(int ii=0;ii<size_st;ii++)
        cerr << st[ii] << " ";
    cerr << endl;
    queue<int> qq, lvl_qq;
    qq.push(0);
    while(qq.front()<size_st)
    {
        while(!qq.empty() && qq.front()<size_st)
        {
            cerr << st[qq.front()] << " ";
            lvl_qq.push(qq.front()*2+1);
            lvl_qq.push(qq.front()*2+2);
            qq.pop();
        }
        cerr << endl;
        swap(qq, lvl_qq);
    }
}