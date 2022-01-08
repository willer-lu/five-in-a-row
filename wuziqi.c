#include <stdio.h>
#include<string.h>
#include<windows.h>
#define N 19
int p[N + 2][N + 2]; //0空1黑2白
int s = 2, ais = 1, s0=2;//s是轮到谁下,s=1,2，s=1是ai下，s=2是玩家，s=s0是黑方下，否则是白方下
int is_end = 0;//判断是否下结束
int dx[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
int dy[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };//（dx,dy）是8个方向向量
int print(int i, int j)//打印棋盘
{
    if (p[i][j] == 1)return printf("●");
    if (p[i][j] == 2)return printf("○");

    if (i == N)
    {
        if (j == 1)return printf("┏");
        if (j == N)return printf("┓");
        return printf("┯");
    }
    if (i == 1)
    {
        if (j == 1)return printf("┗");
        if (j == N)return printf("┛");
        return printf("┷");
    }
    if (j == 1)return printf("┠");
    if (j == N)return printf("┨");
    return printf("┼");
}
void DrawBoard()//打印整个游戏界面
{
    system("cls");
    system("color F0");
    int row = 0, col = 0;
    char alpha  = 'A';
    printf("      ");
    for (col = 1; col <= N; col++)printf("%c ", alpha++);
    for (row = N; row >= 1; row--)
    {
        printf("\n   %2d", row);
        for (col = 1; col <= N; col++)
        {
            print(row, col);

        }
        printf("%d", row);
    }
    alpha = 'A';
    printf("  \n      ");
    for (col = 1; col <= N; col++)printf("%c ", alpha++);
    printf("\n");
}
void init()//游戏开局初始化
{
    system("color f0");
    int i, j;
    for (i = 0; i <= N + 1; i++)for (j = 0; j <= N + 1; j++)p[i][j] = 0;//以空格包围棋盘
    DrawBoard();
}
int inboard(int row, int col)//判断(row,col)是否在棋盘内
{
    if (row <1 || row > N)return 0;
    return col >= 1 && col <= N;
}
int same(int row, int col, int key)//判断2个棋子是否同色
{
    if (!inboard(row, col))return 0;
    return (p[row][col] == key || p[row][col] + key == 0);
}
int num(int row, int col, int u)//坐标（row,col），方向向量u，返回该方向有多少连续同色棋子
{
    int i = row + dx[u], j = col + dy[u], sum = 0, ref = p[row][col];
    if (ref == 0)return 0;
    while (same(i, j, ref))sum++, i += dx[u], j += dy[u];
    return sum;
}
int live4(int row, int col)//落子成活4的数量
{
    int sum = 0, i, u;
    for (u = 0; u < 4; u++)//4个方向，判断每个方向是否落子就成活4
    {
        int sumk = 1;
        for (i = 1; same(row + dx[u] * i, col + dy[u] * i, p[row][col]); i++)sumk++;
        if((!inboard(row + dx[u] * i, col + dy[u] * i) || p[row + dx[u] * i][col + dy[u] * i] != 0))continue;
        for (i = -1; same(row + dx[u] * i, col + dy[u] * i, p[row][col]); i--)sumk++;
        if((!inboard(row + dx[u] * i, col + dy[u] * i) || p[row + dx[u] * i][col + dy[u] * i] != 0))continue;
        if (sumk == 4)sum++;
    }
    return sum;
}
int cheng5(int row, int col)//成5点的数量
{
    int sum = 0, i, u;
    for (u = 0; u < 8; u++)//8个成五点的方向
    {
        int  sumk = 0;
        int flag = 1;
        for (i = 1; same(row + dx[u] * i, col + dy[u] * i, p[row][col]) || flag; i++)
        {
            if (!same(row + dx[u] * i, col + dy[u] * i, p[row][col]))
            {
                if (p[row + dx[u] * i][col + dy[u] * i])sumk -= 10;
                flag = 0;
            }
            sumk++;
        }
        if (!inboard(row + dx[u] * --i, col + dy[u] * i))continue;        
for (i = -1; same(row + dx[u] * i, col + dy[u] * i, p[row][col]); i--)sumk++;
        if (sumk == 4)sum++;
    }
    return sum;
}
int chong4(int row, int col)//冲4的数量
{
    return cheng5(row, col) - live4(row, col) * 2;
}
int live3(int row, int col)//落子成活3的数量
{
    int  sum = 0, i, u,flag=2;
    for (u = 0; u < 4; u++)//三连的活三
    {
        int sumk = 1;
        for (i = 1; same(row + dx[u] * i, col + dy[u] * i, p[row][col]); i++)sumk++;
        if((!inboard(row + dx[u] * i, col + dy[u] * i) || p[row + dx[u] * i][col + dy[u] * i] != 0))continue;
        i++;
        if((!inboard(row + dx[u] * i, col + dy[u] * i) || p[row + dx[u] * i][col + dy[u] * i] != 0))flag--;
        for (i = -1; same(row + dx[u] * i, col + dy[u] * i, p[row][col]); i--)sumk++;
        if((!inboard(row + dx[u] * i, col + dy[u] * i) || p[row + dx[u] * i][col + dy[u] * i] != 0))continue;
        i--;
        if((!inboard(row + dx[u] * i, col + dy[u] * i) || p[row + dx[u] * i][col + dy[u] * i] != 0))flag--;
        if (sumk == 3 && flag>0)sum++;
    }
    for (u = 0; u < 8; u++)//8个方向，每个方向最多1个非三连的活三
    {
        int  sumk = 0;
        int flag = 1;
        for (i = 1;same(row + dx[u] * i, col + dy[u] * i, p[row][col]) || flag; i++)//成活四点的方向
        {
            if (!same(row + dx[u] * i, col + dy[u] * i, p[row][col]))
            {
                if (flag&&p[row + dx[u] * i][col + dy[u] * i])sumk -= 10;
                flag = 0;
            }
            sumk++;
        }
        if((!inboard(row + dx[u] * i, col + dy[u] * i) || p[row + dx[u] * i][col + dy[u] * i] != 0))continue;;
        if (p[row + dx[u] * --i][col + dy[u] * i] == 0)continue;
        for (i = 1; same(row + dx[u] * i, col + dy[u] * i, p[row][col]); i++)sumk++;
        if((!inboard(row + dx[u] * i, col + dy[u] * i) || p[row + dx[u] * i][col + dy[u] * i] != 0))continue;;
        if (sumk == 3)sum++;
    }
    return sum;
}
int end_(int row, int col)//(row,col)处落子之后是否游戏结束
{
    for (int u = 0; u < 4; u++)if (num(row, col, u) + num(row, col, u + 4) >= 4)is_end = 1;
    if (is_end)return 1;
    return is_end;
}
void go(int row, int col)//落下一子
{
    if (s == s0)p[row][col] = 1; //标出最新下的棋
    else p[row][col] = 2;
    DrawBoard();
    if (end_(row, col))
    {
        if (s == ais)printf("AI胜");
        else printf("玩家胜");
    }
}
int ok(int row, int col)//能否落子
{
    return inboard(row, col) && (p[row][col] == 0);
}
int point(int row, int col)//非负分值
{
    if (end_(row, col))
    {
        is_end = 0;
        return 10000;
    }
    int ret = live4(row, col) * 1000 + (chong4(row, col) + live3(row, col)) * 100, u;
    for (u = 0; u < 8; u++)if (p[row + dx[u]][col + dy[u]])ret++;//无效点0分
    return ret;
}
int AI3(int p2)
{
    int keyp = -100000, tempp;
    for (int i = 1; i <= N; i++)for (int j = 1; j <= N; j++)
    {
        if (!ok(i, j))continue;
        p[i][j] = s0;
        tempp = point(i, j);
        if (tempp == 0)
        {
            p[i][j] = 0;
            continue;
        }
        if (tempp == 10000)
        {
            p[i][j] = 0;
            return 10000;
        }
        p[i][j] = 0;
        if (tempp - p2 * 2 > keyp)keyp = tempp - p2 * 2;//第三层取极大
    }
    return keyp;
}
int AI2()
{
    int keyp = 100000, tempp;
    for (int i = 1; i <= N; i++)for (int j = 1; j <= N; j++)
    {
        if (!ok(i, j))continue;
        p[i][j] = 3 - s0;
        tempp = point(i, j);
        if (tempp == 0)
        {
            p[i][j] = 0;
            continue;
        }
        if (tempp == 10000)
        {
            p[i][j] = 0;
            return -10000;
        }
        tempp = AI3(tempp);
        p[i][j] = 0;
        if (tempp < keyp)keyp = tempp;//第二层取极小
    }
    return keyp;
}
void AI()
{
    int i, j;
    int keyp = -100000, keyi, keyj, tempp;
    for (i = 1; i <= N; i++)
    {
        for (j = 1; j <= N; j++)
        {
            if (!ok(i, j))continue;
            p[i][j] = s0;
            tempp = point(i, j);
            if (tempp == 0)
            {
                p[i][j] = 0;
                continue;
            }
            if (tempp == 10000)return go(i, j);
            tempp = AI2();
            p[i][j] = 0;
            if (tempp > keyp)keyp = tempp, keyi = i, keyj = j;//第一层取极大
        }
    }
    return go(keyi, keyj);
}
void player()
{
    DrawBoard();
    printf("  轮到玩家下，请输入坐标： ");
    char c = '\n';
    int row = 0, col = 0;
    while (c<'0')scanf("%c%d", &c, &row);
    if (c < 'a')col = c - 'A' + 1;
    else col = c - 'a' + 1;
    if (!ok(row, col))
    {
        printf("此处不能下");
        Sleep(1000);
        return player();
    }
    go(row, col);
}
int main()
{
    init();
   while (!is_end)
    {
        if (s == ais)AI();
        else player();
        s = 3 - s;//换下棋方
    }
    return 0;
}


