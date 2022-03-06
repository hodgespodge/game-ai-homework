def geometricDigraph(n, m, min_weight=1, max_weight=10, allow_self_loops=False, x_range=100, y_range=100):

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

    