#include "maze.h"
#include "cell.h"

using namespace std;

#include <stack>
#include <queue>
#include <QLabel>
#include <QHBoxLayout>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QGraphicsItemAnimation>
#include <QTimeLine>
#include <QDebug>
#include <QPropertyAnimation>
#include <QApplication>
#include <QThread>
#include <QTimer>

Maze::Maze(QWidget *parent) : QWidget(parent)
{
    //sets the maze's background color to while
    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::white);
    this->setPalette(pal);
    this->setAutoFillBackground(true);

    i = 0;
    j = 0;
    ROW_LEN = 21;
    map_drawn = false;
    maze_drawn = false;
    finished_draw = false;
    path_drawn = false;
    explored_drawn = false;
    //allows the directions to be random
    srand(time(0));

}

//create an array fill with 2d cells
//animate the pop off
void Maze::generateMap(){

    for(int i = 0; i < ROW_LEN; i++){
        vector<Cell*> row;
        //row, col, 0 is the row, i is col
        for(int j = 0; j < ROW_LEN; j++){
            Cell* cell = new Cell(i,j);
            row.push_back(cell);
        }
        this->list.push_back(row);
    }

    this->c_start = this->list[1][1];
    this->c_end = this->list[this->list.size()-2][this->list.size()-2];
}

void Maze::generateMaze(){

    stack<Cell*> stack;
    stack.push(this->c_start);
    this->c_start->setVisited(true);
    this->c_start->setWall(false);

    //dfs starting with the start cell
    dfsHelper(this->c_start, this->c_start);

    for(auto row : this->list){
        for(auto cell : row){
            if(!cell->isVisited()){
                this->walls.push_back(cell);
                cell->setWall(true);
            }
        }
    }

    removeWalls();
}

void Maze::bfsPath(){

    resetVisited();
    this->path = {};
    queue<Cell*> queue;

    queue.push(this->c_start);
    this->c_start->setParent(nullptr);
    this->c_start->setVisited(true);

    while(!queue.empty()){
        Cell* curr = queue.front();
        queue.pop();

        if(curr == this->c_end){  
            Cell* prev = curr->getParent();
            while(prev->getParent() != nullptr){
                this->path.push_back(prev);
                prev = prev->getParent();
            } 
            return;
        }

        vector<Cell*> neighbors = validNeighbors(curr);
        for(auto cell : neighbors){
            if(!cell->isVisited()){
                this->explored.push_back(cell);
                cell->setVisited(true);
                cell->setParent(curr);
                queue.push(cell);
            }
        }
    }

}

void Maze::dfsPath(){

    resetVisited();
    this->path = {};
    stack<Cell*> stack;

    stack.push(this->c_start);
    this->c_start->setParent(nullptr);
    this->c_start->setVisited(true);

    while(!stack.empty()){
        Cell* curr = stack.top();
        stack.pop();
        this->explored.push_back(curr);
        if(curr == this->c_end){
            Cell* prev = curr->getParent();
            while(prev->getParent() != nullptr){
                this->path.push_back(prev);
                prev = prev->getParent();
            }
            return;
        }

        vector<Cell*> neighbors = validNeighbors(curr);
        for(auto cell : neighbors){
            if(!cell->isVisited()){
                cell->setVisited(true);
                cell->setParent(curr);
                stack.push(cell);
            }
        }
    }

}

vector<Cell*> Maze::validNeighbors(Cell* curr){
    vector<Cell*> neighbors;
    vector<pair<int,int>> directions = {{0,1}, {1,0}, {0,-1}, {-1,0}};
    random_shuffle(directions.begin(), directions.end());
    int nRow, nCol;
    Cell* nextCell;
    for(auto dir : directions){
        nRow = curr->getRow() + dir.first;
        nCol = curr->getCol() + dir.second;
        if(posInMap(nRow, nCol)){
            nextCell = this->list[nRow][nCol];
            if(!nextCell->isWall()){
                neighbors.push_back(nextCell);
            }
        }
    }
    return neighbors;
}

void Maze::dfsHelper(Cell* curr1, Cell* curr2){
    curr1->setVisited(true);
    curr2->setVisited(true);

    this->visited.push_back(curr1);
    this->visited.push_back(curr2);

    for(auto pair : neighborCells(curr2)){
        if(!pair.second->isVisited()){
            dfsHelper(pair.first, pair.second);
        }
    }
}

void Maze::resetVisited(){
    for(auto row : this->list){
        for(auto cell: row){
            cell->setVisited(false);
        }
    }
}

void Maze::removeWalls() {
    int randIdx;
    while(this->walls_open.size() < ROW_LEN * ROW_LEN / 50){
        randIdx = rand() % this->walls.size();
        Cell* remove = this->walls[randIdx];
        if(isWallCorner(remove) && !isWallEdge(remove)){
            this->walls_open.push_back(remove);
            remove->setWall(false);
        }
    }
}

vector<pair<Cell*, Cell*>> Maze::neighborCells(Cell *curr){
    vector<pair<Cell*, Cell*>> neighbors;
    vector<pair<int,int>> directions{{2,0}, {0,2}, {0,-2}, {-2,0}};
    random_shuffle(directions.begin(), directions.end());
    unsigned int nextRow, nextCol;
    pair<int,int> prev;
    for(auto pair : directions){
        nextRow = curr->getRow()+pair.first;
        nextCol = curr->getCol()+pair.second;
        if(posInMap(nextRow, nextCol)){
            prev = getPrevDirection(pair);
            Cell* cell = this->list[nextRow][nextCol];
            Cell* cellBefore = this->list[curr->getRow()+prev.first][curr->getCol()+prev.second];
            neighbors.push_back(make_pair(cellBefore,cell));
        }
    }
    return neighbors;
}

pair<int,int> Maze::getPrevDirection(pair<int,int> pair){
    std::pair<int, int> dir_before;
    if(pair.first == 2 && pair.second == 0){
        dir_before.first = 1;
        dir_before.second = 0;
    }
    else if(pair.first == -2 && pair.second == 0){
        dir_before.first = -1;
        dir_before.second = 0;
    }
    else if(pair.first == 0 && pair.second == 2){
        dir_before.first = 0;
        dir_before.second = 1;
    }
    else if(pair.first == 0 && pair.second == -2){
        dir_before.first = 0;
        dir_before.second = -1;
    }
    return dir_before;
}

bool Maze::posInMap(unsigned int row, unsigned int col){
     return row >= 0 && row < this->list.size() && col >= 0 && col < this->list[0].size();
}

bool Maze::isWallEdge(Cell *wall){
    return  wall->getRow() == 0 || wall->getRow() == this->list.size()-1 ||
            wall->getCol() == 0 || wall->getCol() == this->list[0].size()-1;
}

bool Maze::isWallCorner(Cell *wall){
    vector<pair<int,int>> directions1{{1,0},{-1,0}};
    vector<pair<int,int>> directions2{{0,1},{0,-1}};
    int nRow, nCol;
    int countV = 0;
    int countH = 0;
    for(auto pair : directions1){
        nRow = wall->getRow()+pair.first;
        nCol = wall->getCol()+pair.second;
        if(posInMap(nRow, nCol) && !this->list[nRow][nCol]->isVisited()){
            countV++;
        }
    }
    for(auto pair : directions2){
        nRow = wall->getRow()+pair.first;
        nCol = wall->getCol()+pair.second;
        if(posInMap(nRow, nCol) && !this->list[nRow][nCol]->isVisited()){
            countH++;
        }
    }
    return (countH == 2 && countV == 0) ||( countV == 2 && countH == 0);
}

void Maze::print(){

}

void Maze::addRect() {
    //handle case when list is empty
    for(auto cell : this->list[i]){
        this->draw_cells.push_back(cell);
    }
    update();
    ++i;//increment counter

    //if(j < this->list[0].size()) QTimer::singleShot(0,this,SLOT(addRect()));
    if(i < this->list.size()){
        QTimer::singleShot(0,this,SLOT(addRect()));
    }
    else {
        //call dfs here
        drawVisited();
        return;
    }
}

void Maze::drawVisited(){
   if(!this->map_drawn){
       this->map_drawn = true;
       i = 0;
       j = 0;
   }

   this->draw_visited_list.push_back(this->visited[i]);
   update();
   ++i;//increment counter

   if(i < this->visited.size()) QTimer::singleShot(0,this,SLOT(drawVisited()));
   else drawWalls();
}

void Maze::drawWalls(){
   if(!this->maze_drawn){
       this->maze_drawn = true;
       i = 0;
   }
   if(this->walls_open.size() == 0 ){
       this->finished_draw = true;
       return;
   }

   this->draw_walls.push_back(this->walls_open[i]);
   update();
   ++i;//increment counter

   if(i < this->walls_open.size()) QTimer::singleShot(0,this,SLOT(drawWalls()));
   else this->finished_draw = true;
}

void Maze::drawExplore(){
   if(!this->explored_drawn){
       this->explored_drawn = true;
       i = 0;
   }
   if(this->explored.size() == 0 ){
       drawPath();
       return;
   }

   this->draw_explored.push_back(this->explored[i]);
   update();
   ++i;//increment counter

   if(i < this->explored.size()) QTimer::singleShot(50/ROW_LEN,this,SLOT(drawExplore()));
   else drawPath();
}

void Maze::drawPath(){
   if(!this->path_drawn){
       this->path_drawn = true;
       i = 0;
   }
   if(this->path.size() == 0 ){
       this->finished_path = true;
       return;
   }

   this->draw_path.push_back(this->path[i]);
   update();
   ++i;//increment counter

   if(i < this->path.size()) QTimer::singleShot(20,this,SLOT(drawPath()));
   else this->finished_path = true;

}

void Maze::paintEvent(QPaintEvent * event) {
    Q_UNUSED(event);

    QPainter painter(this);

    QPen red(Qt::red);
    QPen norm(Qt::black);
    QBrush wall(Qt::black);
    QBrush space(Qt::white);
    QBrush start(Qt::blue);
    QBrush end(Qt::green);
    QBrush explored(Qt::yellow);
    QBrush path(Qt::red);

    double rsize = double(MAZE_SIZE) / ROW_LEN;

    for(auto cell : this->draw_cells){
        //x is the col, y is the row
        painter.setBrush(wall);
        painter.drawRect(rsize*cell->getCol(),rsize*cell->getRow(),rsize,rsize);
    }
    for(auto cell : this->draw_visited_list){
        painter.setPen(norm);
        painter.setBrush(start);
        painter.drawRect(rsize*this->c_start->getCol(),rsize*this->c_start->getRow(),rsize,rsize);
        painter.setBrush(space);
        QRectF sq(rsize*cell->getCol(),rsize*cell->getRow(),rsize,rsize);
        painter.drawRect(sq);
        if(cell == this->c_end){
            painter.setBrush(end);
            painter.drawRect(rsize*this->c_end->getCol(),rsize*this->c_end->getRow(),rsize,rsize);
        }
    }
    for(auto cell: this->draw_walls){
        painter.setBrush(space);
        QRectF sq(rsize*cell->getCol(),rsize*cell->getRow(),rsize,rsize);
        painter.drawRect(sq);
    }
    for(auto cell: this->draw_explored){
        if(cell != this->c_start && cell != this->c_end){
            painter.setPen(norm);
            painter.setBrush(explored);
            QRectF sq(rsize*cell->getCol(),rsize*cell->getRow(),rsize,rsize);
            painter.drawRect(sq);
        }
    }
    for(auto cell: this->draw_path){
        if(cell != this->c_start && cell != this->c_end){
            painter.setPen(norm);
            painter.setBrush(path);
            QRectF sq(rsize*cell->getCol(),rsize*cell->getRow(),rsize,rsize);
            painter.drawRect(sq);
        }
    }
}

Maze::~Maze(){
    for(auto row : this->list){
        for(auto cell : row){
            delete cell;
        }
    }
    qInfo() << "freed the cell list";
}

void Maze::reset(){
    for(auto row : this->list){
        for(auto cell : row){
            delete cell;
        }
    }

    this->list = {};
    i = 0;
    j = 0;

    map_drawn = false;
    maze_drawn = false;
    finished_draw = false;

    srand(time(0));

    this->c_start = nullptr;
    this->c_end = nullptr;
    this->draw_cells = {};
    this->draw_walls = {};
    this->draw_visited_list = {};

    this->visited = {};
    this->walls = {};
    this->walls_open = {};

    resetExplore();
}

void Maze::resetExplore(){
    i = 0;
    j = 0;
    //
    this->draw_path = {};
    this->draw_explored = {};
    this->path = {};
    this->explored = {};

    finished_path = false;
    path_drawn = false;
    explored_drawn = false;
}

void Maze::freeCellList(vector<Cell*> list){
    for(auto cell : list){
        delete cell;
    }
}
bool Maze::finishedMap(){
    return this->finished_draw;
}

bool Maze::finishedPath(){
    return this->finished_path;
}

void Maze::setFinishedPath(bool path){
    this->finished_path = path;
}

void Maze::setRowLen(int rowLen){
    if(rowLen % 2 == 0){
       ROW_LEN = rowLen+1;
    }
    else {
        ROW_LEN = rowLen;
    }
}
maze_map Maze::getList(){
    return this->list;
}
