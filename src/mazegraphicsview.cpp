#include "mazegraphicsview.h"

#include <QTimer>
#include <QEvent>
#include <QDebug>
#include <thread>

const int MazeGraphicsView::WINDOW_SIZE = 400;
const int MazeGraphicsView::FPS = 20;

MazeGraphicsView::MazeGraphicsView(QWidget *parent)
    : QGraphicsView(parent)
{
    m_scene = new MazeGraphicsScene(this);
    connect(m_scene, SIGNAL(building(bool)), this, SLOT(isBuilding(bool)));
    connect(m_scene, &MazeGraphicsScene::done, this, [this](){
        this->done();
    });
    connect(m_scene, &MazeGraphicsScene::reseted, this, [this](){
        this->reseted();
    });
    //! Немного поднастроим отображение виджета и его содержимого
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);      //! Отключим скроллбар по горизонтали
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);        //! Отключим скроллбар по вертикали
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed); //! Растягиваем содержимое по виджету
    //! Также зададим минимальные размеры виджета
    this->setMinimumHeight(WINDOW_SIZE);
    this->setMinimumWidth(WINDOW_SIZE);
    this->setMaximumHeight(WINDOW_SIZE);
    this->setMaximumWidth(WINDOW_SIZE);
    //!
    this->setScene(m_scene);
    //!
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), m_scene, SLOT(newFrame()));
    timer->start(FPS);
}

MazeGraphicsView::~MazeGraphicsView()
{

}

void MazeGraphicsView::isBuilding(bool state)
{
    building(state);
}

void MazeGraphicsView::findWay()
{
    m_scene->findWayExec();
}

void MazeGraphicsView::buildMaze()
{
    m_scene->reset();
    m_scene->run();
}

