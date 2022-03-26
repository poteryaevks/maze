#include "mazegraphicsscene.h"

#include <queue>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>

const int MazeGraphicsScene::DX = 20;
const int MazeGraphicsScene::DY = 20;
const int MazeGraphicsScene::STEP_RECT = 5;
const int MazeGraphicsScene::WALL = 1;
const int MazeGraphicsScene::X_NUM = 20;
const int MazeGraphicsScene::Y_NUM = 20;

MazeGraphicsScene::MazeGraphicsScene(QObject *parent)
    : QGraphicsScene(parent)
{
    m_mazeGenerator = std::make_unique<MazeGenerator>(X_NUM * Y_NUM, X_NUM);
    //!
    drawGrid();
    building(false);
}

MazeGraphicsScene::~MazeGraphicsScene()
{
}

void MazeGraphicsScene::reset()
{
    clear();
    drawGrid();
    reseted();
}

void MazeGraphicsScene::findAndRemove(QLineF line)
{
    auto items = this->items();
    auto iter = findLine(line, items);
    if(iter != items.end()) {
        //! если есть - стереть
        QGraphicsScene::removeItem(*iter);
    }
}

void MazeGraphicsScene::drawMazeCell(qreal x, qreal y)
{
    addLine(x, y, x + DX, y, QPen(Qt::darkRed, WALL));
    addLine(x, y, x, y + DY, QPen(Qt::darkRed, WALL));
}

QGraphicsRectItem* MazeGraphicsScene::addRect(qreal x, qreal y, qreal w, qreal h, const QPen &pen, const QBrush &brush)
{
    //QRectF rect(x, y, w, h);
    //auto iter = findRect(rect, m_rects);
    //if(iter != m_rects.end()) {
    //    //! если  уже есть - стереть и не рисовать
    //    delete (*iter);
    //    m_rects.erase(iter);
    //    return nullptr;
    //}
    //else{
    return QGraphicsScene::addRect(x, y, w, h, pen, brush);
    //     m_rects << newItem;
    //     return newItem;
    // }
}

QGraphicsLineItem* MazeGraphicsScene::addLine(qreal x1, qreal y1, qreal x2, qreal y2, const QPen& pen)
{
    //    QLineF line(x1, y1, x2, y2);
    //    auto items = this->items();
    //    auto iter = findLine(line, items);
    //    if(iter != items.end()) {
    //        //! если линия уже есть - стереть и не рисовать
    //        delete (*iter);
    //        items.erase(iter);
    //        return nullptr;
    //    }
    //    else
    return QGraphicsScene::addLine(x1, y1, x2, y2, pen);
}

QList<QGraphicsItem*> :: iterator MazeGraphicsScene::findLine(const QLineF& lineToFind, QList<QGraphicsItem*>& items)
{
    auto iter = items.begin();
    for (; iter != items.end(); ++iter) {
        QGraphicsLineItem* item = dynamic_cast<QGraphicsLineItem*>(*iter);
        if(item != nullptr){
            if(item->line() == lineToFind)
                break;
        }
    }
    return iter;
}

QList<QGraphicsItem*> :: iterator MazeGraphicsScene::findRect(const QRectF& rectToFind, QList<QGraphicsItem*>& items)
{
    auto iter = items.begin();
    for (; iter != items.end(); ++iter) {
        QGraphicsRectItem* item = dynamic_cast<QGraphicsRectItem*>(*iter);
        if(item != nullptr){
            if(item->rect() == rectToFind)
                break;
        }
    }
    return iter;
}

void MazeGraphicsScene::drawGrid()
{
    for (int y = 0; y < Y_NUM; ++y)
        for (int x = 0; x < X_NUM; ++x)
            drawMazeCell(DX * x, DY * y);
}

void MazeGraphicsScene::newFrame()
{
    std::lock_guard<std::mutex> guard(m_mazeGenerator->getLock());
    auto& pairs = m_mazeGenerator->getResult();
    if(pairs.empty())
        return;

    if(m_iter != std::end(pairs)){
        QLineF line = getLineToDestroy(*m_iter);
        //! стераем
        findAndRemove(line);
        ++m_iter;
        building(true);
    }
    else {
        done();
        building(false);
    }

    if(!m_way.empty()){
        auto number = m_way.top(); // закрасить
        auto point = getCoordinateByNumber(number);
        addRect(point.x() + ((DX  - STEP_RECT) / 2),
                point.y() + ((DY  - STEP_RECT) / 2),
                STEP_RECT,
                STEP_RECT,
                QPen(),
                QBrush(Qt::darkBlue));
        m_way.pop();
    }
}

void MazeGraphicsScene::run()
{
    m_future = std::async(std::launch::async, [&]() { m_mazeGenerator->calculate(); });
    auto& pairs = m_mazeGenerator->getResult();
    m_iter = std::begin(pairs);
}

QLineF MazeGraphicsScene::getLineToDestroy(const std::pair<int, int>& pair)
{
    QList<QLineF> list1;
    QList<QLineF> list2;
    //
    auto coord1 = getCoordinateByNumber(pair.first);
    auto coord2 = getCoordinateByNumber(pair.second);
    //
    list1 << QLineF(coord1.x(), coord1.y(), coord1.x() + DX, coord1.y());
    list1 << QLineF(coord1.x(), coord1.y(), coord1.x(), coord1.y()+ DY);
    list1 << QLineF(coord1.x() + DY, coord1.y(), coord1.x() + DY, coord1.y() + DY);
    list1 << QLineF(coord1.x(), coord1.y() + DY, coord1.x() + DX, coord1.y() + DY);
    //
    list2 << QLineF(coord2.x(), coord2.y(), coord2.x() + DX, coord2.y());
    list2 << QLineF(coord2.x(), coord2.y(), coord2.x(), coord2.y()+ DY);
    list2 << QLineF(coord2.x() + DY, coord2.y(), coord2.x() + DY, coord2.y() + DY);
    list2 << QLineF(coord2.x(), coord2.y() + DY, coord2.x() + DX, coord2.y() + DY);

    foreach(auto& line1, list1)
        foreach(auto& line2, list2){
            if(line1 == line2){
                //! если координаты совпали - это как раз линия, которую нужно стереть
                return line1;
            }
        }
    return QLineF();
}

int MazeGraphicsScene::getNumber(qreal x, qreal y)
{
    return (int(x / DX) + X_NUM * int(y / DY) + 1);
}

QPoint MazeGraphicsScene::getCoordinateByNumber(int number)
{
    QPoint point;
    point.setY((int)((number - 1) / X_NUM) * DY);
    point.setX(((number - 1) % X_NUM) * DX);
    return point;
}

void MazeGraphicsScene::findWayExec()
{
    std::lock_guard<std::mutex> guard(m_mazeGenerator->getLock());
    auto& pairs = m_mazeGenerator->getResult();
    //! формируем граф
    std::list<std::list<graph_node>> graph;
    for(int i = 1; i <= X_NUM * Y_NUM; ++i){
        std::list<graph_node> nodeList;
        nodeList.push_back(i);
        std::for_each(std::begin(pairs), std::end(pairs), [&nodeList, i](const std::pair<int, int>& pair){
            if(pair.first == i){
                graph_node node(pair.second);
                nodeList.push_back(node);
            }
            else if(pair.second == i){
                graph_node node(pair.first);
                nodeList.push_back(node);
            }
        });
        graph.push_back(nodeList);
    }
    m_way = findWay(graph, 1, 300);
}

std::stack<int> MazeGraphicsScene::findWay(std::list<std::list<graph_node>>&graph, int start, int finish)
{
    std::stack<int> way;

    int temp_weight = 0;
    int current_num = start;

    std::map <int, int> num_weight = {{ current_num, temp_weight }};

    int counter {0};
    int exp {1};
    std::queue<int> for_counter;   //! очередь для присвоения номера-веса
    std::queue<int> queue;         //! очередь на проверку

    auto it_g = graph.begin();
    (*(*it_g).begin()).m_weight = temp_weight;
    temp_weight++;
    do{
        int temp_num = (*(*it_g).begin()).m_number;
        if (current_num == temp_num){
            exp--;
            for (auto it_n = (*it_g).begin(); it_n != (*it_g).end(); it_n++){
                auto it = num_weight.find((*it_n).m_number);

                if (it == num_weight.end()) {
                    //заносим в map
                    num_weight.insert(std::make_pair((*it_n).m_number, temp_weight));
                    (*it_n).m_weight = temp_weight;
                    queue.push((*it_n).m_number);
                    counter++;
                }
                else
                    (*it_n).m_weight = it->second;
            }
            if (counter)
                for_counter.push(counter);

            if (exp == 0 && !queue.empty()){
                exp = for_counter.front();
                for_counter.pop();
                temp_weight++;
            }
            //если очередь пуста - обход графа закончен
            if (queue.empty()) {
                std::map <int, int> ::iterator it;
                it = num_weight.find(finish);
                if (it == (num_weight.end())) {
                    return way; //маршрут не найден
                }
                else{
                    //восстанавливаем путь
                    current_num = finish;
                    way.push(finish);
                    it_g = graph.begin();
                    do {
                        int temp_num = (*(*it_g).begin()).m_number;
                        if (current_num == temp_num) {
                            std::list<graph_node> ::iterator it_n = (*it_g).begin();
                            int min = (*it_n).m_weight;
                            it_n++;
                            int temp;
                            for (; it_n != (*it_g).end(); it_n++){
                                if ((*it_n).m_weight < min){
                                    min = (*it_n).m_weight;
                                    temp = (*it_n).m_number;
                                }
                            }
                            way.push(temp);
                            if (min == 0) {
                                way.pop();
                                return way;
                            }
                            it_g = graph.begin();
                            current_num = temp;
                        }
                        else {
                            //если не нашли, то идем дальше
                            ++it_g;
                        }
                    } while (it_g != graph.end());
                }
            }

            //если не пуста - берем соседа из очереди
            current_num = queue.front();
            queue.pop();

            //сброс счетчика
            counter = 0;

            //сброс итератора
            it_g = graph.begin();
        }
        else {
            //если не нашли, то идем дальше
            ++it_g;
        }
    } while (it_g != graph.end());

    //если не нашли  - выход
    return way;
}
