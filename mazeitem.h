#ifndef MAZEITEM_H
#define MAZEITEM_H

#include <QGraphicsItem>

class MazeItem : public QGraphicsLineItem
{
public:
    explicit MazeItem(const QLineF &line, QGraphicsItem *parent = nullptr);
    virtual ~MazeItem() override {}
    void start();
public slots:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
private:
    bool m_bInit;
};

#endif // MAZEITEM_H
