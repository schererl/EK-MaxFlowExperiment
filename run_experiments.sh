#!/bin/bash

# *main.cpp ---> uncomment exportCsv
cc Fluxo_Maximo/src/new_washington.c -o gengraph;
g++ graph.cpp main.cpp -o run;

output_file="graph_output.csv"
echo -e "MAP;ARG1;ARG2;Max Capacity;Vertices(n);Edges(m);BFS Calls(I);Augment Calls;Max Flow;Elapsed Time(s);#BFS Operations;AVG BFS Operations;Std Dev BFS Operations;C;r;s;t;uI" > "$output_file"
run_graph() {
    local p=$1
    local q=$2
    local r=$3
    local s=$4

    ./gengraph $p $q $r $s out.dimacs #|| { echo "Error generating graph for p=$p, q=$q, r=$r, s=$s"; return; }
    local run_output=$(./run < out.dimacs)
    echo -e "$p;$q;$r;$s;$run_output" >> "$output_file"
}

for m in 1; do
    for q in 2 4 8 16 32 64 128; do
        for r in 2 4 8 16 32 64 128; do
            for f in 100 1000 10000 100000; do
                run_graph $m $q $r $f
            done
        done
    done
done

#bipartite
# run_graph 4 1000 2 10000;
# run_graph 4 1000 4 10000;
# run_graph 4 1000 8 10000;
# run_graph 4 1000 16 10000;
# run_graph 4 1000 32 10000;
# run_graph 4 1000 64 10000;
# run_graph 4 1000 128 10000;
