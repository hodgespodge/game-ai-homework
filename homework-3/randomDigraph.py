# Create a random weighted digraph with n nodes and m edges

# Use a adjacency list representation

import random


# Create a graph with n nodes and m edges
# Let there only be at most one edge between any two nodes
# Output the graph as a list of edges
def digraph(n, m, min_weight=1, max_weight=10):

    # check if number of nodes and edges are valid
    if n < 1 or m < 0 :
        
        print("Invalid input")
        return

    if m > n*(n-1)/2:
        print("Too many edges for the number of nodes")
        return


    # Create a list of n nodes
    nodes = [i for i in range(n)]

    # Create a set of edges
    edges = set()

    while len(edges) < m:
        # pick two random nodes
        node1 = random.choice(nodes)
        node2 = random.choice(nodes)

        edges.add((node1, node2, random.randint(min_weight, max_weight)))

    # Return the list of edges
    return edges


def main():
    n = int(input("Enter number of nodes: "))
    print("Max number of edges allowed: ",(int) (n*(n-1)/2))

    m = int(input("Enter number of edges: "))

    min_weight = 1
    max_weight = 10
    outputCSV = "digraph.csv"

    edges = digraph(n, m, min_weight, max_weight)

    # write edges to a csv file
    with open(outputCSV, "w") as f:
        for edge in edges:
            f.write(str(edge[0]) + "," + str(edge[1]) + "," + str(edge[2]) + "\n")


if __name__ == '__main__':
    main()