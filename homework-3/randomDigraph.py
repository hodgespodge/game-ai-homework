# Create a random weighted digraph with n nodes and m edges

# Use a adjacency list representation

import random


# Create a graph with n nodes and m edges
# Let there only be at most one edge between any two nodes
# Output the graph as a list of edges
def digraph(n, m, min_weight=1, max_weight=10, allow_self_loops=False):

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
    return edges_with_costs


def main():

    allow_self_loops = False

    n = int(input("Enter number of nodes: "))
    
    #parse boolean input
    response = input("Allow self loops? (y/n): ").strip().capitalize()
    if response == "Y" or response == "YES":
        allow_self_loops = True

    if allow_self_loops:
        print("Max number of edges allowed: ",(int) (n + (n*(n-1))))
    else:
        print("Max number of edges allowed: ",(int) (n*(n-1)))

    m = int(input("Enter number of edges: "))

    min_weight = 1
    max_weight = 10
    outputCSV = "digraph.csv"

    edges = digraph(n, m, min_weight, max_weight, allow_self_loops)

    # write edges to a csv file
    with open(outputCSV, "w") as f:
        for edge in edges:
            f.write(str(edge[0]) + "," + str(edge[1]) + "," + str(edge[2]) + "\n")

    f.close()


if __name__ == '__main__':
    main()