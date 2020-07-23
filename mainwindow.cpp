#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setStyleSheet("background-color: aliceblue;");
    ui->start->setStyleSheet("background-color: salmon;");
    ui->reset_map->setStyleSheet("background-color: lightsteelblue;");
    ui->selection->setStyleSheet("background-color: lightgray;");

    ui->size->setMinimum(10);
    ui->size->setMaximum(50);
    ui->size->setValue(20);

    ui->mazeWidget->setRowLen(ui->size->value());
    ui->mazeWidget->generateMap();
    ui->mazeWidget->generateMaze();
    ui->mazeWidget->addRect();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_reset_map_clicked()
{
    qInfo() << "reset map pressed";
    if(ui->mazeWidget->finishedMap()){
        ui->mazeWidget->reset();
        ui->mazeWidget->setRowLen(ui->size->value());
        ui->mazeWidget->generateMap();
        ui->mazeWidget->generateMaze();
        ui->mazeWidget->addRect();
        ui->mazeWidget->setFinishedPath(true);
    }
}

void MainWindow::on_start_clicked()
{
    qInfo() << "start visualization pressed";
    if(ui->mazeWidget->finishedMap() && ui->mazeWidget->finishedPath()){
        ui->mazeWidget->resetExplore();
        if(ui->selection->currentText() == "DFS"){
            ui->mazeWidget->dfsPath();
        }
        else if (ui->selection->currentText() == "BFS"){
            ui->mazeWidget->bfsPath();
        }
        else {
            return;
        }
        ui->mazeWidget->drawExplore();
    }
}
