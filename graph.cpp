#include "graph.hpp"
#include <iostream>
#include <list>
#include<queue>
#include <climits>
#include <tuple>

struct Edge {
    int u, v;
    int capacity;
    int flow;
    Edge* reverse;

    Edge(int _u, int _v, int _capacity)
        : u(_u), v(_v), capacity(_capacity), flow(0), reverse(nullptr) {}
};

Graph::Graph(int V, int source, int sink):V(V), M(0), s(source),t(sink),adj_lst(V){}

//add or update weight for edges forward edge (u,v) and backward edge(v,u)
void Graph::addEdge(int u, int v, int c){
    M+=1;
    for(auto *edge : adj_lst[u]){
        if(edge->u == u && edge->v == v){
            edge->capacity += c;
            return;
        }
    }
    
    Edge *forward_edge  = new Edge(u, v, c);
    Edge *backward_edge = new Edge(v, u, 0);

    forward_edge->reverse = backward_edge;
    backward_edge->reverse = forward_edge;
    adj_lst[u].push_back(forward_edge);
    adj_lst[v].push_back(backward_edge);
}


bool Graph::bfsFindPath(std::vector<Edge*> &edge_path) {
    bfs_calls+=1;
    std::fill(edge_path.begin(), edge_path.end(), nullptr); 
    std::vector<bool> visited(V, false);
    std::queue<int> q;
    q.push(s);
    edge_path[s]=nullptr;
    while(!q.empty()){
        bfs_iterations+=1;
        int u = q.front();
        q.pop();
        visited[u] = true;
        for (Edge* edge : adj_lst[u]) {
            if (!visited[edge->v] && edge->capacity - edge->flow > 0) {
                edge_path[edge->v] = edge;
                if(edge->v == t){
                    while (!q.empty()) q.pop();
                    return true;
                } 
                q.push(edge->v);
            }
        }
    }
    
    while (!q.empty()) q.pop();
    return false;
}


int Graph::augment(std::vector<Edge*>&edge_path) {
    aug_calls+=1;
    int path_flow = INT_MAX;

    // find minimum residual capacity (bottleneck)
    for (int v = t; edge_path[v] != nullptr; v = edge_path[v]->u) {
        Edge *e = edge_path[v];
        path_flow = std::min(path_flow, e->capacity - e->flow);
    }
    
    // update residual capacities of the forward edges and backward (reverse) edges along the path
    for (int v = t; edge_path[v] != nullptr; v = edge_path[v]->u) {
        Edge* e = edge_path[v];
        e->flow += path_flow;
        e->reverse->flow -= path_flow;
    }
    return path_flow;
}

int Graph::EdmondsKarp() {
    int max_flow = 0;
    std::vector<Edge *> edge_path(V);
    
    // while there exists an augmenting path, push flow
    while (bfsFindPath(edge_path)) {
        augment(edge_path);
    }
    for(Edge *e:adj_lst[t]){ max_flow+=e->reverse->flow;}
    return max_flow;
}

void Graph::printPath(std::vector<Edge*>& edge_path) {
    std::cout << "printing path" << std::endl;
    std::list<std::tuple<int, int>> rev_path;
    for (int v = t; edge_path[v] != nullptr; v = edge_path[v]->u) {
        Edge *e = edge_path[v];
        rev_path.emplace_front(std::make_tuple(e->u, v));
    }

    std::cout << "BFS PATH: ";
    for (const std::tuple<int, int> &tp : rev_path) {
        std::cout << "(" << std::get<0>(tp) << "," << std::get<1>(tp) << ") ";
    }
    std::cout << std::endl;

    std::cout << "FLOW PATH: \n";
    for(const std::tuple<int,int> &tp:rev_path){printVertex(std::get<0>(tp));}
    printVertex(t);
}

void Graph::printVertex(int u){
    std::cout << "\tvertex " << u << std::endl;
    for (Edge* edge : adj_lst[u]) {
        std::cout << "\t\t(to: " << edge->v << ", c" << edge->capacity << " f" << edge->flow << ") ";
        std::cout << " (reverse:  c" << edge->reverse->capacity << " f" << edge->reverse->flow << ") \n";
    }
    std::cout << std::endl;
    
}

void Graph::printGraph(){
    for (int i = 0; i < V; ++i) {
        std::cout << "Adjacency list of vertex " << i << " \n";
        for (Edge* edge : adj_lst[i]) {
            std::cout << "\t(to: " << edge->v << ", c" << edge->capacity << " f" << edge->flow << ") ";
            std::cout << " (reverse:  c" << edge->reverse->capacity << " f" << edge->reverse->flow << ") \n";
        }
        std::cout << std::endl;
    }
}


Graph::~Graph() {
    for (auto &list : adj_lst) {
        for (Edge* edge : list) {
            delete edge;
        }
        list.clear(); 
    }
}

