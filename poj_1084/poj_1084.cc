/**
 * 迭代加深搜索。
 * 学到的一个非常重要的小技巧是用bitset来做集合的交、并等操作，太方便了。
 * 在可以用_Find_first()跟_Find_next(ii)这两个函数的编译环境里，编历bitset，可能比集合还要快，反正是非常快。
 * 另外就是这道题里怎么找方形的所有边，这里细心就可以。自己对这道题望而生畏，就是因为这个觉得会很棘手。
 */

#include <iostream>
#include <bitset>
using namespace std;

const int maxNN = 5;
const int maxSquareCnt = 55;
const int maxEdgeCnt = 60;

int nn;
int squareCnt, edgeCnt;
bitset<maxEdgeCnt> lackEdge;
bitset<maxEdgeCnt> squareEdges[maxSquareCnt];

int stepLimit;
bitset<maxEdgeCnt> edgesTaken;
bool foundAns;
void dfs(int sid, int step)
{
    if (sid == squareCnt)
    {
        if (step <= stepLimit)
            foundAns = true;
        return;
    }
    if (step > stepLimit || foundAns)
        return;

    if ((squareEdges[sid] & edgesTaken).any())
        dfs(sid + 1, step);
    else
    {
        // for (int ii = 0; ii < edgeCnt; ii++)
        // poj不支持下面注释部分的接口，不然会更快
        int ii = squareEdges[sid]._Find_first();
        while (ii != maxEdgeCnt)
        {
            if (!squareEdges[sid][ii]) continue;
            edgesTaken[ii] = true;
            dfs(sid + 1, step + 1);
            edgesTaken[ii] = false;
            ii = squareEdges[sid]._Find_next(ii);
        }
    }
}

int main()
{
    int kk, mm, eid;
    cin >> kk;
    while (kk--)
    {
        cin >> nn;

        edgeCnt = 2 * nn * (nn + 1);

        lackEdge.reset();
        cin >> mm;
        for (int ii = 0; ii < mm; ii++)
        {
            cin >> eid;
            lackEdge[eid - 1] = true;
        }

        for (int ii = 0; ii < maxSquareCnt; ii++)
            squareEdges[ii].reset();
        squareCnt = 0;

        for (int ll = 1; ll <= nn; ll++) //square的长度
        {
            for (int ii = 0; ii <= nn - ll; ii++)
                for (int jj = 0; jj <= nn - ll; jj++) //(ii,jj)square的左上角坐标
                {
                    bool squareValid = true;
                    int h_st = ii * (2 * nn + 1) + jj;
                    int v_st = h_st + nn;
                    for (int ee = 0; ee < ll; ee++)
                    {
                        if (!lackEdge[h_st + ee])
                            squareEdges[squareCnt][h_st + ee] = true;
                        else
                        {
                            squareValid = false;
                            break;
                        }
                    }
                    h_st += ll * (2 * nn + 1);
                    for (int ee = 0; ee < ll && squareValid; ee++)
                    {
                        if (!lackEdge[h_st + ee])
                            squareEdges[squareCnt][h_st + ee] = true;
                        else
                        {
                            squareValid = false;
                            break;
                        }
                    }
                    for (int ee = 0; ee < ll && squareValid; ee++)
                    {
                        if (!lackEdge[v_st + ee * (2 * nn + 1)])
                            squareEdges[squareCnt][v_st + ee * (2 * nn + 1)] = true;
                        else
                        {
                            squareValid = false;
                            break;
                        }
                    }
                    v_st += ll;
                    for (int ee = 0; ee < ll && squareValid; ee++)
                    {
                        if (!lackEdge[v_st + ee * (2 * nn + 1)])
                            squareEdges[squareCnt][v_st + ee * (2 * nn + 1)] = true;
                        else
                        {
                            squareValid = false;
                            break;
                        }
                    }
                    if (squareValid)
                        squareCnt++;
                    else
                        squareEdges[squareCnt].reset();
                }
        }

        stepLimit = -1;
        foundAns = false;
        edgesTaken.reset();
        while (!foundAns)
        {
            stepLimit++;
            dfs(0, 0);
        }
        cout << stepLimit << endl;
    }
}