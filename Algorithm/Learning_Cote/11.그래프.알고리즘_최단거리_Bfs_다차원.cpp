#if 0

/*
    - 최단거리 Bfs 에서, 조건 이 생길 때 사용할 수 있는 Bfs _ 다차원 문제 답이다.
    - 출저 : https://school.programmers.co.kr/learn/courses/30/lessons/67259

    - 아래와 같은 패턴은 최단거리에 조건부가 생길 때 사용할 수 있다 한다. 아직 안풀어봤지만,

    - 예시 1: 벽 부수고 이동하기 (백준 2206)
    - 예시 2: 말이 되고픈 원숭이 (백준 1600)
    - 예시 3: 로봇 청소기 경로 (백준 4991)

     가 있다고 한다. 체감상 많이 어려운데, 위 백준 3문제들도 나중에 풀었으면 좋겠다

*/


#include <string>
#include <vector>
#include <queue>
#include <functional>
#include <stdio.h>

using namespace std;

#define NORTH   0
#define SOUTH   1
#define WEST    2
#define EAST    3   

#define STRAIGHT    100
#define CORNER      600


struct Pos
{
    Pos(int Y, int X, int Dir) : y(Y), x(X), dir(Dir) {}
    Pos() {}
    int y;
    int x;
    int dir;
};
struct Node
{
    bool passable = false;
    int cost[4] = { INT32_MAX, INT32_MAX, INT32_MAX, INT32_MAX };
};

struct Graph
{
    void Initialize(const vector<vector<int>>& board, int yMax, int xMax)
    {
        nodes = vector<vector<Node>>(yMax + 1, vector<Node>(xMax + 1));

        for (int y = 0; y <= yMax; y++)
        {
            for (int x = 0; x <= xMax; x++)
            {
                if (board[y][x] == 0) nodes[y][x].passable = true;
            }
        }
    }
    int Bfs(const vector<vector<int>>& board)
    {
        int yMax = board.size() - 1;
        int xMax = board[0].size() - 1;
        Initialize(board, yMax, xMax);

        queue<Pos> q;
        Pos start1(0, 0, EAST);
        Pos start2(0, 0, SOUTH);
        q.push(start1);
        q.push(start2);
        Node& startNode = nodes[0][0];
        startNode.cost[EAST] = 0;
        startNode.cost[SOUTH] = 0;


        int dir[4][2] =
        {
            {-1, 0}, {1, 0}, {0, -1}, {0, 1}
        };

        auto IsPossible = [&](Pos aim, const Node& prev)
            {
                auto& [y, x, dir] = aim;
                if (y < 0 || y > yMax || x < 0 || x > xMax) return false;

                if (nodes[y][x].passable == false) return false;

                return true;
            };

        while (!q.empty())
        {
            Pos curr = q.front();
            q.pop();

            // DEBUG
#if 0
            printf("%d, %d, %d\n", curr.y, curr.x, curr.dir);
#endif

            Node& curNode = nodes[curr.y][curr.x];
            int cost = curNode.cost[curr.dir];
            for (int i = 0; i < 4; i++)
            {
                Pos aim(curr.y + dir[i][0], curr.x + dir[i][1], i);

                if (!IsPossible(aim, curNode)) continue;

                int newCost = cost + (i == curr.dir ? STRAIGHT : CORNER);
                Node& aimNode = nodes[aim.y][aim.x];
                if (newCost < aimNode.cost[i])
                {
                    aimNode.cost[i] = newCost;
                    q.push(aim);
                }
            }
        }

        int ret = INT32_MAX;
        Node& lastNode = nodes[yMax][xMax];
        for (int i = 0; i < 4; i++)
        {
            if (lastNode.cost[i] < ret) ret = lastNode.cost[i];
        }
        return ret;
    }

    vector<vector<Node>> nodes;
};


int solution(vector<vector<int>> board)
{
    Graph graph;

    int answer = graph.Bfs(board);
    return answer;
}


int main()
{
    printf("11.그래프.알고리즘_최단거리_bfs_다차원.cpp");
    printf("\n\n\n");

    return 0;
}


#endif