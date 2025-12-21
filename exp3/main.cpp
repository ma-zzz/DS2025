#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <limits>
#include <algorithm>
using namespace std;

// 边的结构体
struct Edge {
    int src, dest, weight;
};

// 比较边的权重
bool compareEdges(const Edge& a, const Edge& b) {
    return a.weight < b.weight;
}

// 查找根节点
int find(const vector<int>& parent, int i) {
    if (parent[i] == i)
        return i;
    return find(parent, parent[i]);
}

// 合并集合
void unionSets(vector<int>& parent, vector<int>& rank, int x, int y) {
    x = find(parent, x);
    y = find(parent, y);
    if (rank[x] < rank[y])
        parent[x] = y;
    else if (rank[x] > rank[y])
        parent[y] = x;
    else {
        parent[y] = x;
        rank[x]++;
    }
}

// 图的结构
class Graph {
private:
    int V; // 顶点数
    vector<vector<int>> adjMatrix; // 邻接矩阵

public:
    Graph(int V) {
        this->V = V;
        adjMatrix.resize(V, vector<int>(V, numeric_limits<int>::max())); // 初始化邻接矩阵为无穷大
        for (int i = 0; i < V; i++) {
            adjMatrix[i][i] = 0; // 自己到自己的距离为0
        }
    }

    void addEdge(int src, int dest, int weight = 1) {
        adjMatrix[src][dest] = weight;
        adjMatrix[dest][src] = weight; // 无向图
    }

    void printAdjMatrix() {
        cout << "图的邻接矩阵为：" << endl;
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                if (adjMatrix[i][j] == numeric_limits<int>::max()) {
                    cout << "INF ";
                }
                else {
                    cout << adjMatrix[i][j] << " ";
                }
            }
            cout << endl;
        }
    }

    void BFS(int start) {
        vector<bool> visited(V, false);
        queue<int> q;
        q.push(start);
        visited[start] = true;

        while (!q.empty()) {
            int node = q.front();
            q.pop();
            cout << node << " ";

            for (int i = 0; i < V; i++) {
                if (adjMatrix[node][i] != numeric_limits<int>::max() && !visited[i]) {
                    q.push(i);
                    visited[i] = true;
                }
            }
        }
    }

    void DFSUtil(int node, vector<bool>& visited) {
        cout << node << " ";
        visited[node] = true;

        for (int i = 0; i < V; i++) {
            if (adjMatrix[node][i] != numeric_limits<int>::max() && !visited[i]) {
                DFSUtil(i, visited);
            }
        }
    }

    void DFS(int start) {
        vector<bool> visited(V, false);
        DFSUtil(start, visited);
    }

    void dijkstra(int start) {
        vector<int> dist(V, numeric_limits<int>::max());
        dist[start] = 0;
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
        pq.push({ 0, start });

        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();

            for (int v = 0; v < V; v++) {
                if (adjMatrix[u][v] != numeric_limits<int>::max() && dist[u] + adjMatrix[u][v] < dist[v]) {
                    dist[v] = dist[u] + adjMatrix[u][v];
                    pq.push({ dist[v], v });
                }
            }
        }

        cout << "从A点出发的最短路径结果为：" << endl;
        for (int i = 0; i < V; i++) {
            cout << "A到" << i << "的最短距离为：" << dist[i] << endl;
        }
    }

    void kruskalMST() {
        vector<Edge> edges;
        for (int i = 0; i < V; i++) {
            for (int j = i + 1; j < V; j++) {
                if (adjMatrix[i][j] != numeric_limits<int>::max()) {
                    edges.push_back({ i, j, adjMatrix[i][j] });
                }
            }
        }

        sort(edges.begin(), edges.end(), compareEdges);

        vector<int> parent(V);
        vector<int> rank(V, 0);
        for (int i = 0; i < V; i++) {
            parent[i] = i;
        }

        vector<Edge> mst;
        for (const Edge& edge : edges) {
            int x = find(parent, edge.src);
            int y = find(parent, edge.dest);
            if (x != y) {
                mst.push_back(edge);
                unionSets(parent, rank, x, y);
            }
        }

        cout << "最小支撑树的边为：" << endl;
        for (const Edge& edge : mst) {
            cout << edge.src << " - " << edge.dest << " : " << edge.weight << endl;
        }
    }
};

int main() {
    int V = 6; // 假设图1有6个顶点
    Graph g(V);

    // 添加边
    g.addEdge(0, 1, 1); // A-B
    g.addEdge(0, 2, 2); // A-C
    g.addEdge(1, 3, 3); // B-D
    g.addEdge(2, 3, 1); // C-D
    g.addEdge(2, 4, 2); // C-E
    g.addEdge(3, 5, 4); // D-F
    g.addEdge(4, 5, 1); // E-F

    // 输出邻接矩阵
    g.printAdjMatrix();

    // BFS和DFS
    cout << "从A点出发的BFS遍历结果为：" << endl;
    g.BFS(0);
    cout << endl;

    cout << "从A点出发的DFS遍历结果为：" << endl;
    g.DFS(0);
    cout << endl;

    // 最短路径
    g.dijkstra(0);

    // 最小支撑树
    g.kruskalMST();

    return 0;
}
