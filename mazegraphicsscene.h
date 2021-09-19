#ifndef MAZEGRAPHICSSCENE_H
#define MAZEGRAPHICSSCENE_H

#include "mazeitem.h"
#include "mazegenerator.h"

#include <future>
#include <thread>
#include <memory>
#include <QGraphicsScene>
#include <QGraphicsRectItem>

class MazeGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit MazeGraphicsScene(QObject *parent = nullptr);
    virtual ~MazeGraphicsScene();
    //!
    //! \brief findAndRemove
    //!
    void findAndRemove(QLineF);
    //!
    //! \brief addLine
    //! \param x1
    //! \param y1
    //! \param x2
    //! \param y2
    //! \param pen
    //! \return
    //!
    QGraphicsLineItem *addLine(qreal x1, qreal y1, qreal x2, qreal y2, const QPen &pen = QPen());
    //!
    //! \brief addRect
    //! \param x
    //! \param y
    //! \param w
    //! \param h
    //! \param pen
    //! \param brush
    //! \return
    //!
    QGraphicsRectItem *addRect(qreal x, qreal y, qreal w, qreal h, const QPen &pen = QPen(), const QBrush &brush = QBrush());
signals:
    void building(bool);
    void done();
    void reseted();
public slots:
    void run();
    void reset();
    void newFrame();
    void findWayExec();
private:
    //!
    //! \brief drawLines
    //! \param x
    //! \param y
    //!
    void drawMazeCell(qreal x, qreal y);
    //!
    //! \brief drawGrid - рисует сетку
    //!
    void drawGrid();
    //!
    //! \brief getNumber
    //! \param x
    //! \param y
    //! \return
    //!
    int getNumber(qreal x, qreal y);
    //!
    //! \brief getCoordinateByNumber
    //! \param number
    //! \return
    //!
    QPoint getCoordinateByNumber(int number);
    //!
    //! \brief getLineToDestroy
    //! \param pair
    //! \return
    //!
    QLineF getLineToDestroy(const std::pair<int, int> &pair);
    //!
    //! \brief findLine
    //! \param lineToFind
    //! \param items
    //! \return
    //!
    QList<QGraphicsItem*> :: iterator findLine(const QLineF &lineToFind, QList<QGraphicsItem*>& items);
    //!
    //! \brief findRect
    //! \param rectToFind
    //! \param items
    //! \return
    //!
    QList<QGraphicsItem *>::iterator findRect(const QRectF &rectToFind, QList<QGraphicsItem *> &items);
    struct graph_node {
        graph_node(int number)
            : m_number(number) {}
        int m_number; //номер - это первый и последний индекс элемента матрицы
        int m_weight;
    };
    //!
    //! \brief findWay
    //! \param g
    //! \param start
    //! \param finish
    //! \return
    //!
    std::stack<int> findWay(std::list<std::list<graph_node>> &g, int start, int finish);
private:
    static const int DX;
    static const int DY;
    static const int WALL;
    static const int STEP_RECT;
    static const int X_NUM;
    static const int Y_NUM;
    //!
    std::unique_ptr<MazeGenerator> m_mazeGenerator;
    QList<QGraphicsItem*> m_rects;
    std::vector<std::pair<int, int>> ::iterator m_iter;
    std::stack<int> m_way;
    std::future<void> m_future;
};

#endif // MAZEGRAPHICSSCENE_H
