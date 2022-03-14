import random
from venv import create

# generate a grid map for indoor navigation as a csv file

# let "s" represent the start point
# let "g" represent the goal point
# let "#" represent a wall/obstacle
# let "." represent a free space

# create a grid map with the given width and height which is enclosed by "#" and with a number of rooms with doors
# write the grid map to a csv file

id = 0


def generateIndoorMap(width, height, room_density):
    grid = [['.' for i in range(width)] for j in range(height)]

    # each of the 4 edges of the grid is a wall
    for i in range(height):
        grid[i][0] = '#,'
        grid[i][width - 1] = '#,'
    for j in range(width):
        grid[0][j] = '#,'
        grid[height - 1][j] = '#,'


    grid = createRoomsRecursively(room_density, 0.7, grid)

    return grid

def createRoomsRecursively(min_iterations, stop_chance ,grid):
    
    if stop_chance <= 0: # stop chance should be 0 < stop_chance < 1
        stop_chance = 0.05

    start_x = 1
    start_y = 1
    end_x = len(grid[0]) - 1
    end_y = len(grid) - 1

    return divideVertically(min_iterations, stop_chance, grid, start_x, start_y, end_x, end_y)


def divideVertically(min_iterations, stop_chance, grid, start_x , start_y , end_x , end_y):

    global id

    if min_iterations <= 0:
        if random.random() < stop_chance:
            # Fill up the room with room_number

            if start_x == end_x or start_y == end_y:
                return grid


            for i in range(start_y, end_y):
                for j in range(start_x, end_x):
                    if grid[i][j] == '.':
                        
                        # grid[i][j] = str(id).ljust(2)
                        grid[i][j] = str(id) + ','

            # place an 'r' in the center of the room
            grid[(start_y + end_y) // 2][(start_x + end_x) // 2] = 'r'+str(id)+','

            id += 1
            return grid

    # get the mid point between start_x and end_x
    
    width = end_x - start_x
    mid_x = (start_x + end_x) // 2
    
    # x is the mid point of the grid +- 0.25*width
    x = random.randint(mid_x - width // 4, mid_x + width // 4)


    # divide the grid vertically at x using a for loop
    for i in range(start_y, end_y):
        grid[i][x] = '#,'

    for i in range(min_iterations + 1):
        #cut out a hole in the wall
        y = random.randint(start_y+1, end_y-1)
        grid[y][x] = 'd,'

    # now divide each of the two subgrids recursively using divideHorizontally
    grid = divideHorizontally(min_iterations - 1, stop_chance, grid, start_x, start_y, x, end_y)
    grid = divideHorizontally(min_iterations - 1, stop_chance, grid, x, start_y, end_x, end_y)

    return grid

def divideHorizontally(min_iterations, stop_chance, grid, start_x , start_y , end_x , end_y):

    global id # please excuse the global variable

    if min_iterations <= 0:
        if random.random() < stop_chance:



            # Fill up the room with room_number
            for i in range(start_y , end_y):
                for j in range(start_x, end_x):
                    if grid[i][j] == '.':
                        
                        # grid[i][j] = str(id).ljust(2)
                        grid[i][j] = str(id) + ','
            

            # place an 'r' in the center of the room
            grid[(start_y + end_y) // 2][(start_x + end_x) // 2] = 'r'+str(id)+','
            
            id += 1

            return grid

    # get the mid point between start_y and end_y
    height = end_y - start_y
    mid_y = (start_y + end_y) // 2

    # y is the mid point of the grid +- 0.25*height
    y = random.randint(mid_y - height // 4, mid_y + height // 4)


    # divide the grid horizontally at y using a for loop
    for i in range(start_x, end_x):
        grid[y][i] = '#,'

    for i in range(min_iterations + 1):
        #cut out a hole in the wall
        x = random.randint(start_x+1, end_x-1)
        grid[y][x] = 'd,'

    # now divide each of the two subgrids recursively using divideVertically
    grid = divideVertically(min_iterations - 1, stop_chance, grid, start_x, start_y, end_x, y)
    grid = divideVertically(min_iterations - 1, stop_chance, grid, start_x, y, end_x, end_y)

    return grid

def main():

    width = 100
    height = 50
    room_density = 4

    grid = generateIndoorMap(width, height, room_density)
    for i in range(height):
        for j in range(width):
            print( str( grid[i][j]).ljust(3), end="")
        print()

    # write the grid map to a csv file

    csvFilePath = "graphFiles/indoorMap.csv"
    with open(csvFilePath, "w") as f:
        for i in range(height):
            for j in range(width):
                f.write(grid[i][j])
            f.write("\n")

    f.close()

if __name__ == "__main__":
    main()