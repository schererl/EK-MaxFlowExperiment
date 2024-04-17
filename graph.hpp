#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <list>

 //Edge (v,u): (vertex u, weight, reverse edge addr)
struct Edge;

class Graph {
private:
    int s; //source
    int t; //sink
    std::vector<std::list<Edge*>> adj_lst;
public:
    //counting variables
    int bfs_calls;
    int bfs_iterations;
    int aug_calls;
    int M;
    int V;    
    
    Graph(int V, int src, int dst);
    void addEdge(int u, int v, int c);
    bool bfsFindPath(std::vector<Edge*> &edge_path);
    int augment(std::vector<Edge*>&edge_path);
    int  EdmondsKarp();
    
    void printPath(std::vector<Edge*>& edge_path);
    void printVertex(int u);
    void printGraph();
    ~Graph();
};


#endif