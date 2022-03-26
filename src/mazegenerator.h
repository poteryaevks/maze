#ifndef MAZEGENERATOR_H
#define MAZEGENERATOR_H

#include <stack>
#include <list>
#include <vector>
#include <mutex>
#include <thread>

class MazeGenerator
{
public:
    MazeGenerator(int numberOfElements, int numberInRow);
    ~MazeGenerator() {}
    //!
    //! \brief calculate расчитывает набор пар соседних ящеек лабиринта, стенки между которыми нужно сломать
    //! \return
    //!
    std::vector<std::pair<int,int>>calculate();
    //!
    std::vector<std::pair<int, int>>& getResult();
    //!
    //! \brief getLock
    //! \return
    //!
    std::mutex& getLock() { return m_lock; }
private:
    //!
    //! \brief randNeighbor - возвращает рандомно соседа для number, который не посещен
    //! \param number
    //! \return - если 0, значет нет таких соседей
    //!
    int randNeighbor(int number);
    //!
    //! \brief clear
    //!
    void clear();
    //!
    //! \brief addPair
    //! \param pair
    //!
    void addPair(std::pair<int, int> pair);
private:
    std::list<int> m_alreadyBeen;           //! список ящеек, где мы побывали
    std::vector<std::pair<int, int>> pairs; //! результат расчета можно получить асинхронно даже если он недосчитан
    int m_numberOfElements;
    int m_numberInRow;
    std::mutex m_lock;
};

#endif // MAZEGENERATOR_H
