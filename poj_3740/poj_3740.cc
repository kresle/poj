#include <iostream>
#include <vector>

using namespace std;

struct node
{
    node* left;
    node* right;
    node* above;
    node* below;
    node* header;
    int cid;
    int rid;
    int count;
};

void cover()
{

}

void uncover()
{

}

node *header;
void search(int k)
{
    node *rowNode, *rightNode, *leftNode, *column;
    if (header->right == header )
}