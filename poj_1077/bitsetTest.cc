#include <iostream>
#include <bitset>
using namespace std;

bitset<362880> vis;

int main()
{
    vis[9] = true;
    ulong tmp = vis.to_ulong();
    cerr << vis.size() << endl;
    cerr << tmp << endl;
}