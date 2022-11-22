#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <climits>
#include <algorithm>
#include <math.h>
#include <fstream>

using namespace std;

#define INF INT_MAX

typedef vector<vector<int>> Graph;
class Node {
public:
    Graph graph;
    vector<int> path;
    int cost;
    int curr;
    Node* parent;

    Node() {
        cost = 0;
        curr = 0;
        parent = nullptr;
    }
};

class TSP
{
private:
    int n;
    Graph graph;
    Node* root;
    vector<int> path;

    void reduceGraph(Node* node);
    void reduce(Node* node, int from, int to);

public:
    TSP(int n, Graph graph);
    void printGraph();
    void solve();
    bool printSolution();
    bool printSolutionToFile(string filename);
};

TSP::TSP(int n, Graph graph)
{
    this->n = n;
    this->graph = graph;
    
    root = new Node;
    root->graph = graph;
    root->path = {0};
    root->cost = 0;
    root->curr = 0;
    root->parent = nullptr;
}

void TSP::reduceGraph(Node* node)
{
    int cost = 0;
    //for each row
    for(int i=0; i<n; i++)
    {
        int mn = *min_element(node->graph[i].begin(), node->graph[i].end());
        if(mn != INF)
        {
            cost += mn;
            for(int j=0; j<n; j++)
                if(node->graph[i][j] != INF)
                    node->graph[i][j] -= mn;
        }
    }
    //for each column
    for(int i=0; i<n; i++)
    {
        int mn = INF;
        for(int j=0; j<n; j++)
            if(node->graph[j][i] < mn)
                mn = node->graph[j][i];
        if(mn != INF)
        {
            cost += mn;
            for(int j=0; j<n; j++)
                if(node->graph[j][i] != INF)
                    node->graph[j][i] -= mn;
        }
    }
    node->cost += cost;
}

void TSP::reduce(Node* node, int from, int to) {
    node->cost += node->graph[from][to] + node->parent->cost;
    for (int i = 0; i < n; i++) node->graph[from][i] = INT_MAX;
    for (int i = 0; i < n; i++) node->graph[i][to] = INT_MAX;
    node->graph[to][0] = INT_MAX;
    reduceGraph(node);
}


void TSP::solve()
{
    path = {0};
    reduceGraph(root);

    auto cmp = [](Node* a, Node* b) {
        return a->cost > b->cost;
    };
    priority_queue<Node*, vector<Node*>, decltype(cmp)> pq(cmp);
    pq.push(root);

    while (!pq.empty()) {
        Node* cur = pq.top();
        pq.pop();
        int from = cur->curr;
        if (cur->path.size() == n) {
            // Find a solution
            cur->path.push_back(0);
            path = cur->path;
            break;
        }
        for(int to = 0; to < n; to++){
            //if to is elegible
            bool elegible = true;
            vector<int> path = cur->path;
            for(auto &i : path){
                if(i == to) {elegible = false;break; }
            }
            if(!elegible) continue;
            Node* child = new Node();
            child->graph = cur->graph;
            child->path = cur->path;
            child->path.push_back(to);
            child->curr = to;
            child->parent = cur;

            reduce(child, from, to);
            pq.push(child);
        }
    }
} 

bool TSP::printSolution()
{
    if(path.size() == 0) {
        printf("Make solve to generate a solution\n");
        return 0;
    }
    int cost = 0;
    int prev = -1;
    printf("Solution: ");
    for (int &i : path) {
        printf("%d ", i);
        if (prev != -1) cost += graph[prev][i];
        prev = i;
    }
    printf("\nCost: %d\n", cost);
    return 1;
}

bool TSP::printSolutionToFile(string filename)
{
    if(!printSolution()) return 0;
    
    int cost = 0;
    int prev = -1;
    for (int &i : path) {
        if (prev != -1) cost += graph[prev][i];
        prev = i;
    }
    
    ofstream file;
    file.open(filename, ios::out);
    if(file.fail()) return 0;
    file << cost << "\n";
    for (int &i : path) {
        file << i + 1 << "\n";
    }
    file.close();
    return 1;
}

void TSP::printGraph()
{
    printf("Graph of size %d:\n", n);
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++)
            printf("%d ", graph[i][j]);
        printf("\n");
    }
    printf("\n");
}