#include "graph.hpp"
#include <iostream>
#include <list>
#include<queue>
#include <climits>
#include <tuple>
#include <complex>
#include <iostream>
#include <vector>
#include <fstream>

struct Edge {
    int u, v;
    int capacity;
    int flow;
    Edge* reverse;
    int cpath_count; //critical path count: experimental stuff
    Edge(int _u, int _v, int _capacity)
        : u(_u), v(_v), capacity(_capacity), flow(0), reverse(nullptr), cpath_count(0) {}
};

Graph::Graph(int V, int source, int sink):V(V), M(0), s(source),t(sink),adj_lst(V), bfs_calls(0), aug_calls(0), bfs_operations(0), max_aug_path(0){}

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
    visited[s]=true;
    q.push(s);
    edge_path[s]=nullptr;

    int iterations = 0;
    int v_arcs=0;
    while(!q.empty()){
        iterations+=1;
        int u = q.front();
        q.pop();
        
        for (Edge* edge : adj_lst[u]) {
            bfs_operations+=1;
            if (!visited[edge->v] && edge->capacity - edge->flow > 0) {
                visited[edge->v] = true;
                edge_path[edge->v] = edge;
                v_arcs+=1;
                if(edge->v == t){
                    bfs_count_op.emplace_back(v_arcs);
                    visited_arcs_bfs.emplace_back(v_arcs);
                    visited_nodes_bfs.emplace_back(iterations);

                    while (!q.empty()) q.pop();
                    return true;
                } 
                q.push(edge->v);
            }
        }
    }
    visited_arcs_bfs.emplace_back(v_arcs);
    visited_nodes_bfs.emplace_back(iterations);
    bfs_count_op.emplace_back(v_arcs);
    while (!q.empty()) q.pop();
    return false;
}


int Graph::augment(std::vector<Edge*>&edge_path) {
    aug_calls+=1;
    int path_flow = INT_MAX;
    int path_size=0;
    // find minimum residual capacity (bottleneck)
    for (int v = t; edge_path[v] != nullptr; v = edge_path[v]->u) {
        Edge *e = edge_path[v];
        path_flow = std::min(path_flow, e->capacity - e->flow);
        path_size+=1;
    }
    
    minimum_path_size.emplace_back(path_size);
    int cedgedc=0;
    // update residual capacities of the forward edges and backward (reverse) edges along the path
    
    for (int v = t; edge_path[v] != nullptr; v = edge_path[v]->u) {
        path_size+=1;
        Edge* e = edge_path[v];
        if(e->capacity-e->flow == path_flow) { //experimental stuff
            e->cpath_count+=1; 
            cedgedc+=1;
        }
        e->flow += path_flow;
        e->reverse->flow -= path_flow;
        
    }
    if(path_size > max_aug_path){
        max_aug_path=path_size;
    }
    crit_edges_count.emplace_back(cedgedc);
    return path_flow;
}

int Graph::EdmondsKarp() {
    auto start = std::chrono::high_resolution_clock::now();
    int max_flow = 0;
    std::vector<Edge *> edge_path(V);
    
    // while there exists an augmenting path, push flow
    while (bfsFindPath(edge_path)) {
        augment(edge_path);
    }
    for(Edge *e:adj_lst[t]){ max_flow+=e->reverse->flow;}
    
    auto end = std::chrono::high_resolution_clock::now();
    elapsed_time = end - start;
    last_mflow = max_flow;
    
    std::cout << "Algorithm ended after: " << bfs_calls << " iterations. " << std::endl;
    std::cout << "Elapsed time: " <<  elapsed_time.count() << " seconds. " << std::endl; 
    std::cout << "Total matchings: " << max_flow << " matchings." << std::endl;
    std::cout << "Maximum size augmenting path: " << max_aug_path << std::endl;
    std::cout << "BFS calls: " << bfs_calls << std::endl;
    std::cout << "BFS operations: " << bfs_operations << std::endl;
    
    return max_flow;
}

Graph::~Graph() {
    for (auto &list : adj_lst) {
        for (Edge* edge : list) {
            delete edge;
        }
        list.clear(); 
    }
}

