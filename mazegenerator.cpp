#include "mazegenerator.h"
#include <random>
#include <algorithm>


MazeGenerator::MazeGenerator(int numberOfElements, int numberInRow)
    : m_numberOfElements(numberOfElements)
    , m_numberInRow(numberInRow)
{
    pairs.reserve(numberOfElements);
}

void MazeGenerator::clear()
{
    m_alreadyBeen.clear();
    std::lock_guard<std::mutex> guard(m_lock);
    pairs.clear();
}

void MazeGenerator::addPair(std::pair<int, int> pair)
{
    std::lock_guard<std::mutex> guard(m_lock);
    pairs.push_back(pair);
}

std::vector<std::pair<int, int>> MazeGenerator::calculate()
{
    std::stack<int> mainStack;
    //рандомно выбираем откуда начать..
    mainStack.push((std::rand() % m_numberOfElements) + 1);
    //работаем, пока не пройдем по всем ящейкам лабиринта
    do{
        auto current = mainStack.top();
        m_alreadyBeen.push_back(current);
        //выбираем соседа
        auto neighbor = randNeighbor(current);
        if(neighbor != 0){
            //если сосед есть - ломаем стену
            addPair({current, neighbor});
            mainStack.push(neighbor);
        }
        else{
            mainStack.pop();
        }
    }while (!mainStack.empty());
    // возвращаем результат
    return pairs;
}

std::vector<std::pair<int, int>>& MazeGenerator::getResult()
{
   return pairs;
}


int MazeGenerator::randNeighbor(int number)
{
    //! number + 1
    //! number + 1
    //! number + m_numberInRow
    //! number - m_numberInRow

    std::vector<int> neighbors;
    neighbors.reserve(4);
    int count{0};

    // текущий ряд = кол-во элементов на текущий номер
    double temp = (double)number / (double)m_numberInRow;
    auto rowNum =  std::ceil(temp);
    //
    auto maxRowNum= rowNum * m_numberInRow;
    auto minRowNum = maxRowNum  - (m_numberInRow - 1);

    if(number + 1 <= maxRowNum){
        auto iter = std::find(std::begin(m_alreadyBeen), std::end(m_alreadyBeen), number + 1);
        if(iter == std::end(m_alreadyBeen)){
            neighbors.push_back(number + 1);
            count++;
        }
    }
    if(number + m_numberInRow < m_numberOfElements){
        auto iter = std::find(std::begin(m_alreadyBeen), std::end(m_alreadyBeen), number + m_numberInRow);
        if(iter == std::end(m_alreadyBeen)){
            neighbors.push_back(number + m_numberInRow);
            count++;
        }
    }
    if(number - 1 >= minRowNum){
        auto iter = std::find(std::begin(m_alreadyBeen), std::end(m_alreadyBeen), number - 1);
        if(iter == std::end(m_alreadyBeen)){
            neighbors.push_back(number - 1);
            count++;
        }
    }
    if(number - m_numberInRow > 0){
        auto iter = std::find(std::begin(m_alreadyBeen), std::end(m_alreadyBeen), number - m_numberInRow);
        if(iter == std::end(m_alreadyBeen)){
            neighbors.push_back(number - m_numberInRow);
            count++;
        }
    }
    if(count == 0)
        return 0;
    //
    std::size_t i = static_cast<std::size_t>(std::rand() % count);
    return neighbors[i];
}


