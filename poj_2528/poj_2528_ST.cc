#include <iostream>
#include <stdio.h>
#include <set>
#include <queue>
using namespace std;

#define INF 0x3f3f3f3f
#define MAXLEN 10000000

bool verbose = false;

struct node
{
    int id;
    int lb, rb;
    node *left, *right;
    node(int ll, int rr, int idd) : id(idd), lb(ll), rb(rr), left(NULL), right(NULL) {}
};

void insertNode(node *root, int ll, int rr, int id)
{
    //必须保证[ll,rr]在此结点范围内

    //覆盖
    if (root->lb == ll && root->rb == rr)
    {
        root->id = id;
        if (root->left)
        {
            delete root->left;
            root->left = NULL;
        }
        if (root->right)
        {
            delete root->right;
            root->right = NULL;
        }
        return;
    }

    //包含
    //如果root是叶结点：
    if (root->left == NULL && root->right == NULL)
    {
        //把此结点拆成两份，一份继承此结点的id，别外一份再递归添加ll，rr
        if (root->lb < ll)
        {
            root->left = new node(root->lb, ll - 1, root->id);
            root->right = new node(ll, root->rb, root->id);
            insertNode(root->right, ll, rr, id);
        }
        else
        { //lb == ll
            root->left = new node(ll, rr, id);
            root->right = new node(rr + 1, root->rb, root->id);
        }
        return;
    }

    //不是叶结点，则必有两个子结点

    //在其中一个子结点上，而没有两个都有压住
    if (root->left && rr <= root->left->rb)
    {
        insertNode(root->left, ll, rr, id);
    }
    else if (root->right && ll >= root->right->lb)
    {
        insertNode(root->right, ll, rr, id);
    }
    //要分开
    else
    {
        insertNode(root->left, ll, root->left->rb, id);
        insertNode(root->right, root->right->lb, rr, id);
    }
}

int bfs(node *root)
{
    set<int> vp;
    queue<node *> qq, lvl_qq;
    qq.push(root);
    node *cur;
    while (!qq.empty())
    {
        cur = qq.front();
        if (cur->id && !cur->left && !cur->right)
            vp.insert(cur->id);
        if (cur->left)
            qq.push(cur->left);
        if (cur->right)
            qq.push(cur->right);
        qq.pop();
        delete cur;
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
        cin >> nn;
        int lb, rb;
        node *root = new node(1, MAXLEN, 0);
        for (int ii = 1; ii <= nn; ii++)
        {
            cin >> lb >> rb;
            insertNode(root, lb, rb, ii);
        }
        cout << bfs(root) << endl;
    }
}

//没有delete，没有最后的bfs,用时1.61s
//没有delete，有最后的bfs,用时1.65s
//时间瓶颈就是在建树上