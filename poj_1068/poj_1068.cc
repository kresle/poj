// 这题比较简单，就是在考阅读理解，题意理解以后，非常容易。

#include <string>
#include <iostream>
#include <vector>
using namespace std;

int kk, nn;
vector<int> ps, Ws;
vector<bool> used;
string ss;

int main()
{
    cin >> kk;
    while(kk--)
    {
        cin >> nn;

        ps.clear(), ps.resize(nn+1, 0);
        for(int ii=1;ii<=nn;ii++)
            cin >> ps[ii];
        
        ss.clear();
        for(int ii=1;ii<=nn;ii++)
            ss += string(ps[ii]-ps[ii-1], '(') + ")";
        // cout << ss << endl;
        
        Ws.clear();
        used.clear(), used.resize(ss.size(), false);
        for(int ii=1;ii<ss.size();ii++)
        {
            if (ss[ii]==')')
            {
                int jj=ii-1;
                int ww = 1;
                while(ss[jj]!='('||used[jj])
                {
                    jj--;
                    if (used[jj]) ww++;
                }
                Ws.push_back(ww);
                used[jj] = true;
            }
        }
        for(int ii=0;ii<Ws.size();ii++)
            cout << Ws[ii] << " ";
        cout << endl;
    }
}