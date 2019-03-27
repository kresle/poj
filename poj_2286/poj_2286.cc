#include <iostream>
#include <stdio.h>
#include <cstring>
#include <vector>
using namespace std;

char board[24];
char lines[8][7] = {
    {0, 2, 6, 11, 15, 20, 22},
    {1, 3, 8, 12, 17, 21, 23},
    {10, 9, 8, 7, 6, 5, 4},
    {19, 18, 17, 16, 15, 14, 13},
    {23, 21, 17, 12, 8, 3, 1},
    {22, 20, 15, 11, 6, 2, 0},
    {13, 14, 15, 16, 17, 18, 19},
    {4, 5, 6, 7, 8, 9, 10}};
char center[] = {6, 7, 8, 11, 12, 15, 16, 17};

char centerMostNumCnt(char board[24])
{
    char cnt[3] = {0, 0, 0};
    for (char ii = 0; ii < 8; ii++)
        cnt[board[center[ii]]]++;
    return max(cnt[0], max(cnt[1], cnt[2]));
}

bool readBoard()
{
    for (char ii = 0; ii < 24; ii++)
    {
        cin >> board[ii];
        if (board[ii] == '0')
            return false;
        board[ii] -= '1';
    }
    return true;
}

void takeStep(char board[24], int ii)
{
    for (int jj = 0; jj < 6; jj++)
        swap(board[lines[ii][jj]], board[lines[ii][jj + 1]]);
}

int stepsLimit;
bool solved = false;
vector<char> path, ansPath;
char ansCenter;
void dfs(int stepsTaken)
{
    if (solved) return;
    int minStepNeed = 8 - centerMostNumCnt(board);
    if (stepsTaken + minStepNeed > stepsLimit)
        return; //就这步自己可能想不到。别的地方都还好。
    if (minStepNeed == 0)
    {
        ansPath = path;
        solved = true;
        ansCenter = board[center[0]];
        return;
    }

    char boardbak[24];
    memcpy(boardbak, board, 24);
    for (int ii = 0; ii < 8 && !solved; ii++)
    {
        takeStep(board, ii);
        path.push_back(ii);
        dfs(stepsTaken + 1);
        path.pop_back();
        memcpy(board, boardbak, 24);
    }
}

void printBoard(char board[24])
{
    printf("  %d %d\n", board[0], board[1]);
    printf("  %d %d\n", board[2], board[3]);
    for (char ii = 6; ii >= 0; ii--)
        printf("%d", board[lines[2][ii]]);
    printf("\n  %d %d\n", board[11], board[12]);
    for (char ii = 6; ii >= 0; ii--)
        printf("%d", board[lines[3][ii]]);
    printf("\n  %d %d\n", board[20], board[21]);
    printf("  %d %d\n", board[22], board[23]);
}

int main()
{
    while (readBoard())
    {
        stepsLimit = 0;
        solved = centerMostNumCnt(board) == 8;
        path.clear(), ansPath.clear();
        ansCenter = board[center[0]];
        while (!solved)
        {
            stepsLimit++;
            dfs(0);
        }
        if (ansPath.empty())
            printf("No moves needed");
        for (int ii = 0; ii < ansPath.size(); ii++)
            printf("%c", ansPath[ii]+'A');
        printf("\n%d\n", ansCenter + 1);
    }
}