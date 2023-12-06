/*
题目：旅游景点管理
内容：导游咨询系统要求学生实践图的构造、图的遍历、
图中求最小生成树、求最短路径等算法、查询景点信息。
要求：
1.显示各个景点以及景点之间的路径和路径长度，输入遍历的起点，输入就最短路径的起点和终点，输出路径或铺设线路。
2. 构造图时最少6个景点，各个景点之间的距离和路径，可以通过输入来得到 ，由此构建图， 也可以把这些信息存储到一个文件中， 通过读取文件构建图， 图可以采用邻接矩阵存储
3. 给出从某一景点出发，把所有景点都反访问一遍的所有路径（即DFS），会有多条不同的路径（如何求多条路径？？），要求显示DFS的路径。
4. 根据输入起点和终点，标注这两景点之间的最短路径(迪杰斯特拉算法)
5. 用PRIM或KRUSKAL算法求最小生成树

    图的应用：旅游景点管理（动物园/红果园）
    0. 退出
    1. 文件建图
    2. 手动建图
    3. 显示整体表信息
    4. DFS遍历
    5. 计算最短路径
    6. 生成最小生成树

*/

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <stack>
#include <utility>
#include <algorithm>
#include <climits>
#include <string>

using namespace std;

// 景点结构体
struct Spot {
string name; // 名称
int x, y; // 坐标
};

// 边结构体
struct Edge {
int from, to, weight; // 起点、终点、权重
};

// 图类
class Graph {
private :
int n; // 节点数
vector<Spot> spots; // 景点信息
vector<vector<int>> matrix; // 邻接矩阵

public:
// 构造函数
Graph() {
n = 0;
}
//get方法
int getNum(){
    return n;
}
vector<Spot> getSpots(){
    return spots;
}
vector<vector<int >> getMaTrix(){
    return matrix;
}

// 加载文件建图
void loadGraph(string filename) {
    ifstream fin(filename);
    fin >> n;
    matrix.resize(n);
    for (int i = 0; i < n; i++) {
        matrix[i].resize(n);
        for (int j = 0; j < n; j++) {
            fin >> matrix[i][j];
        }
    }
    for (int i = 0; i < n; i++) {
        Spot spot;
        fin >> spot.name >> spot.x >> spot.y;
        spots.push_back(spot);
    }
}

// 手动建图
void buildGraph() {
    cout << "请输入景点数：";
    cin >> n;
    matrix.resize(n);
    for (int i = 0; i < n; i++) {
        matrix[i].resize(n);
        for (int j = 0; j < n; j++) {
            cout << "请输入景点" << i + 1 << "到景点" << j + 1 << "的距离：";
            cin >> matrix[i][j];
        }
    }
    for (int i = 0; i < n; i++) {
        Spot spot;
        cout << "请输入景点" << i + 1 << "的名称、坐标(x,y)：";
        cin >> spot.name >> spot.x >> spot.y;
        spots.push_back(spot);
    }
}

// 显示图的信息
void displayGraph() {
    cout << "景点信息如下：" << endl;
    for (int i = 0; i < n; i++) {
        cout << i + 1 << ". " << spots[i].name << " (" << spots[i].x << "," << spots[i].y << ")" << endl;
    }
    cout << "路径信息如下：" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] > 0) {
                cout << spots[i].name << "到" << spots[j].name << "的距离为" << matrix[i][j] << endl;
            }
        }
    }
}

// DFS遍历图
void dfs(int start, vector<vector<int>>& paths, vector<int>& path, vector<bool>& visited) {
    visited[start] = true;
    path.push_back(start);
    for (int i = 0; i < n; i++) {
        if (matrix[start][i] > 0 && !visited[i]) {
            dfs(i, paths, path, visited);
        }
    }
    visited[start] = false;
    if (path.size() == n) {
        paths.push_back(path);
    }
    path.pop_back();
}

// 输出DFS遍历路径
void displayPaths(vector<vector<int>>& paths) {
    cout << "DFS遍历路径如下：" << endl;
    for (int i = 0; i < paths.size(); i++) {
        cout << "路径" << i + 1 << ": ";
        for (int j = 0; j < paths[i].size(); j++) {
            cout << spots[paths[i][j]].name;
            if (j < paths[i].size() - 1) {
                cout << "->";
            }
        }
        cout << endl;
    }
}

// Dijkstra算法求最短路径
void dijkstra(int start, int end) {
    vector<int> dist(n, INT_MAX); // 起点到各点的距离
    vector<bool> visited(n, false); // 是否已经访问
    vector<int> parent(n, -1); // 最短路径中的父节点
    dist[start] = 0;
    for (int i = 0; i < n; i++) {
        int u = -1;
        for (int j = 0; j < n; j++) {
            if (!visited[j] && (u == -1 || dist[j] < dist[u])) {
                u = j;
            }
        }
        if (u == -1 || u == end) {
            break; // 找不到更短的路了
        }
        visited[u] = true;
        for (int v = 0; v < n; v++) {
            if (matrix[u][v] > 0 && !visited[v]) {
                if (dist[u] + matrix[u][v] < dist[v]) {
                    dist[v] = dist[u] + matrix[u][v];
                    parent[v] = u;
                }
            }
        }
    }
    if (parent[end] == -1) {
        cout << "不存在路径！" << endl;
    } else {
        stack<int> st;
        int p = end;
        while (p != start) {
            st.push(p);
            p = parent[p];
        }
        st.push(start);
        cout << "最短路径为：";
        while (!st.empty()) {
            cout << spots[st.top()].name;
            st.pop();
            if (!st.empty()) {
                cout << "->";
            }
        }
        cout << "，长度为" << dist[end] << endl;
    }
}

// Kruskal算法求最小生成树
vector<Edge> kruskal() {
    vector<Edge> edges;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (matrix[i][j] > 0) {
                Edge e = {i, j, matrix[i][j]};
                edges.push_back(e);
            }
        }
    }
    sort(edges.begin(), edges.end(), [](const Edge& e1, const Edge& e2) {
        return e1.weight < e2.weight;
    });
    vector<int> uf(n);
    for (int i = 0; i < n; i++) {
        uf[i] = i;
    }
    vector<Edge> mst;
    for (int i = 0; i < edges.size(); i++) {
        int u = edges[i].from;
        int v = edges[i].to;
        while (uf[u] != u) u = uf[u];
        while (uf[v] != v) v = uf[v];
        if (u != v) {
            mst.push_back(edges[i]);
            uf[u] = v;
        }
    }

    return mst;
}
};

// 实现
int main() {
Graph graph;
int choice;
do {
cout << "0. 退出" << endl;
cout << "1. 文件建图" << endl;
cout << "2. 手动建图" << endl;
cout << "3. 显示整体表信息" << endl;
cout << "4. DFS遍历" << endl;
cout << "5. 计算最短路径" << endl;
cout << "6. 生成最小生成树" << endl;
cout << "请选择操作：";
cin >> choice;

    switch (choice) {
        case 0:
            cout << "bye!" << endl;
            exit(1);
        case 1: {
            string filename;
            cout << "请输入文件名：";
            cin >> filename;
            graph.loadGraph(filename);
            break;
        }
        case 2:
            graph.buildGraph();
            break;
        case 3:
            graph.displayGraph();
            break;
        case 4: {
            vector<vector<int>> paths;
            vector<int> path;
            vector<bool> visited(graph.getNum(), false);
            graph.dfs(0, paths, path, visited);
            graph.displayPaths(paths);
            break;
        }

        case 5: {
        int start, end;
        cout << "请输入起点和终点的编号：";
        cin >> start >> end;
        graph.dijkstra(start, end);
        break;
        }

        case 6: {
        vector<Edge> spt = graph.kruskal();
        int totalWeight = 0;

        for (auto& e : spt) {
        totalWeight += e.weight;
            cout<<graph.getSpots()[e.from].name << " -> " <<
            graph.getSpots()[e.to].name<< " : " << e.weight << endl;
        }
        cout << "总权重: " << totalWeight << endl;
        break;
        }
    }
    }while(choice>=0&&choice<=6);
    return 0;
}
