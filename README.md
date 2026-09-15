# Graph Theory MST Group Homework

## Identity
|    NRP     |           Nama             |
| :--------: |       :------------:       |
| 5025251260 | Aqilah Ibrahim             |
| 5025251161 | Rizqi Arya Kuskhilbyano    |
| 5025251009 | Athar Rozy Rasyidan                    |
| 5025251017 | Wafi Fawwaz Sutisna                    |

## Algorithm

### Prim's Algorithm
Prim’s algorithm is a Greedy algorithm like Kruskal's algorithm. This algorithm always starts with a single node and moves through several adjacent nodes, in order to explore all of the connected edges along the way.

- The algorithm starts with an empty spanning tree.
- The idea is to maintain two sets of vertices. The first set contains the vertices already included in the MST, and the other set contains the vertices not yet included.
- At every step, it considers all the edges that connect the two sets and picks the minimum weight edge from these edges. After picking the edge, it moves the other endpoint of the edge to the set containing MST. 

Because it only ever compares crossing edges and always takes the smallest one, Prim's algorithm can never form a cycle. a vertex is only ever added once, so an edge to an already-included vertex is simply never selected.

### Kruskal's Algorithm
Kruskal's algorithm is also a Greedy algorithm, but unlike Prim's which grows a single tree outward from one starting node, Kruskal's algorithm looks at the whole graph at once and decides, edge by edge, whether to include it.

- The algorithm starts by sorting all edges in the graph in increasing order of weight.
- It treats every vertex as its own separate tree at first, tracked using a Union-Find (Disjoint Set Union) structure.
- It goes through the sorted edges one by one, adding an edge to the MST only if its two endpoints currently belong to different trees. If they already belong to the same tree, the edge is skipped.
- The algorithm stops once exactly (V − 1) edges have been added, where V is the number of vertices.

Because an edge is only ever added when its two endpoints sit in different trees, Kruskal's algorithm can never form a cycle. An edge that would connect two vertices already in the same tree is always rejected by the Union-Find check.

### Borůvka’s Algorithm
Borůvka’s algorithm is a Greedy algorithm like Prim's and Kruskal's algorithm. This algorithm starts with each vertex as an individual component and repeatedly connects each component using its cheapest outgoing edge.

The algorithm starts with an empty spanning tree, where each vertex is initially its own separate component.
At every step, each component finds the minimum weight edge that connects it to another component.
These minimum-weight edges are added to the MST, causing multiple components to merge together.
The process is repeated until all vertices belong to a single component.

Because each component always chooses its cheapest outgoing edge, Borůvka's algorithm follows a greedy approach. It also avoids cycles because edges that connect vertices already belonging to the same component are not selected.

## Prerequisites
For algorithms written in C:
- A C compiler
- No external libraries are required, the program only uses the C standard library
- Works on Linux, macOS, and Windows

## Instructions

### Prim's

1. Make sure prims.c and input.txt are in the same folder.

2. Compile:
``
gcc prim.c -o p
``

3. Run, passing the input file as an argument:
``
./p input.txt
``

4. To test with a different graph, edit input.txt (or point to another file) using the format:
```
VERTEX1 VERTEX2 WEIGHT
```
One edge per line. Vertices are single uppercase letters (A–Z). Lines starting with # are treated as comments and ignored.



## Results

### Prim's
<img width="510" height="216" alt="image" src="https://github.com/user-attachments/assets/905cb1cf-91fc-46de-8ad6-f41b2f076a76" />

<img width="340" height="306" alt="image" src="https://github.com/user-attachments/assets/58cc273b-eeb9-436f-8125-6a5009b8690c" />

The algorithm starts at vertex A (the first vertex encountered in the file) and grows the tree by always picking the cheapest edge leaving the current tree, arriving at a Minimum Spanning Tree of total weight 32













