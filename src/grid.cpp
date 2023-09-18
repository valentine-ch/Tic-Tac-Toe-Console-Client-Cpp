#include "grid.h"

Grid::Grid(int size, int winnigLine): m_size{size}, m_winningLine{winnigLine}
{
    resizeGrid(size);
    setDefaultValues();
}

void Grid::resizeGrid(int size)
{
    m_grid.resize(size);
    for (auto& i: m_grid)
    {
        i.resize(size);
    }
}

void Grid::setDefaultValues()
{
    for (int i{}; i < m_size; ++i)
    {
        for (int j{}; j < m_size; ++j)
        {
            m_grid[i][j] = States::empty;
            m_emptyCells.push_back(std::make_pair('a' + i, j + 1));
        }
    }
}

bool Grid::isEmptyCell(char row, int column) const
{
    if (((row >= m_size + 'a') || (row < 'a')) ||
        ((column > m_size) || (column < 1)))
    {
        return false;
    }
    return m_grid[row - 'a'][column - 1] == States::empty;
}

void Grid::setCell(char row, int column, States token)
{
    m_grid[row - 'a'][column - 1] = token;
    auto it = std::find(m_emptyCells.begin(), m_emptyCells.end(),
        std::make_pair(row, column));
    if (it != m_emptyCells.end())
    {
        m_emptyCells.erase(it);
    }
}

bool Grid::isFull() const
{
    return m_emptyCells.empty();
}

void Grid::clear()
{
    m_emptyCells.clear();
    setDefaultValues();
}

const std::vector<std::pair<char, int>>& Grid::getEmptyCells() const
{
    return m_emptyCells;
}

bool Grid::hasWon(States side) const
{
    // check rows
    for (int i{}; i < m_size; ++i)
    {
        for (int j{}; j <= m_size - m_winningLine; ++j)
        {
            bool win{true};
            for (int k{}; k < m_winningLine; ++k)
            {
                if (m_grid[i][j + k] != side)
                {
                    win = false;
                    break;
                }
            }
            if (win)
            {
                return true;
            }
        }
    }
    // check columns
    for (int j{}; j < m_size; ++j)
    {
        for (int i{}; i <= m_size - m_winningLine; ++i)
        {
            bool win{true};
            for (int k{}; k < m_winningLine; ++k)
            {
                if (m_grid[i + k][j] != side)
                {
                    win = false;
                    break;
                }
            }
            if (win)
            {
                return true;
            }
        }
    }
    // check diagonals (top-left to bottom-right)
    for (int i{}; i <= m_size - m_winningLine; ++i)
    {
        for (int j{}; j <= m_size - m_winningLine; ++j)
        {
            bool win{true};
            for (int k{}; k < m_winningLine; ++k)
            {
                if (m_grid[i + k][j + k] != side)
                {
                    win = false;
                    break;
                }
            }
            if (win)
            {
                return true;
            }
        }
    }
    // check diagonals (top-right to bottom-left)
    for (int i{}; i <= m_size - m_winningLine; ++i)
    {
        for (int j{m_winningLine - 1}; j < m_size; ++j)
        {
            bool win{true};
            for (int k{}; k < m_winningLine; ++k)
            {
                if (m_grid[i + k][j - k] != side)
                {
                    win = false;
                    break;
                }
            }
            if (win)
            {
                return true;
            }
        }
    }
    // no winning condition met
    return false;
}

bool Grid::isWinnigMove(char row, int column, States token) const
{
    //check column
    int columnLine{1};
    for (int i{1}; (row - 'a' - i) >= 0; ++i)
    {
        if (m_grid[row - 'a' - i][column - 1] != token)
        {
            break;
        }
        ++columnLine;
    }
    for (int i{1}; (row - 'a' + i) < m_size; ++i)
    {
        if (m_grid[row - 'a' + i][column - 1] != token)
        {
            break;
        }
        ++columnLine;
    }
    if (columnLine >= m_winningLine)
    {
        return true;
    }
    //check row
    int rowLine{1};
    for (int i{1}; (column - i - 1) >= 0; ++i)
    {
        if (m_grid[row - 'a'][column - i - 1] != token)
        {
            break;
        }
        ++rowLine;
    }
    for (int i{1}; (column - 1 + i) < m_size; ++i)
    {
        if (m_grid[row - 'a'][column + i - 1] != token)
        {
            break;
        }
        ++rowLine;
    }
    if (rowLine >= m_winningLine)
    {
        return true;
    }
    //check diagonal (bottom-left to top-right)
    int bottomLeftToTopRightDiagonalLine{1};
    for (int i{1}; ((row - 'a' + i) < m_size) && ((column - 1 - i) >= 0); ++i)
    {
        if (m_grid[row - 'a' + i][column - 1 - i] != token)
        {
            break;
        }
        ++bottomLeftToTopRightDiagonalLine;
    }
    for (int i{1}; ((row - 'a' - i) >= 0) && ((column - 1 + i) < m_size); ++i)
    {
        if (m_grid[row - 'a' - i][column - 1 + i] != token)
        {
            break;
        }
        ++bottomLeftToTopRightDiagonalLine;
    }
    if (bottomLeftToTopRightDiagonalLine >= m_winningLine)
    {
        return true;
    }
    //check diagonal (top-left to bottom-right)
    int topLeftToBottomRightDiagonalLine{1};
    for (int i{1}; ((row - 'a' - i) >= 0) && ((column - 1 - i) >= 0); ++i)
    {
        if (m_grid[row - 'a' - i][column - 1 - i] != token)
        {
            break;
        }
        ++topLeftToBottomRightDiagonalLine;
    }
    for (int i{1}; ((row - 'a' + i) < m_size) &&
        ((column - 1 + i) < m_size); ++i)
    {
        if (m_grid[row - 'a' + i][column - 1 + i] != token)
        {
            break;
        }
        ++topLeftToBottomRightDiagonalLine;
    }
    if (topLeftToBottomRightDiagonalLine >= m_winningLine)
    {
        return true;
    }
    // no winning condition met
    return false;
}

bool Grid::isForkingMove(char row, int column, States token) const
{
    int winningConditions{};
    //check column
    int columnLine{1};
    bool blockedAbove{true};
    int tokensAboveEmptyCell{};
    bool blockedBelow{true};
    int tokensBelowEmptyCell{};
    for (int i{1}; (row - 'a' - i) >= 0; ++i)
    {
        if (m_grid[row - 'a' - i][column - 1] != token)
        {
            if (m_grid[row - 'a' - i][column - 1] == States::empty)
            {
                blockedAbove = false;
                for (int j{1}; (row - 'a' - i - j) >= 0; ++j)
                {
                    if (m_grid[row - 'a' - i - j][column - 1] != token)
                    {
                        break;
                    }
                    ++tokensAboveEmptyCell;
                }
                break;
            }
            else
            {
                break;
            }
        }
        else if (blockedAbove)
        {
            ++columnLine;
        }
        else
        {
            break;
        }
    }
    for (int i{1}; (row - 'a' + i) < m_size; ++i)
    {
        if (m_grid[row - 'a' + i][column - 1] != token)
        {
            if (m_grid[row - 'a' + i][column - 1] == States::empty)
            {
                blockedBelow = false;
                for (int j{1}; (row - 'a' + i + j) < m_size; ++j)
                {
                    if (m_grid[row - 'a' + i + j][column - 1] != token)
                    {
                        break;
                    }
                    ++tokensBelowEmptyCell;
                }
                break;
            }
            else
            {
                break;
            }
        }
        else if (blockedBelow)
        {
            ++columnLine;
        }
        else
        {
            break;
        }
    }
    if ((!(blockedAbove)) && ((columnLine + 1 + tokensAboveEmptyCell) >=
        m_winningLine))
    {
        ++winningConditions;
    }
    if ((!(blockedBelow)) && ((columnLine + 1 + tokensBelowEmptyCell) >=
        m_winningLine))
    {
        ++winningConditions;
    }
    //check row
    int rowLine{1};
    bool blockedLeft{true};
    int tokensLeftOfEmptyCell{};
    bool blockedRight{true};
    int tokensRightOfEmptyCell{};
    for (int i{1}; (column - i - 1) >= 0; ++i)
    {
        if (m_grid[row - 'a'][column - i - 1] != token)
        {
            if (m_grid[row - 'a'][column - i - 1] == States::empty)
            {
                blockedLeft = false;
                for (int j{1}; (column - i - 1 - j) >= 0; ++j)
                {
                    if (m_grid[row - 'a'][column - i - 1 - j] != token)
                    {
                        break;
                    }
                    ++tokensLeftOfEmptyCell;
                }
                break;
            }
            else
            {
                break;
            }
        }
        else if (blockedLeft)
        {
            ++rowLine;
        }
        else 
        {
            break;
        }
    }
    for (int i{1}; (column - 1 + i) < m_size; ++i)
    {
        if (m_grid[row - 'a'][column + i - 1] != token)
        {
            if (m_grid[row - 'a'][column + i - 1] == States::empty)
            {
                blockedRight = false;
                for (int j{1}; (column - 1 + i + j) < m_size; ++j)
                {
                    if (m_grid[row - 'a'][column + i - 1 + j] != token)
                    {
                        break;
                    }
                    ++tokensRightOfEmptyCell;
                }
                break;
            }
            else
            {
                break;
            }
        }
        else if (blockedRight)
        {
            ++rowLine;
        }
        else
        {
            break;
        }
    }
    if ((!(blockedLeft)) && ((rowLine + 1 + tokensLeftOfEmptyCell) >=
        m_winningLine))
    {
        ++winningConditions;
    }
    if ((!(blockedRight)) && ((rowLine + 1 + tokensRightOfEmptyCell) >=
        m_winningLine))
    {
        ++winningConditions;
    }
    // check diagonal (bottom-left to top-right)
    int bottomLeftToTopRightDiagonalLine{1};
    bool blockedTopRight{true};
    int tokensTopRightOfEmptyCell{};
    bool blockedBottomLeft{true};
    int tokensBottomLeftOfEmptyCell{};
    for (int i{1}; ((row - 'a' + i) < m_size) && ((column - 1 - i) >= 0); ++i)
    {
        if (m_grid[row - 'a' + i][column - 1 - i] != token)
        {
            if (m_grid[row - 'a' + i][column - 1 - i] == States::empty)
            {
                blockedBottomLeft = false;
                for (int j{1}; ((row - 'a' + i + j) < m_size) &&
                    ((column - 1 - i - j) >= 0); ++j)
                {
                    if (m_grid[row - 'a' + i + j][column - 1 - i - j] != token)
                    {
                        break;
                    }
                    ++tokensBottomLeftOfEmptyCell;
                }
                break;
            }
            else
            {
                break;
            }
        }
        else if (blockedBottomLeft)
        {
            ++bottomLeftToTopRightDiagonalLine;
        }
        else
        {
            break;
        }
    }
    for (int i{1}; ((row - 'a' - i) >= 0) && ((column - 1 + i) < m_size); ++i)
    {
        if (m_grid[row - 'a' - i][column - 1 + i] != token)
        {
            if (m_grid[row - 'a' - i][column - 1 + i] == States::empty)
            {
                blockedTopRight = false;
                for (int j{1}; ((row - 'a' - i - j) >= 0) &&
                    ((column - 1 + i + j) < m_size); ++j)
                {
                    if (m_grid[row - 'a' - i - j][column - 1 + i + j] != token)
                    {
                        break;
                    }
                    ++tokensTopRightOfEmptyCell;
                }
                break;
            }
            else
            {
                break;
            }
        }
        else if (blockedTopRight)
        {
            ++bottomLeftToTopRightDiagonalLine;
        }
        else
        {
            break;
        }
    }
    if ((!(blockedBottomLeft)) && ((bottomLeftToTopRightDiagonalLine + 1 +
        tokensBottomLeftOfEmptyCell) >= m_winningLine))
    {
        ++winningConditions;
    }
    if ((!(blockedTopRight)) && ((bottomLeftToTopRightDiagonalLine + 1 +
        tokensTopRightOfEmptyCell) >= m_winningLine))
    {
        ++winningConditions;
    }
    // check diagonal (top-left to bottom-right)
    int topLeftToBottomRightDiagonalLine{1};
    bool blockedTopLeft{true};
    int tokensTopLeftOfEmptyCell{};
    bool blockedBottomRight{true};
    int tokensBottomRightOfEmptyCell{};
    for (int i{1}; ((row - 'a' - i) >= 0) && ((column - 1 - i) >= 0); ++i)
    {
        if (m_grid[row - 'a' - i][column - 1 - i] != token)
        {
            if (m_grid[row - 'a' - i][column - 1 - i] == States::empty)
            {
                blockedTopLeft = false;
                for (int j{1}; ((row - 'a' - i - j) >= 0) &&
                    ((column - 1 - i - j) >= 0); ++j)
                {
                    if (m_grid[row - 'a' - i - j][column - 1 - i - j] != token)
                    {
                        break;
                    }
                    ++tokensTopLeftOfEmptyCell;
                }
                break;
            }
            else
            {
                break;
            }
        }
        else if (blockedTopLeft)
        {
            ++topLeftToBottomRightDiagonalLine;
        }
        else 
        {
            break;
        }
    }
    for (int i{1}; ((row - 'a' + i) < m_size) &&
        ((column - 1 + i) < m_size); ++i)
    {
        if (m_grid[row - 'a' + i][column - 1 + i] != token)
        {
            if (m_grid[row - 'a' + i][column - 1 + i] == States::empty)
            {
                blockedBottomRight = false;
                for (int j{1}; ((row - 'a' + i + j) < m_size) &&
                    ((column - 1 + i + j) < m_size); ++j)
                {
                    if (m_grid[row - 'a' + i + j][column - 1 + i + j] != token)
                    {
                        break;
                    }
                    ++tokensBottomRightOfEmptyCell;
                }
                break;
            }
            else
            {
                break;
            }
        }
        else if (blockedBottomRight)
        {
            ++topLeftToBottomRightDiagonalLine;
        }
        else 
        {
            break;
        }
    }
    if ((!(blockedTopLeft)) && ((topLeftToBottomRightDiagonalLine + 1 +
        tokensTopLeftOfEmptyCell) >= m_winningLine))
    {
        ++winningConditions;
    }
    if ((!(blockedBottomRight)) && ((topLeftToBottomRightDiagonalLine + 1 +
        tokensBottomRightOfEmptyCell) >= m_winningLine))
    {
        ++winningConditions;
    }
    // If winningConditions is 2 or more then it's a forking move
    return winningConditions >= 2;
}

bool Grid::isBlockedMove(char row, int column, States token) const
{
    //check column
    int columnLine{1};
    bool blockedVertically{false};
    for (int i{1}; (row - 'a' - i) >= 0; ++i)
    {
        if ((m_grid[row - 'a' - i][column - 1] != token) &&
            (m_grid[row - 'a' - i][column - 1] != States::empty))
        {
            break;
        }
        ++columnLine;
    }
    for (int i{1}; (row - 'a' + i) < m_size; ++i)
    {
        if ((m_grid[row - 'a' + i][column - 1] != token) &&
            (m_grid[row - 'a' + i][column - 1] != States::empty))
        {
            break;
        }
        ++columnLine;
    }
    if (columnLine < m_winningLine)
    {
        blockedVertically = true;
    }
    //check row
    int rowLine{1};
    bool blockedHorizontally{false};
    for (int i{1}; (column - i - 1) >= 0; ++i)
    {
        if ((m_grid[row - 'a'][column - i - 1] != token) &&
            (m_grid[row - 'a'][column - i - 1] != States::empty))
        {
            break;
        }
        ++rowLine;
    }
    for (int i{1}; (column - 1 + i) < m_size; ++i)
    {
        if ((m_grid[row - 'a'][column + i - 1] != token) &&
            (m_grid[row - 'a'][column + i - 1] != States::empty))
        {
            break;
        }
        ++rowLine;
    }
    if (rowLine < m_winningLine)
    {
        blockedHorizontally = true;
    }
    //check diagonal (bottom-left to top-right)
    int bottomLeftToTopRightDiagonalLine{1};
    bool blockedBottomLeftToTopRight{false};
    for (int i{1}; ((row - 'a' + i) < m_size) && ((column - 1 - i) >= 0); ++i)
    {
        if ((m_grid[row - 'a' + i][column - 1 - i] != token) && 
            (m_grid[row - 'a' + i][column - 1 - i] !=States::empty))
        {
            break;
        }
        ++bottomLeftToTopRightDiagonalLine;
    }
    for (int i{1}; ((row - 'a' - i) >= 0) && ((column - 1 + i) < m_size); ++i)
    {
        if ((m_grid[row - 'a' - i][column - 1 + i] != token) &&
            (m_grid[row - 'a' - i][column - 1 + i] != States::empty))
        {
            break;
        }
        ++bottomLeftToTopRightDiagonalLine;
    }
    if (bottomLeftToTopRightDiagonalLine < m_winningLine)
    {
        blockedBottomLeftToTopRight = true;
    }
    //check diagonal (top-left to bottom-right)
    int topLeftToBottomRightDiagonalLine{1};
    bool blockedTopLeftToBottomRight{false};
    for (int i{1}; ((row - 'a' - i) >= 0) && ((column - 1 - i) >= 0); ++i)
    {
        if ((m_grid[row - 'a' - i][column - 1 - i] != token) &&
            (m_grid[row - 'a' - i][column - 1 - i] != States::empty))
        {
            break;
        }
        ++topLeftToBottomRightDiagonalLine;
    }
    for (int i{1}; ((row - 'a' + i) < m_size) &&
        ((column - 1 + i) < m_size); ++i)
    {
        if ((m_grid[row - 'a' + i][column - 1 + i] != token) && 
            (m_grid[row - 'a' + i][column - 1 + i]) != States::empty)
        {
            break;
        }
        ++topLeftToBottomRightDiagonalLine;
    }
    if (topLeftToBottomRightDiagonalLine < m_winningLine)
    {
        blockedTopLeftToBottomRight = true;
    }
    //if all directions are blocked return true
    return (blockedVertically && blockedHorizontally && 
        blockedBottomLeftToTopRight && blockedTopLeftToBottomRight);
}

bool Grid::hasImmediateThreat(char row, int column, States token) const
{
    //check column
    int columnLine{1};
    bool blockedAbove{true};
    int tokensAboveEmptyCell{};
    bool blockedBelow{true};
    int tokensBelowEmptyCell{};
    for (int i{1}; (row - 'a' - i) >= 0; ++i)
    {
        if (m_grid[row - 'a' - i][column - 1] != token)
        {
            if (m_grid[row - 'a' - i][column - 1] == States::empty)
            {
                blockedAbove = false;
                for (int j{1}; (row - 'a' - i - j) >= 0; ++j)
                {
                    if (m_grid[row - 'a' - i - j][column - 1] != token)
                    {
                        break;
                    }
                    ++tokensAboveEmptyCell;
                }
                break;
            }
            else
            {
                break;
            }
        }
        else if (blockedAbove)
        {
            ++columnLine;
        }
        else
        {
            break;
        }
    }
    for (int i{1}; (row - 'a' + i) < m_size; ++i)
    {
        if (m_grid[row - 'a' + i][column - 1] != token)
        {
            if (m_grid[row - 'a' + i][column - 1] == States::empty)
            {
                blockedBelow = false;
                for (int j{1}; (row - 'a' + i + j) < m_size; ++j)
                {
                    if (m_grid[row - 'a' + i + j][column - 1] != token)
                    {
                        break;
                    }
                    ++tokensBelowEmptyCell;
                }
                break;
            }
            else
            {
                break;
            }
        }
        else if (blockedBelow)
        {
            ++columnLine;
        }
        else
        {
            break;
        }
    }
    if ((!(blockedAbove)) && ((columnLine + 1 + tokensAboveEmptyCell) >=
        m_winningLine))
    {
        return true;
    }
    if ((!(blockedBelow)) && ((columnLine + 1 + tokensBelowEmptyCell) >=
        m_winningLine))
    {
        return true;
    }
    //check row
    int rowLine{1};
    bool blockedLeft{true};
    int tokensLeftOfEmptyCell{};
    bool blockedRight{true};
    int tokensRightOfEmptyCell{};
    for (int i{1}; (column - i - 1) >= 0; ++i)
    {
        if (m_grid[row - 'a'][column - i - 1] != token)
        {
            if (m_grid[row - 'a'][column - i - 1] == States::empty)
            {
                blockedLeft = false;
                for (int j{1}; (column - i - 1 - j) >= 0; ++j)
                {
                    if (m_grid[row - 'a'][column - i - 1 - j] != token)
                    {
                        break;
                    }
                    ++tokensLeftOfEmptyCell;
                }
                break;
            }
            else
            {
                break;
            }
        }
        else if (blockedLeft)
        {
            ++rowLine;
        }
        else 
        {
            break;
        }
    }
    for (int i{1}; (column - 1 + i) < m_size; ++i)
    {
        if (m_grid[row - 'a'][column + i - 1] != token)
        {
            if (m_grid[row - 'a'][column + i - 1] == States::empty)
            {
                blockedRight = false;
                for (int j{1}; (column - 1 + i + j) < m_size; ++j)
                {
                    if (m_grid[row - 'a'][column + i - 1 + j] != token)
                    {
                        break;
                    }
                    ++tokensRightOfEmptyCell;
                }
                break;
            }
            else
            {
                break;
            }
        }
        else if (blockedRight)
        {
            ++rowLine;
        }
        else
        {
            break;
        }
    }
    if ((!(blockedLeft)) && ((rowLine + 1 + tokensLeftOfEmptyCell) >=
        m_winningLine))
    {
        return true;
    }
    if ((!(blockedRight)) && ((rowLine + 1 + tokensRightOfEmptyCell) >=
        m_winningLine))
    {
        return true;
    }
    // check diagonal (bottom-left to top-right)
    int bottomLeftToTopRightDiagonalLine{1};
    bool blockedTopRight{true};
    int tokensTopRightOfEmptyCell{};
    bool blockedBottomLeft{true};
    int tokensBottomLeftOfEmptyCell{};
    for (int i{1}; ((row - 'a' + i) < m_size) && ((column - 1 - i) >= 0); ++i)
    {
        if (m_grid[row - 'a' + i][column - 1 - i] != token)
        {
            if (m_grid[row - 'a' + i][column - 1 - i] == States::empty)
            {
                blockedBottomLeft = false;
                for (int j{1}; ((row - 'a' + i + j) < m_size) &&
                    ((column - 1 - i - j) >= 0); ++j)
                {
                    if (m_grid[row - 'a' + i + j][column - 1 - i - j] != token)
                    {
                        break;
                    }
                    ++tokensBottomLeftOfEmptyCell;
                }
                break;
            }
            else
            {
                break;
            }
        }
        else if (blockedBottomLeft)
        {
            ++bottomLeftToTopRightDiagonalLine;
        }
        else
        {
            break;
        }
    }
    for (int i{1}; ((row - 'a' - i) >= 0) && ((column - 1 + i) < m_size); ++i)
    {
        if (m_grid[row - 'a' - i][column - 1 + i] != token)
        {
            if (m_grid[row - 'a' - i][column - 1 + i] == States::empty)
            {
                blockedTopRight = false;
                for (int j{1}; ((row - 'a' - i - j) >= 0) &&
                    ((column - 1 + i + j) < m_size); ++j)
                {
                    if (m_grid[row - 'a' - i - j][column - 1 + i + j] != token)
                    {
                        break;
                    }
                    ++tokensTopRightOfEmptyCell;
                }
                break;
            }
            else
            {
                break;
            }
        }
        else if (blockedTopRight)
        {
            ++bottomLeftToTopRightDiagonalLine;
        }
        else
        {
            break;
        }
    }
    if ((!(blockedBottomLeft)) && ((bottomLeftToTopRightDiagonalLine + 1 +
        tokensBottomLeftOfEmptyCell) >= m_winningLine))
    {
        return true;
    }
    if ((!(blockedTopRight)) && ((bottomLeftToTopRightDiagonalLine + 1 +
        tokensTopRightOfEmptyCell) >= m_winningLine))
    {
        return true;
    }
    // check diagonal (top-left to bottom-right)
    int topLeftToBottomRightDiagonalLine{1};
    bool blockedTopLeft{true};
    int tokensTopLeftOfEmptyCell{};
    bool blockedBottomRight{true};
    int tokensBottomRightOfEmptyCell{};
    for (int i{1}; ((row - 'a' - i) >= 0) && ((column - 1 - i) >= 0); ++i)
    {
        if (m_grid[row - 'a' - i][column - 1 - i] != token)
        {
            if (m_grid[row - 'a' - i][column - 1 - i] == States::empty)
            {
                blockedTopLeft = false;
                for (int j{1}; ((row - 'a' - i - j) >= 0) &&
                    ((column - 1 - i - j) >= 0); ++j)
                {
                    if (m_grid[row - 'a' - i - j][column - 1 - i - j] != token)
                    {
                        break;
                    }
                    ++tokensTopLeftOfEmptyCell;
                }
                break;
            }
            else
            {
                break;
            }
        }
        else if (blockedTopLeft)
        {
            ++topLeftToBottomRightDiagonalLine;
        }
        else 
        {
            break;
        }
    }
    for (int i{1}; ((row - 'a' + i) < m_size) &&
        ((column - 1 + i) < m_size); ++i)
    {
        if (m_grid[row - 'a' + i][column - 1 + i] != token)
        {
            if (m_grid[row - 'a' + i][column - 1 + i] == States::empty)
            {
                blockedBottomRight = false;
                for (int j{1}; ((row - 'a' + i + j) < m_size) &&
                    ((column - 1 + i + j) < m_size); ++j)
                {
                    if (m_grid[row - 'a' + i + j][column - 1 + i + j] != token)
                    {
                        break;
                    }
                    ++tokensBottomRightOfEmptyCell;
                }
                break;
            }
            else
            {
                break;
            }
        }
        else if (blockedBottomRight)
        {
            ++topLeftToBottomRightDiagonalLine;
        }
        else 
        {
            break;
        }
    }
    if ((!(blockedTopLeft)) && ((topLeftToBottomRightDiagonalLine + 1 +
        tokensTopLeftOfEmptyCell) >= m_winningLine))
    {
        return true;
    }
    if ((!(blockedBottomRight)) && ((topLeftToBottomRightDiagonalLine + 1 +
        tokensBottomRightOfEmptyCell) >= m_winningLine))
    {
        return true;
    }
    // no immediate threat found
    return false;
}

int Grid::countPotentialForks(char row, int column, States token) const
{
    int potentialForks{};
    //check column
    int columnLine{1};
    int emptyCellsAbove{};
    int emptyCellsBelow{};
    for (int i{1}; (row - 'a' - i) >= 0; ++i)
    {
        if (m_grid[row - 'a' - i][column - 1] == token && emptyCellsAbove == 0)
        {
            ++columnLine;
        }
        else if (m_grid[row - 'a' - i][column - 1] == States::empty)
        {
            ++emptyCellsAbove;
        }
        else
        {
            break;
        }
    }
    for (int i{1}; (row - 'a' + i) < m_size; ++i)
    {
        if (m_grid[row - 'a' + i][column - 1] == token && emptyCellsBelow == 0)
        {
            ++columnLine;
        }
        else if (m_grid[row - 'a' + i][column - 1] == States::empty)
        {
            ++emptyCellsBelow;
        }
        else
        {
            break;
        }
    }
    if ((columnLine >= (m_winningLine - 2)) &&
        ((emptyCellsAbove >= 1 && emptyCellsBelow >= 2) ||
        (emptyCellsAbove >= 2 && emptyCellsBelow >= 1)))
    {
        ++potentialForks;
    }
    // Check row
    int rowLine{1};
    int emptyCellsLeft{};
    int emptyCellsRight{};
    for (int i{1}; (column - 1 - i) >= 0; ++i)
    {
        if (m_grid[row - 'a'][column - 1 - i] == token && emptyCellsLeft == 0)
        {
            ++rowLine;
        }
        else if (m_grid[row - 'a'][column - 1 - i] == States::empty)
        {
            ++emptyCellsLeft;
        }
        else
        {
            break;
        }
    }
    for (int i{1}; (column - 1 + i) < m_size; ++i)
    {
        if (m_grid[row - 'a'][column - 1 + i] == token && emptyCellsRight == 0)
        {
            ++rowLine;
        }
        else if (m_grid[row - 'a'][column - 1 + i] == States::empty)
        {
            ++emptyCellsRight;
        }
        else
        {
            break;
        }
    }
    if ((rowLine >= (m_winningLine - 2)) &&
        ((emptyCellsLeft >= 1 && emptyCellsRight >= 2) ||
        (emptyCellsLeft >= 2 && emptyCellsRight >= 1)))
    {
        ++potentialForks;
    }
    // Check diagonal (top left to bottom right)
    int diagonal1Line{1};
    int emptyCellsTopLeft{};
    int emptyCellsBottomRight{};
    for (int i{1}; (row - 'a' - i) >= 0 && (column - 1 - i) >= 0; ++i)
    {
        if (m_grid[row - 'a' - i][column - 1 - i] == token &&
            emptyCellsTopLeft == 0)
        {
            ++diagonal1Line;
        }
        else if (m_grid[row - 'a' - i][column - 1 - i] == States::empty)
        {
            ++emptyCellsTopLeft;
        }
        else
        {
            break;
        }
    }
    for (int i{1}; (row - 'a' + i) < m_size && (column - 1 + i) < m_size; ++i)
    {
        if (m_grid[row - 'a' + i][column - 1 + i] == token &&
            emptyCellsBottomRight == 0)
        {
            ++diagonal1Line;
        }
        else if (m_grid[row - 'a' + i][column - 1 + i] == States::empty)
        {
            ++emptyCellsBottomRight;
        }
        else
        {
            break;
        }
    }
    if ((diagonal1Line >= (m_winningLine - 2)) &&
        ((emptyCellsTopLeft >= 1 && emptyCellsBottomRight >= 2) ||
        (emptyCellsTopLeft >= 2 && emptyCellsBottomRight >= 1)))
    {
        ++potentialForks;
    }
    // Check diagonal (bottom left to top right)
    int diagonal2Line{1};
    int emptyCellsBottomLeft{};
    int emptyCellsTopRight{};
    for (int i{1}; (row - 'a' + i) < m_size && (column - 1 - i) >= 0; ++i)
    {
        if (m_grid[row - 'a' + i][column - 1 - i] == token &&
            emptyCellsBottomLeft == 0)
        {
            ++diagonal2Line;
        }
        else if (m_grid[row - 'a' + i][column - 1 - i] == States::empty)
        {
            ++emptyCellsBottomLeft;
        }
        else
        {
            break;
        }
    }
    for (int i{1}; (row - 'a' - i) >= 0 && (column - 1 + i) < m_size; ++i)
    {
        if (m_grid[row - 'a' - i][column - 1 + i] == token &&
            emptyCellsTopRight == 0)
        {
            ++diagonal2Line;
        }
        else if (m_grid[row - 'a' - i][column - 1 + i] == States::empty)
        {
            ++emptyCellsTopRight;
        }
        else
        {
            break;
        }
    }
    if ((diagonal2Line >= (m_winningLine - 2)) &&
        ((emptyCellsBottomLeft >= 1 && emptyCellsTopRight >= 2) ||
        (emptyCellsBottomLeft >= 2 && emptyCellsTopRight >= 1)))
    {
        ++potentialForks;
    }
    return potentialForks;
}

int Grid::calculateDistanceScore(char row, int column, States token) const
{
    constexpr int weightFriendly{2};
    constexpr int weightOpponent{1};
    int score{};
    for (int i{std::max(static_cast<int>('a'), row - m_winningLine + 1)};
        i <= std::min(static_cast<int>('a') + m_size - 1,
            row + m_winningLine - 1); ++i)
    {
        for (int j{std::max(1, column - m_winningLine + 1)}; 
            j <= std::min(m_size, column + m_winningLine - 1); ++j)
        {
            if (i == row && j == column) // the cell itself, do nothing
            {
                continue;
            }
            int distance = std::max(std::abs(row - i), std::abs(column - j));
            int weightMultiplier = m_winningLine - distance + 1;
            if (m_grid[i - 'a'][j - 1] == token)
            {
                score += weightFriendly * weightMultiplier;
            }
            else if (m_grid[i - 'a'][j - 1] != States::empty)
            {
                score += weightOpponent * weightMultiplier;
            }
        }
    }
    return score;
}

std::ostream& operator<<(std::ostream& output, const Grid& grid)
{
    output << "  ";
    for (int i{}; i < grid.m_size; ++i)
    {
        output << "  " << i + 1;
        if (i < 9)
        {
            output << ' ';
        }
    }
    output << '\n';
    for (int i{}; i < grid.m_size; ++i)
    {
        output << "  ";
        for (int j{}; j < grid.m_size; ++j)
        {
            output << "+---";
        }
        output << "+\n" << static_cast<char>('a' + i) << ' ';
        for (int j{}; j < grid.m_size; ++j)
        {
            output << "| " << grid.m_grid[i][j] << ' ';
        }
        output << "| " << static_cast<char>('a' + i) << '\n';
    }
    output << "  ";
    for (int i{}; i < grid.m_size; ++i)
    {
        output << "+---";
    }
    output << "+\n  ";
    for (int i{}; i < grid.m_size; ++i)
    {
        output << "  " << i + 1;
        if (i < 9)
        {
            output << ' ';
        }
    }
    return output;
}