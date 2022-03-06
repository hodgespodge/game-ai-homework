from randomDigraph import *
from randomGeometricDigraph import *


def main():

    allow_self_loops = False

    n = int(input("Enter number of nodes: "))
    
    #parse boolean input
    response = input("Allow self loops? (y/n): ").strip().capitalize()
    if response == "Y" or response == "YES":
        allow_self_loops = True

    max_edges = 0

    if allow_self_loops:
        max_edges = n + n*(n-1)
        print("Max number of edges allowed: ",(int) (max_edges))
    else:
        max_edges = n*(n-1)
        print("Max number of edges allowed: ",(int) (max_edges))

    m = int(input("Enter number of edges: "))

    if m > max_edges:
        print("Got too many edges!\nSetting number of edges to max allowed.")
        m = max_edges

    print("")

    min_weight = 1
    max_weight = ceil( 1 + m/n)
    edgesCSV = "graphFiles/edges.csv"
    nodesCSV = "graphFiles/nodes.csv"
    # numNodesCSV = "graphFiles/numNodes.csv"

    edges_with_costs, nodes_with_coords = randomDigraph(n, m, min_weight, max_weight, allow_self_loops)
    # edges = geometricDigraph(n, m, min_weight, max_weight, allow_self_loops)

    # write edges to a csv file
    with open(edgesCSV, "w") as f:
        for edge in edges_with_costs:
            f.write(str(edge[0]) + "," + str(edge[1]) + "," + str(edge[2]) + "\n")
    f.close()

    with open(nodesCSV, "w") as f:
        for i,node in enumerate(nodes_with_coords):
            f.write(str(i) + "," + str(node[0]) + "," + str(node[1]) + "\n")
    f.close()

    # with open(numNodesCSV, "w") as f:
    #     f.write(str(n))
    # f.close()

if __name__ == '__main__':
    main()