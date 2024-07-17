#include "graph.hpp"
#include <iostream>
#include <sstream>
#include <string>

int main() {
    int n1, n2, num_edges;
    std::string line;

    std::getline(std::cin, line);
    std::istringstream first_line(line);
    char p, edge[5];
    first_line >> p >> edge >> n1 >> n2 >> num_edges;
    
    int n = n1 + n2;
    int m = num_edges; 
    int s = n1 + n2; 
    int t = n1 + n2 + 1;

    // std::cout << "n:" << n << std::endl;
    // std::cout << "m:" << m << std::endl;
    // std::cout << "s:" << s << std::endl;
    // std::cout << "t:" << t << std::endl;
    Graph *g = new Graph(n+2, s, t);

    while (std::getline(std::cin, line)) {
        std::istringstream edge_line(line);
        char e;
        int u, v;
        edge_line >> e >> u >> v;
        g->addEdge(u, v, 1);
    }
    // add edges of s and t
    for(int i = 0; i < n/2; i++){
        g->addEdge(s, i, 1);
    }
    for(int i = n/2; i < n; i++){
        g->addEdge(i, t, 1);
    }

    if (g) {
        g->EdmondsKarp();
        
        //g->exportCSV(); // experiments
        //std::cout << g->last_mflow << std::endl; // assignment
        delete g;
    } else {
        std::cerr << "Graph initialization failed." << std::endl;
        return 1;
    }
    return 0;
}
