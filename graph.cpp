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

Graph::Graph(int V, int source, int sink):V(V), M(0), s(source),t(sink),adj_lst(V), bfs_calls(0), aug_calls(0){}

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
        Edge* e = edge_path[v];
        if(e->capacity-e->flow == path_flow) { //experimental stuff
            e->cpath_count+=1; 
            cedgedc+=1;
        }
        e->flow += path_flow;
        e->reverse->flow -= path_flow;
        
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

void Graph::printElapsedTime(){
    std::cout << "Elapsed time: " << elapsed_time.count() << " seconds." << std::endl;
}

void Graph::printCritialPath(){
    int critical_path_edges_count = 0;
        std::cout << "Critical Path Edge Counts:\n";
        for (const auto& list : adj_lst) {
            for (auto* edge : list) {
                if (edge->cpath_count > 1) {
                    critical_path_edges_count++;
                    std::cout << "Edge (" << edge->u << " -> " << edge->v << ") was critical " << edge->cpath_count << " times\n";
                }
            }
        }
        std::cout << "Number of edges that were part of critical paths: " << critical_path_edges_count << std::endl;
}

void Graph::printMinimumPath(){
    if (!minimum_path_size.empty()) {
        std::cout << "Minimum Path Sizes: ";
        for (auto size : minimum_path_size) {
            std::cout << size << " ";
        }
        std::cout << std::endl;
    }
}

void Graph::printBFSPath(){
    // Sum, Average, and Standard Deviation of BFS Iterations
    if (!bfs_count_op.empty()) {
        int sum = 0;
        for (int iter : bfs_count_op) {
            sum += iter;
        }
        double average = static_cast<double>(sum) / bfs_count_op.size();

        double variance = 0;
        for (int iter : bfs_count_op) {
            variance += (iter - average) * (iter - average);
        }
        double stddev = std::sqrt(variance / bfs_count_op.size());

        std::cout << "Sum of BFS Iterations: " << sum << std::endl;
        std::cout << "Average BFS Iterations: " << average << std::endl;
        std::cout << "Standard Deviation of BFS Iterations: " << stddev << std::endl;
    }
}

void Graph::printGraphInfo(){
    std::cout << "Vertices: " << V << std::endl;
    std::cout << "Edges: " << M << std::endl;
    std::cout << "BFS calls (" << bfs_calls << ')' << std::endl;
    std::cout << "Augment: " << aug_calls << std::endl;
}

void Graph::exportCSV() {
    int sum_iterations = 0, total_calls = bfs_count_op.size();
    for (int iter : bfs_count_op) {
        sum_iterations += iter;
    }
    double avg_iterations = total_calls > 0 ? static_cast<double>(sum_iterations) / total_calls : 0;
    double variance = 0;
    for (int iter : bfs_count_op) {
        variance += (iter - avg_iterations) * (iter - avg_iterations);
    }
    double stddev_iterations = total_calls > 0 ? std::sqrt(variance / total_calls) : 0;

    double C = 0.0; // fraction of critical edges
    double r = 0.0; // average criticality
    double s = 0.0; // sum of BFS iteration fractions
    double t = 0.0; // sum of fractions of critical edges per iteration

    // Calculate C and r
    int num_critical_edges = 0;
    for (const auto& edge_list : adj_lst) {
        for (const Edge* edge : edge_list) {
            if (edge->cpath_count > 0) {
                num_critical_edges++;
                double r_a = static_cast<double>(edge->cpath_count) / (V / 2);
                r += r_a;
            }
        }
    }           
    C = static_cast<double>(num_critical_edges) / M;
    r /= M;
    int total_aug_paths = bfs_count_op.size();
    for (size_t i = 0; i < bfs_count_op.size(); i++) {
        double u_s = (static_cast<double>(visited_nodes_bfs[i])/V);
        s += u_s / bfs_count_op.size();
        double u_t = (static_cast<double>(visited_arcs_bfs[i])/M) / bfs_count_op.size();
        t += u_t / bfs_count_op.size();
    }
    
    double up_I = static_cast<double>(aug_calls)/(r*static_cast<double>(M)*static_cast<double>(V)/2);
    std::cout << V << ';' << M << ';' << bfs_calls << ';' << aug_calls << ';'<< last_mflow  << ';' << elapsed_time.count() << ';' << sum_iterations << ';' << avg_iterations << ';' << stddev_iterations << ';' << C << ';' << r << ';' << s << ';' << t << ';' << up_I << std::endl;
}

Graph::~Graph() {
    for (auto &list : adj_lst) {
        for (Edge* edge : list) {
            delete edge;
        }
        list.clear(); 
    }
}

