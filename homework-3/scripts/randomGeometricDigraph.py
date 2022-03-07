
from math import ceil, sqrt, floor
import multiprocessing as mp
import random

nodes_with_coords = []
percent_symmetric = 0.5
max_edge_length = 0

def build_edges(possible_edges):
    edges_with_costs = []

    print("Process: ", mp.current_process().name, " started")

    num_edges = len(possible_edges)
    is_printer = mp.current_process().name == "ForkPoolWorker-1"

    # pre-process the pointers to speed up calculations
    edge_append = edges_with_costs.append

    for i, edge in enumerate(possible_edges):

        if is_printer:
            print((i / num_edges) * 100, "%", end="\r")

        node1 = nodes_with_coords[edge[0]]
        node2 = nodes_with_coords[edge[1]]

        # check if the distance between the two nodes is smaller than the radius
        distance = ceil( sqrt(((node1[0]-node2[0])**2 + (node1[1]-node2[1])**2) ) )

        if distance <= max_edge_length:

            # set the cost of the edge to the distance
            if random.random() < percent_symmetric:
                edge_append((edge[0], edge[1], distance))
                edge_append((edge[1], edge[0], distance))
            else:
                # 50/50 which direction the edge is going
                if random.random() < 0.5:
                    edge_append((edge[0],edge[1], distance))
                else:
                    edge_append((edge[1],edge[0], distance))

    return edges_with_costs


def geometricDigraph(n, edge_radius, per_sym, x_range=1000, y_range=1000):

    global nodes_with_coords
    global percent_symmetric
    global max_edge_length

    percent_symmetric = per_sym

    # check if number of nodes and edges are valid
    if n < 1:
        print("Invalid input")
        return

    # if edge_radius = 1, an edge in one corner will be able to connect to its opposite diagonal corner
    max_edge_length = floor (edge_radius * sqrt(x_range**2 + y_range**2))

    print("max edge length",max_edge_length)

    # Assign random coordinates to each node
    nodes_with_coords = [(0,0)]
    nodes_with_coords.extend([(random.randint(0,x_range), random.randint(0,y_range)) for _ in range(n-2)])
    nodes_with_coords.append((x_range-1,y_range-1))

    edges_with_costs = []

    possible_edges = []

    for i in range(n):
        for j in range(i+1, n):
            possible_edges.append((i, j))

    
    processes = []
    num_processors = mp.cpu_count() -1
    
    # Use a multiprocessing pool to parallely check if each edge in possible_edges is valid
    with mp.Pool(num_processors) as pool:
        results = pool.map(build_edges, [possible_edges[i::num_processors] for i in range(num_processors)])

        for result in results:
            edges_with_costs.extend(result)

    return edges_with_costs, nodes_with_coords