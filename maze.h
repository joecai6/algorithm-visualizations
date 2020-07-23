#ifndef MAZE_H
#define MAZE_H

using namespace std;

#include "cell.h"
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPropertyAnimation>
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>

typedef vector<vector<Cell*>> maze_map;

const int MAZE_SIZE = 600;

class Maze : public QWidget {

    Q_OBJECT

    public:  
        Maze(QWidget *parent = nullptr);
        ~Maze();

        void display();
        void paintEvent(QPaintEvent * event) override;
        void generateMap();
        void generateMaze();
        void bfsPath();
        void dfsPath();
        vector<Cell*> validNeighbors(Cell* curr);
        vector<pair<Cell*, Cell*>> neighborCells(Cell* curr);
        bool posInMap(unsigned int row, unsigned int col);
        void dfsHelper(Cell* curr1, Cell* curr2);
        void removeWalls();
        bool isWallEdge(Cell* wall);
        bool isWallCorner(Cell* wall);
        pair<int,int> getPrevDirection(pair<int,int> currDir);
        void reset();
        void resetExplore();
        void resetVisited();
        bool finishedMap();
        bool finishedPath();
        void setFinishedPath(bool path);
        void freeCellList(vector<Cell*> list);
        maze_map getList();

        void setRowLen(int rowLen);

    public slots:
        void addRect();
        void print();
        void drawVisited();
        void drawWalls();
        void drawExplore();
        void drawPath();

    private:
        int ROW_LEN;

        //fields for drawing
        vector<Cell*> draw_cells;
        vector<Cell*> draw_visited_list;
        vector<Cell*> draw_walls;
        vector<Cell*> draw_explored;
        vector<Cell*> draw_path;

        unsigned int i, j;
        bool map_drawn;
        bool maze_drawn;
        bool finished_draw;
        bool finished_path;
        bool explored_drawn;
        bool path_drawn;

        Cell* c_start;
        Cell* c_end;
        maze_map list;
        vector<Cell*> visited;
        vector<Cell*> walls;
        vector<Cell*> walls_open;
        vector<Cell*> explored;
        vector<Cell*> path;
};
#endif // MAZE_H
