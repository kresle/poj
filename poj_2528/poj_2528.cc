#include <iostream>
#include <vector>
#include <map>
#include <queue>
using namespace std;

#define INF 0x3f3f3f3f

#define MAXLEN 10000000

struct node
{
    bool valid;
    int id;
    pair<int, int> val;
    vector<node*> children;
    node(int il, int ir, int iid)
    {
        val = make_pair(il, ir);
        valid = true;
        id = iid;
    }
};

node* root;

void deleteTree(node* root)
{
    for(int ii=0;ii<root->children.size();ii++)
        deleteTree(root->children[ii]);
    delete root;
}

node* findParent(node* root, int il, int ir)
{
    node* ans = root;
    for(int ii=0;ii<root->children.size();ii++)
    {
        pair<int, int> pp = root->children[ii]->val;
        if ((pp.first<il && pp.second>=ir)||(pp.first<=il && pp.second>ir))
        {
            ans = findParent(root->children[ii], il, ir);
            break;
        }
    }
    return ans;
}

// void insertNode(node* root, int il, int ir)
// {

// }

int countNodeNo(node* root)
{
    int ans = 1;    //自身
    for(int ii=0;ii<root->children.size();ii++)
    {
        ans += countNodeNo(root->children[ii]);
    }
    return ans;
}

void bfs()
{
    queue<node*> qq, lvl_qq;
    qq.push(root);
    while(!qq.empty())
    {
        while(!qq.empty())
        {
            cerr << qq.front()->val.first << "," << qq.front()->val.second << " ";
            for(int ii=0;ii<qq.front()->children.size();ii++)
                lvl_qq.push(qq.front()->children[ii]);
            qq.pop();
        }
        cerr << endl;
        swap(qq, lvl_qq);
    }
}

int main()
{
    ios_base::sync_with_stdio(0), cin.tie(0);
    int kk, nn;
    cin >> kk;
    while(kk--)
    {
        cin >> nn;
        root = new node(1, MAXLEN, nn);
        int a2, b2, ansCnt;
        vector<int> pieceCnt(nn+1,0);
        while(nn--)
        {
            cin >> a2 >> b2;
            // cerr << a2 << "," << b2 << endl;
            node* parent = findParent(root, a2, b2);
            for(int jj=0;jj<parent->children.size();jj++)
            {
                if (!parent->children[jj]->valid) continue;
                // pair<int, int> &pp = parent->children[jj]->val;
                int &a1 = parent->children[jj]->val.first, &b1 = parent->children[jj]->val.second;
                // cerr << "\t" << a1 << "," << b1 << endl;
                if (b1<a2 || a1 >b2) continue;
                if (a1<a2 && a2<=b1 && b1<b2)
                {
                    // cerr << "modifing intersected seg" << endl;
                    b1 = a2-1;
                }
                else if (a1>a2 && a1<=b2 && b1>b2)
                    a1 = b2+1;
            }
            bool inserted = false;
            for(int jj=0;jj<parent->children.size();jj++)
            {
                if (!parent->children[jj]->valid) continue;
                pair<int, int> pp = parent->children[jj]->val;
                if (a2<=pp.first && b2>=pp.second)
                {
                    if (!inserted)
                    {
                        pp.first = a2;
                        pp.second = b2;
                        inserted = true;
                        parent->children[jj]->id = nn;
                        pieceCnt[nn]++;//只执行一次
                    }
                    else
                    {
                        parent->children[jj]->valid = false;
                    }
                    pieceCnt[parent->children[jj]->id]--;//所有被覆盖的有效结点都
                }
            }
            if (!inserted)
            {
                node* tmp = new node(a2, b2, nn);
                pieceCnt[nn]++;
                node *ll=NULL, *rr=NULL;
                if (parent->children.empty())
                {
                    if (a2>parent->val.first)
                        ll = new node(parent->val.first, a2-1, parent->id);
                    if (b2<parent->val.second)
                        rr = new node(b2+1, parent->val.second, parent->id);
                    if (ll&&rr) pieceCnt[parent->id]++;
                    // if (!ll&!rr) pieceCnt[parent->id]--;//覆盖
                }
                if (ll) parent->children.push_back(ll);
                parent->children.push_back(tmp);
                if (rr) parent->children.push_back(rr);
            }
        }
        bfs();
        ansCnt = 0;
        for(int ii=0;ii<pieceCnt.size()-1;ii++)
            if (pieceCnt[ii]>0) ansCnt++;
        cout << ansCnt << endl;
        delete(root);
    }
}