# Pathfinding Algorithm Visualizer

The visualization involves randomly generating a maze to be solved with pathfinding algorithms such as BFS, DFS, A* search, and Greedy Best First Search. The functions were implemented through C++ and the graphic interfaces were implemented through Qt.

# Generating the Maze

The maze is randomly generated using DFS by stepping 2 units every iteration and randomly choosing a neighbor to step to. With the interface of Qt, a slider is displayed to allow the user to adjust the map size from 20x20 to 50x50. When the user click on the "Reset Map" button, the user can see the animation of the map being generated.

<img src="/img/maze_gen.gif"/>

# Solving the Maze

THe user can select the algorithm they want to use to solve the maze. There are four choices of pathfinding algorithms: BFS, DFS, A*, and Greedy BFS. When the user clicks "Start", the user can see the cells that are being visited. The yellow cell hopes to reach the end goal, which is represented by the green cell. When an algorithm reachs the end cell, the path will backtrack to the start to form the path.

<img src="/img/bfs_ex.gif"/>

<img src="/img/a_star_ex.gif"/>
