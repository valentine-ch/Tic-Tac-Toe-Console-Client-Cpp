#ifndef GRID_H
#define GRID_H

#include <array>
#include <vector>
#include <utility>
#include <algorithm>
#include <cmath>
#include "states.h"

class Grid
{
private:
    int m_size{};
    int m_winningLine{};
    std::vector<std::vector<States>> m_grid;
    std::vector<std::pair<char, int>> m_emptyCells;
    void resizeGrid(int size);
    void setDefaultValues();
public:
    Grid(int size, int winnigLine);
    bool isEmptyCell(char row, int column) const;
    void setCell(char row, int column, States token);
    bool isFull() const;
    void clear();
    bool hasWon(States side) const;
    bool isWinnigMove(char row, int column, States token) const;
    bool isForkingMove(char row, int column, States token) const;
    bool isBlockedMove(char row, int column, States token) const;
    bool hasImmediateThreat(char row, int column, States token) const;
    int countPotentialForks(char row, int column, States token) const;
    int calculateDistanceScore(char row, int column, States token) const;
    const std::vector<std::pair<char, int>>& getEmptyCells() const;
    friend std::ostream& operator<<(std::ostream& output, const Grid& grid);
};

#endif