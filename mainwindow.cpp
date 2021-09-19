#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
  ,ui(new Ui::MainWindow)
  ,m_graphView(new MazeGraphicsView(this))
{
    ui->setupUi(this);
    ui->findWayButton->setEnabled(false);
    ui->sceneLayout->addWidget(m_graphView);

    connect(m_graphView, &MazeGraphicsView::building, this, [this](bool state){
        ui->buildButton->setEnabled(!state);
    });

    connect(m_graphView, &MazeGraphicsView::done, this, [this](){
        ui->findWayButton->setEnabled(true);
    });

    connect(m_graphView, &MazeGraphicsView::reseted, this, [this](){
        ui->findWayButton->setEnabled(false);
    });

    connect(m_graphView, &MazeGraphicsView::reseted, this, [this](){
        ui->findWayButton->setEnabled(false);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_buildButton_released()
{
    m_graphView->buildMaze();
}

void MainWindow::on_findWayButton_released()
{
    m_graphView->findWay();
}
