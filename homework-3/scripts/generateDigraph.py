from randomDigraph import *
from randomGeometricDigraph import *


def main():

    is_geometric = False
    #parse boolean input
    response = input("Coorelate edge weights to (x,y) coords? (y/n): ").strip().capitalize()
    if response == "Y" or response == "YES":
        is_geometric = True
        
        edge_radius = float( input("Enter max edge length between 0 to 1. (0 is no edges, 1 is a complete graph): "))
        if edge_radius < 0 or edge_radius > 1:
            print("Got invalid edge radius!\nSetting edge radius to 0.2")
            edge_radius = 0.2

        response = input("Is the graph directed? (y/n): ").strip().capitalize()
        if response == "Y" or response == "YES":
            percent_symettric = float( input("What proportion of edges should be symettric (0 to 1): "))
            if percent_symettric < 0 or percent_symettric > 1:
                print("Got invalid percent symettric!\nSetting percent symettric to 0.5")
                percent_symettric = 0.5
        else:
            percent_symettric = 1.0 # undirected graph is inherently symmetric

    # Note: for geometric graph, 10000 nodes uses about 24GB of RAM
    # 5000 nodes uses about 9 GB of RAM
    # 7500 nodes uses about 16 GB of RAM
    n = int(input("Enter number of nodes: "))

    if is_geometric and n > 5000:
        print("Warning: A large number of nodes consume a lot of memory when generating graph!")
        print("Approximately" ,0.003*n - 6, "GB of RAM will be used")

    allow_self_loops = False
    #parse boolean input

    if not is_geometric:
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

    edgesCSV = "graphFiles/edges.csv"
    nodesCSV = "graphFiles/nodes.csv"

    if is_geometric:
        x_range=1000
        y_range=1000
        edges_with_costs, nodes_with_coords = geometricDigraph(n, edge_radius, percent_symettric , x_range, y_range)
    else:
        min_weight = 1
        max_weight = ceil( 1 + m/n)
        edges_with_costs, nodes_with_coords = randomDigraph(n, m, min_weight, max_weight, allow_self_loops)

    print("Writing edges to file...")

    # write edges to a csv file
    with open(edgesCSV, "w") as f:
        for edge in edges_with_costs:
            f.write(str(edge[0]) + "," + str(edge[1]) + "," + str(edge[2]) + "\n")
    f.close()

    with open(nodesCSV, "w") as f:
        for i,node in enumerate(nodes_with_coords):
            f.write(str(i) + "," + str(node[0]) + "," + str(node[1]) + "\n")
    f.close()

    print("Done writing to file!")

if __name__ == '__main__':
    main()