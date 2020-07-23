#include "cell.h"

Cell::Cell(int row, int col){
    this->rowIdx = row;
    this->colIdx = col;
    this->visited = false;
    this->is_wall = false;
}

Cell::~Cell() {

}

unsigned int Cell::getRow(){
    return this->rowIdx;
}

unsigned int Cell::getCol(){
    return this->colIdx;
}

bool Cell::isVisited(){
    return this->visited;
}

bool Cell::isWall(){
    return this->is_wall;
}

void Cell::setVisited(bool visited){
    this->visited = visited;
}

void Cell::setWall(bool wall){
    this->is_wall = wall;
}

void Cell::setParent(Cell* parent){
    this->parent = parent;
}

Cell* Cell::getParent(){
    return this->parent;
}

bool Cell::equal(Cell* other){
    return (this->rowIdx == other->rowIdx) && (this->colIdx == other->colIdx);
}

QString Cell::toString(){
    string cell_str = "(" + to_string(this->rowIdx) + ", " + to_string(this->colIdx) + ")";
    return QString::fromStdString(cell_str);
}
