/**
 * 以方块为列，边为行。
 * 要覆盖的对象为列。在此，就是方块。
 * 主要是，是不是这道问题的最优解，以及是不是能找到解。
 */

#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <cmath> 

using namespace std;

const int maxn = 65*6*6*6;
const int oo = 1 << 30;
const int maxrow = 65;
const int maxcol = 6*6*6;
bool mtx[maxrow][maxcol];
int t, n, k, ans;
int has[65];

int totRow, totCol, head, idx;
int L[maxn], R[maxn], U[maxn], D[maxn];
int RH[maxn], CH[maxn], S[maxn];

bool ok(int x1, int y1, int x2, int y2)
{
    bool flag = true;
    int id; 
    for (int i = y1; i <= y2; ++i) {
        id = (x1 - 1) * (2 * n + 1) + i;
        if (!has[id]) {
            flag = false;
            break;
        }
        id = (x2) * (2 * n + 1) + i;
        if (!has[id]) {
            flag = false;
            break;
        }
    }
    if (!flag) return false;
    for (int i = x1; i <= x2; ++i) {
        id = (i - 1)*(2*n+1) + n + y1;
        if (!has[id]) {
            flag = false;
            break;  
        }
        id = (i - 1)*(2*n+1) + n + y2 + 1;
        if (!has[id]) {
            flag = false;
            break;
        }
    }
    if (!flag) return false;
    return true;
}

void add(int x1, int y1, int x2, int y2, int c)
{
    int id;
    for (int i = y1; i <= y2; ++i) {
        id = (x1 - 1) * (2 * n + 1) + i;
        mtx[id-1][c] = 1;
        id = (x2) * (2 * n + 1) + i;
        mtx[id-1][c] = 1;
    }
    for (int i = x1; i <= x2; ++i) {
        id = (i - 1)*(2*n+1) + n + y1;
        mtx[id-1][c] = 1;
        id = (i - 1)*(2*n+1) + n + y2 + 1;
        mtx[id-1][c] = 1;
    }
}

void initMtx()
{
    int cnt = 0;
    memset(mtx, 0, sizeof(mtx));
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n; ++j) {
            for (int k = 0; i + k <= n && j + k <= n; ++k) {
                if (ok(i, j, i + k, j + k)) {
                    add(i, j, i + k, j + k, cnt);
                    cnt++;
                }
            }
        }
    }
    totRow = 2 * n * (n + 1);
    totCol = cnt;
}

int newNode(int up, int down, int left, int right)
{
    U[idx] = up;    D[idx] = down;
    L[idx] = left;  R[idx] = right;
    U[down] = D[up] = L[right] = R[left] = idx;
    return idx++;
}

void build()
{
    idx = maxn - 1;
    head = newNode(idx, idx, idx, idx);
    idx = 0;
    for (int j = 0; j < totCol; ++j) {
        newNode(idx, idx, L[head], head);
        CH[j] = j;  S[j] = 0;
    }
    for (int i = 0; i < totRow; ++i) {
        int k = -1;
        for (int j = 0; j < totCol; ++j) {
            if (!mtx[i][j]) continue;
            if (-1 == k) {
                k = newNode(U[CH[j]], CH[j], idx, idx);
                RH[k] = i;  CH[k] = j;  S[j]++;
            } else {
                k = newNode(U[CH[j]], CH[j], k, R[k]);
                RH[k] = i;  CH[k] = j;  S[j]++;
            }
        }
    }
}

void remove(int c)
{
    for (int i = D[c]; i != c; i = D[i]) {
        L[R[i]] = L[i]; R[L[i]] = R[i]; /*S[CH[i]]--;*/
    }
}

void resume(int c)
{
    for (int i = U[c]; i != c; i = U[i]) {
        L[R[i]] = R[L[i]] = i; /*S[CH[i]]++;*/ 
    }
}

/*估价函数*/
int h()
{
    bool vis[maxcol];
    memset(vis, false, sizeof(vis));
    int ret = 0;
    for (int i = R[head]; i != head; i = R[i]) {
        if (!vis[i]) {
            ret++;
            vis[i] = true;
            for (int j = D[i]; j != i; j = D[j]) {
                for (int k = R[j]; k != j; k = R[k]) {
                    vis[CH[k]] = true;
                }
            }
        }
    }
    return ret;
}

void dance(int cnt)
{
    if (cnt + h() >= ans) {
        return ;
    }
    if (R[head] == head) {
        ans = cnt;
        return ;
    }
    int c, Min = oo;
    for (int i = R[head]; i != head; i = R[i]) {
        if (S[i] < Min) {
            Min = S[i]; c = i;
        }
    }
    for (int i = D[c]; i != c; i = D[i]) {
        remove(i);
        for (int j = R[i]; j != i; j = R[j]) {
            remove(j);
        } 
        dance(cnt + 1);
        
        for (int j = L[i]; j != i; j = L[j]) {
            resume(j);
        }
        resume(i);
    }
    return ; 
}

int main()
{
    scanf("%d", &t);
    while (t--) {
        scanf("%d", &n);
        scanf("%d", &k);
        memset(has, true, sizeof(has));
        int num;
        for (int i = 0; i < k; ++i) {
            scanf("%d", &num);
            has[num] = false;
        }
        initMtx();
        build();
        ans = oo;
        dance(0);
        printf("%d\n", ans);
    }
    return 0;
}