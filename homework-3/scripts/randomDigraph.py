# Create a random weighted digraph with n nodes and m edges

# Use a adjacency list representation

from math import ceil
import random

from scipy import rand

# Create a graph with n nodes and m edges
# Let there only be at most one edge between any two nodes
# Output the graph as a list of edges
def randomDigraph(n, m, min_weight=1, max_weight=10, allow_self_loops=False, x_range=100, y_range=100):

    # check if number of nodes and edges are valid
    if n < 1 or m < 0 :
        
        print("Invalid input")
        return

    if allow_self_loops:
        if m > n+ n*(n-1):
            print("Too many edges")
            return

    elif m > n*(n-1):
        print("Too many edges")
        return


    # Create a list of n nodes
    nodes = [i for i in range(n)]

    # Assign random coordinates to each node
    nodes_with_coords = [(random.randint(0,x_range), random.randint(0,y_range)) for _ in range(n)]

    # Create a set of edges (without costs)
    edges = set()

    while len(edges) < m:
        # pick two random nodes
        node1 = random.choice(nodes)
        node2 = random.choice(nodes)
        if not allow_self_loops and node1 == node2:
            continue

        edges.add((node1, node2))

    edges_with_costs = []
    # For each edge, pick a random cost
    for edge in edges:
        cost = random.randint(min_weight, max_weight)
        edges_with_costs.append((edge[0], edge[1], cost))

    # Return the list of edges
    return edges_with_costs, nodes_with_coords
