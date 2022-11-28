#include <fstream>
#include <iostream>
#include <limits>
#include <math.h>
#include <omp.h>
#include <stack>
#include <vector>

using namespace std;

#define INF numeric_limits<int>::max()

typedef vector<vector<int>> Graph;
int p;

struct Node {
    Graph graph;
    int cost;
    int cost_parent;

    Node() {
        cost = 0;
        cost_parent = 0;
    }
};

struct TSP {
    int n;
    Graph graph;
    Node * root;
    vector<int> path;
    int score;

    void reduce_graph(Node * node);
    void reduce(Node * node, int from, int to);

    TSP(int n, Graph graph, int p);
    void print_graph(Graph graph);
    void solve();
    bool print_solution();
    bool print_solution_to_file(string filename);
    int get_score();
};

TSP::TSP(int n, Graph graph, int p) {
    this->n = n;
    this->graph = graph;
    p = p;
    this->score = INF;
    
    root = new Node;
    root->graph = graph;
    root->cost = 0;
    root->cost_parent = 0;
}

void TSP::reduce_graph(Node * node) {
    int cost = 0;

    // Reduce rows
    for(int i = 0; i < n; i++) {
        int min = INF;
        for(int j = 0; j < n; j++) {
            if(node->graph[i][j] < min)
                min = node->graph[i][j];
        }
        if(min != INF && min > 0) {
            cost += min;
            for(int j = 0; j < n; j++) {
                if(node->graph[i][j] != INF)
                    node->graph[i][j] -= min;
            }
        }
    }
    // Reduce columns
    for(int j = 0; j < n; j++) {
        int min = INF;
        for(int i = 0; i < n; i++) {
            if(node->graph[i][j] < min)
                min = node->graph[i][j];
        }
        if(min != INF && min > 0) {
            cost += min;
            for(int i = 0; i < n; i++) {
                if(node->graph[i][j] != INF)
                    node->graph[i][j] -= min;
            }
        }
    }

    node->cost += cost;
}

void TSP::reduce(Node * node, int from, int to) {
    node->cost += node->graph[from][to] + node->cost_parent;
    
    for(int i = 0; i < n; i++) {
        node->graph[from][i] = INF;
        node->graph[i][to] = INF;
    }
    
    node->graph[to][0] = INF;
    reduce_graph(node);
}

void TSP::print_graph(Graph graph) {
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if(graph[i][j] == INF)
                cout << "INF ";
            else
                cout << graph[i][j] << " ";
        }
        cout << '\n';
    }
}

void TSP::solve() {
    path = {0};
    reduce_graph(root);
    stack<pair<Node *, vector<int>>> nodes;
    nodes.push({root, path});

    while(!nodes.empty()) {
        auto current = nodes.top();
        Node * current_node = current.first;
        vector<int> current_path = current.second;
        nodes.pop();

        if(current_node->cost >= score)
            continue;

        int from = current_path.back();
        #pragma omp parallel for shared(nodes, current_node, current_path) num_threads(p)
        for(int to = 0; to < n; to++) {
            if(current_node->graph[from][to] != INF) {
                Node * new_node = new Node;
                new_node->graph = current_node->graph;
                new_node->cost_parent = current_node->cost;
                reduce(new_node, from, to);

                vector<int> new_path = current_path;
                new_path.push_back(to);

                if(new_path.size() == n) {
                    if(new_node->cost < score) {
                        #pragma omp critical
                        {
                            score = new_node->cost;
                            path = new_path;
                        }
                    }
                } else {
                    if(new_node->cost < score)
                        #pragma omp critical
                        nodes.push({new_node, new_path});
                }
            }
        }
        #pragma omp barrier

        delete current_node;
    }
}

bool TSP::print_solution() {
    if(path.empty())
        return false;
    printf("Solution: ");
    for(int i = path.size() - 1; i >= 0; i--)
        cout << path[i] << " ";
    cout << '\n';

    printf("Cost: %d\n", score);
    return true;
}

bool TSP::print_solution_to_file(string filename) {
    if(path.empty())
        return false;
    ofstream file;
    file.open(filename);
    for(int i = path.size() - 1; i >= 0; i--)
        file << path[i] << " ";
    file << endl;
    file.close();
    return true;
}

int TSP::get_score() {
    return score;
}