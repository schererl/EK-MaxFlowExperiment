# EK-MaxFlowExperiement
Assignment on Maximum Flow Problem - Advanced Algorithms Course (UFRGS) 

# TODO:
- [x] Count the number of operations in BFS.
- [x] Count the number of augmentations.
- [ ] Count the total number of operations, divide by its theoretical bound O(nm**2)
- [x] Count the number of times a single edge is saturated.
- [ ] Create a Bash script to run operations experiments.
- [ ] Develop a Python script for plotting experiments.
- [x] Compute the path size over each augmentation.
- [x] Count the number of times each edge is a critical edge 

# Test Plan

Run each test for 5 minutes at most. Use 3 maps Mesh, Random Level, Matching. 
Use the following paramaters
2 2 1000, 4 2 1000, 8 2 1000, 16 2 1000
2 4 1000, 4 4 100, 8 4 1000, 16 4 1000
2 8 1000, 4 8 1000, 8 8 1000, 16 8 1000
2 16 1000, 4 16 100, 8 16 1000, 16 16 1000

# Assignment 
## Deadline: May 1, 2024.

#### Objectives:
- Implement the Edmonds-Karp algorithm to find the maximum s-t flow.
- Experimentally verify the algorithm's complexity.

#### Test Cases:
- A test case generator in DIMACS format in C is available [here](https://github.com/mrpritt/Fluxo_Maximo).

#### Conventions:
- Algorithm implementations should accept an instance in DIMACS format on standard input (stdin) and print the value of the maximum flow on standard output (stdout).

#### Test Case Generator:
- The maxflow.cpp code calculates the maximum flow. To compile: use C++ and Boost.


