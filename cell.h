#ifndef CELL_H
#define CELL_H

using namespace std;

#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <string>
#include <QString>

class Cell {

public:
    Cell(int x, int y);
    Cell(int x, int y, bool wall);
    ~Cell();

    unsigned int getRow();
    unsigned int getCol();
    bool isVisited();
    bool isWall();
    void setVisited(bool visited);
    void setWall(bool wall);
    void setParent(Cell* parent);
    Cell* getParent();
    bool equal(Cell* other);
    QString toString();

protected:
    //overloaded functions

private:
    bool visited;
    bool is_wall;
    unsigned int rowIdx;
    unsigned int colIdx;
    Cell* parent;
};

#endif // CELL_H
