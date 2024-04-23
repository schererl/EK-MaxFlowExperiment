#!/bin/bash

# Initialize CSV file and write the header
output_file="graph_output.csv"
echo "Vertices,Edges,BFS Calls,Augment Calls,Max Flow,Elapsed Time,Sum BFS Iterations,Average BFS Iterations,Std Dev BFS Iterations" > "$output_file"

# Function to generate graph and run with specified parameters
run_graph() {
    local p=$1
    local q=$2
    local r=$3
    local s=$4

    ./gengraph $p $q $r $s out.dimacs
    echo "Running with parameters: $p $q $r $s"
    
    # Capture the output from the run and append it to the CSV file
    ./run < out.dimacs >> "$output_file"
}

# Loop through different parameter sets and execute
for q in 2 4 8 16; do
    for r in 2 4 8 16; do
        run_graph 1 $q $r 1000
    done
done
