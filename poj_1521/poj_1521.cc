/** 第一次提交，WA，下了官方的样例发现是因为没考虑只有一个char的情形，也就是root为leaf的情况.
 *  而自己其实是默认，如果定义根结点的深度为0,子结点依次加1,那么叶结点也就是字符对应的结点的深度，就是其encoding的长度。
 *  这个法则在面对只有一个结点也就是其深度为0的情形时，就会出错。
 * 另外下面的代码，是有找结点的encoding的，如果依题意，其实只要知道结点的深度就可以，在生成树的时候就可以对此进行设置（似乎不行，在建完以前，不能确定当前结点的深度）。
 * 
 * 思路：
 * 建以频率为依据的小根堆，依次取堆顶两个，合并建新结点，再压入堆。
 * 原因是，取的时机越晚，离huffman tree的根越近，所需要的位数也就越少。
 * 所以小根堆先取的都是最少出现的字符，先处理他们，他们最后的位数也就最多。
 * 另外，默认左边放频率大的，右边放频率小的。顺序反了也没关系，但整棵树的顺序要统一。
 */

#include <iostream>
#include <iomanip>
#include <queue>
#include <vector>
#include <string>
#include <cstring>
#include <map>
using namespace std;

struct huffNode
{
    char data;
    int app;
    vector<bool> encoding;
    huffNode *left;
    huffNode *right;
    huffNode(char iv) : data(iv), app(1), left(NULL), right(NULL) {}
    huffNode(huffNode *ll, huffNode *rr) : data('-'), app(ll->app + rr->app), left(ll), right(rr) {}
};

struct huffNodeCmp
{
    bool operator()(huffNode *ll, huffNode *rr)
    {
        return ll->app > rr->app;
    }
};

huffNode *id2nodes[256];
huffNode *root;
string ss;

void deleteHuffTree(huffNode *root)
{
    if (!root)
        return;
    deleteHuffTree(root->left);
    deleteHuffTree(root->right);
    delete root;
}

vector<bool> path;
void dfs(huffNode *cur)
{
    if (!cur->left && !cur->right)
    {
        cur->encoding = path;
        return;
    }
    else
    {
        if (cur->left)
        {
            path.push_back(false);
            dfs(cur->left);
            path.pop_back();
        }
        if (cur->right)
        {
            path.push_back(true);
            dfs(cur->right);
            path.pop_back();
        }
    }
}

string encodingRpr(vector<bool> enc)
{
    string ans;
    for (int ii = 0; ii < enc.size(); ii++)
        ans.push_back(enc[ii] ? '1' : '0');
    return ans;
}

int main()
{
    cout << fixed << showpoint << setprecision(1);
    while (cin >> ss)
    {
        memset(id2nodes, 0, sizeof(id2nodes));
        if (ss == "END")
            break;
        // cerr << ss << endl;
        for (int ii = 0; ii < ss.size(); ii++)
            if (!id2nodes[ss[ii]])
                id2nodes[ss[ii]] = new huffNode(ss[ii]);
            else
                id2nodes[ss[ii]]->app++;
        priority_queue<huffNode *, vector<huffNode *>, huffNodeCmp> qq;
        for (char cc = 'A'; cc <= 'Z'; cc++)
            if (id2nodes[cc])
                qq.push(id2nodes[cc]);
        if (id2nodes['_'])
            qq.push(id2nodes['_']);
        while (!qq.empty())
        {
            huffNode *n1, *n2, *tmp;
            n1 = qq.top(), qq.pop();
            if (qq.empty())
            {
                root = n1;
                break;
            }
            n2 = qq.top(), qq.pop();
            // cerr << n1->data << " & " << n2->data << endl;
            tmp = new huffNode(n1, n2);
            qq.push(tmp);
        }
        path.clear();
        dfs(root);
        if (root->data != '-')
            root->encoding.push_back(false);//处理整个语句就出现一个字符的情形。
        int huffLen = 0;
        for (int ii = 0; ii < ss.size(); ii++)
        {
            huffLen += id2nodes[ss[ii]]->encoding.size();
            // cerr << encodingRpr(id2nodes[ss[ii]]->encoding) << " ";
        }
        // cerr << endl;
        cout << ss.size() * 8 << " " << huffLen << " " << ss.size() * 8 / (float)huffLen << endl;
        deleteHuffTree(root);
    }
}