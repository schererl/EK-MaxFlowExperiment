#include "graph.hpp"
#include <iostream>
#include <sstream>
#include <string>

Graph* read_dimacs_max_flow(std::istream& in) {
    std::string line;
    int n, m, s = -1, t = -1;
    Graph* g = nullptr;

    while (getline(in, line)) {
        if (line[0] == 'c') {
            continue;  // Comment line, ignore it
        } else if (line[0] == 'p') {
            std::istringstream iss(line);
            std::string tmpStr;
            iss >> tmpStr >> tmpStr >> n >> m;  // tmpStr is used to skip the "p max"
        } else if (line[0] == 'n') {
            int id;
            char type;
            std::istringstream iss(line);
            iss >> type >> id >> type;
            if (type == 's')
                s = id - 1; // Convert 1-based to 0-based index
            else if (type == 't')
                t = id - 1;
        } else if (line[0] == 'a') {
            if (!g && s != -1 && t != -1)
                g = new Graph(n, s, t); // Initialize graph only once source and sink are known
            int u, v, c;
            char tmp;
            std::istringstream iss(line);
            iss >> tmp >> u >> v >> c; // 'tmp' to skip the 'a' in the arc description
            g->addEdge(u - 1, v - 1, c); // Convert 1-based to 0-based index
        }
    }
    if (!g) {
        std::cerr << "Error: Graph not initialized properly." << std::endl;
    }
    return g;
}

int main() {
    Graph* g = read_dimacs_max_flow(std::cin);
    if (g) {
        std::cout << "Max Flow from source to sink: " << g->EdmondsKarp() << std::endl;
        delete g; // Clean up dynamically allocated memory
    } else {
        std::cerr << "Graph initialization failed." << std::endl;
        return 1;
    }
    return 0;
}
