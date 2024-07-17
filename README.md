# EK-MaxFlowExperiement
Assignment on Maximum Flow Problem - Advanced Algorithms Course (UFRGS) 

*Branch exclusively for Matching Assignment*

Matching assingment can be found here: https://github.com/schererl/hopcroft-Karp-Experiment

This branch includes modifications to compare maximum matching reduction to a flow problem. Several pieces of irrelevant code have been removed, but some parts remain, I don't have time for it.

running example:

```
g++ graph.cpp main.cpp -o bip_flow; ./graph_generator 100 100 0.014 123 | ./bip_flow 
``` 

 - You can get 'graph_generator' from hocroft-karp repo.