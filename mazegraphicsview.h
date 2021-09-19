#ifndef MAZEGRAPHICSVIEW_H
#define MAZEGRAPHICSVIEW_H

#include <QGraphicsView>
#include "mazegraphicsscene.h"

class MazeGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MazeGraphicsView(QWidget *parent = nullptr);
    virtual ~MazeGraphicsView();

public slots:
    //!
    //! \brief buildMaze
    //!
    void buildMaze();
    //!
    //! \brief isBuilding
    //!
    void isBuilding(bool);
    //!
    //! \brief findWay
    //!
    void findWay();
signals:
    void building(bool);
    void done();
    void reseted();
private:
    static const int WINDOW_SIZE;
    static const int FPS;
    MazeGraphicsScene* m_scene;
    QTimer *timer;
};

#endif // MAZEGRAPHICSVIEW_H
