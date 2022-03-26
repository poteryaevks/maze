#include "mazeitem.h"
#include <QPainter>
#include <QDebug>

MazeItem::MazeItem(const QLineF &line, QGraphicsItem *parent)
    : QGraphicsLineItem(line, parent)
    ,m_bInit(false)
{
}

void MazeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setPen(QPen(Qt::black, 5));
    painter->drawLine(this->line());
}

void MazeItem::start()
{
    m_bInit = true;
}
