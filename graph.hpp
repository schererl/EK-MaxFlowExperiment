#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <list>
#include <chrono>
 //Edge (v,u): (vertex u, weight, reverse edge addr)
struct Edge;

class Graph {
private:
    int s; //source
    int t; //sink
    std::vector<std::list<Edge*>> adj_lst;
    int M;
    int V;    
    
    // experimental stuff
    std::vector<int> minimum_path_size;
    std::vector<int> bfs_count_op; 
    std::vector<int> visited_arcs_bfs;
    std::vector<int> visited_nodes_bfs;
    std::vector<int> crit_edges_count;
    
    std::chrono::duration<double> elapsed_time;
    int bfs_calls;
    int bfs_operations;
    int aug_calls;
    int max_aug_path;
    

public:
    int last_mflow;
    
    // the real stuff
    Graph(int V, int src, int dst);
    void addEdge(int u, int v, int c);
    bool bfsFindPath(std::vector<Edge*> &edge_path);
    int augment(std::vector<Edge*>&edge_path);
    int  EdmondsKarp();
    
    ~Graph();
};


#endif