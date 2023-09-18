#include "play.h"

void playerMove(Grid& grid, States side)
{
    while (true)
    {
        std::stringstream input{getCellFromUser()};
        char row{};
        int column;
        input >> row >> column;
        row = std::tolower(row);
        if (grid.isEmptyCell(row, column))
        {
            grid.setCell(row, column, side);
            printMove("Your", row, column);
            return;
        }
        std::cout << "This is not a valid move\n";
    }
}

int generateRandomNumber(int min, int max)
{
    static std::mt19937 mt(std::time(nullptr));
    std::uniform_int_distribution range{min, max};
    return range(mt);
}

void makeComputerMove(Grid& grid, char row, int column, States side)
{
    grid.setCell(row, column, side);
    printMove("My", row, column);
}

void makeRamdomMove(Grid& grid, States side)
{
    auto& emptyCells = grid.getEmptyCells();
    int index = generateRandomNumber(0, emptyCells.size() - 1);
    auto [row, column] = emptyCells[index];
    makeComputerMove(grid, row, column, side);
}

bool makeAWinnigMoveIfPossible(Grid& grid, States side)
{
    auto& emptyCells = grid.getEmptyCells();
    for (auto i: emptyCells)
    {
        if (grid.isWinnigMove(i.first, i.second, side))
        {
            makeComputerMove(grid, i.first, i.second, side);
            return true;
        }
    }
    return false;
}

bool blockAWinnigMoveIfPossible(Grid& grid, States side)
{
    auto& emptyCells = grid.getEmptyCells();
    for (auto i: emptyCells)
    {
        if (grid.isWinnigMove(i.first, i.second,
            (side == States::x ? States::o : States::x)))
        {
            makeComputerMove(grid, i.first, i.second, side);
            return true;
        }
    }
    return false;
}

bool makeAForkingMoveIfPossible(Grid& grid, States side)
{
    auto& emptyCells = grid.getEmptyCells();
    for (auto i: emptyCells)
    {
        if (grid.isForkingMove(i.first, i.second, side))
        {
            makeComputerMove(grid, i.first, i.second, side);
            return true;
        }
    }
    return false;
}

bool blockAForkingMoveIfPossible(Grid& grid, States side)
{
    auto& emptyCells = grid.getEmptyCells();
    for (auto i: emptyCells)
    {
        if (grid.isForkingMove(i.first, i.second,
            (side == States::x ? States::o : States::x)))
        {
            makeComputerMove(grid, i.first, i.second, side);
            return true;
        }
    }
    return false;
}

bool makeImmediateAndForkingThreatIfPossible(Grid& grid, States side)
{
    auto& emptyCells = grid.getEmptyCells();
    for (auto i: emptyCells)
    {
        if ((grid.hasImmediateThreat(i.first, i.second, side)) && 
            (grid.countPotentialForks(i.first, i.second, side) >= 1))
        {
            makeComputerMove(grid, i.first, i.second, side);
            return true;
        }
    }
    return false;
}

bool blockImmediateAndForkingThreatIfPossible(Grid& grid, States side)
{
    auto& emptyCells = grid.getEmptyCells();
    for (auto i: emptyCells)
    {
        if ((grid.hasImmediateThreat(i.first, i.second,
                (side == States::x ? States::o : States::x))) && 
            (grid.countPotentialForks(i.first, i.second,
                (side == States::x ? States::o : States::x)) >= 1))
        {
            makeComputerMove(grid, i.first, i.second, side);
            return true;
        }
    }
    return false;
}

bool makeDoubleForkingThreatIfPossile(Grid& grid, States side)
{
    auto& emptyCells = grid.getEmptyCells();
    for (auto i: emptyCells)
    {
        if (grid.countPotentialForks(i.first, i.second, side) >= 2)
        {
            makeComputerMove(grid, i.first, i.second, side);
            return true;
        }
    }
    return false;
}

bool blockDoubleForkingThreatIfPossile(Grid& grid, States side)
{
    auto& emptyCells = grid.getEmptyCells();
    for (auto i: emptyCells)
    {
        if (grid.countPotentialForks(i.first, i.second,
            (side == States::x ? States::o : States::x)) >= 2)
        {
            makeComputerMove(grid, i.first, i.second, side);
            return true;
        }
    }
    return false;
}

bool makeSingleImmediateThreatIfPossible(Grid& grid, States side)
{
    auto& emptyCells = grid.getEmptyCells();
    for (auto i: emptyCells)
    {
        if (grid.hasImmediateThreat(i.first, i.second, side))
        {
            makeComputerMove(grid, i.first, i.second, side);
            return true;
        }
    }
    return false;
}

bool blockSingleImmediateThreatIfPossible(Grid& grid, States side)
{
    auto& emptyCells = grid.getEmptyCells();
    for (auto i: emptyCells)
    {
        if (grid.hasImmediateThreat(i.first, i.second,
            (side == States::x ? States::o : States::x)))
        {
            makeComputerMove(grid, i.first, i.second, side);
            return true;
        }
    }
    return false;
}

bool makeSingleForkingThreatIfPossible(Grid& grid, States side)
{
    auto& emptyCells = grid.getEmptyCells();
    for (auto i: emptyCells)
    {
        if (grid.countPotentialForks(i.first, i.second, side) >= 1)
        {
            makeComputerMove(grid, i.first, i.second, side);
            return true;
        }
    }
    return false;
}

bool blockSingleForkingThreatIfPossile(Grid& grid, States side)
{
    auto& emptyCells = grid.getEmptyCells();
    for (auto i: emptyCells)
    {
        if (grid.countPotentialForks(i.first, i.second,
            (side == States::x ? States::o : States::x)) >= 1)
        {
            makeComputerMove(grid, i.first, i.second, side);
            return true;
        }
    }
    return false;
}

bool makeBestScoringMoveIfPossible(Grid& grid, States side)
{
    std::vector<std::pair<char, int>> bestCells;
    int bestScore{-1};
    const auto& emptyCells = grid.getEmptyCells();
    for (const auto& [row, column] : emptyCells)
    {
        if (grid.isBlockedMove(row, column, side))
        {
            continue;
        }
        int score = grid.calculateDistanceScore(row, column, side);
        if (score > bestScore)
        {
            bestCells.clear();
            bestCells.push_back({row, column});
            bestScore = score;
        }
        else if (score == bestScore)
        {
            bestCells.push_back({row, column});
        }
    }
    if (!bestCells.empty())
    {
        auto [bestRow, bestColumn] = bestCells[generateRandomNumber(
            0, bestCells.size() - 1)];
        makeComputerMove(grid, bestRow, bestColumn, side);
        return true;
    }
    return false;
}

void computerMoveDifficluly1(Grid& grid, States side)
{
    makeRamdomMove(grid, side);
}

void computerMoveDifficluly2(Grid& grid, States side)
{
    if (makeAWinnigMoveIfPossible(grid, side))
    {
        return;
    }
    else if (blockAWinnigMoveIfPossible(grid, side))
    {
        return;
    }
    else 
    {
        makeRamdomMove(grid, side);
    }
}

void computerMoveDifficluly3(Grid& grid, States side)
{
    if (makeAWinnigMoveIfPossible(grid, side))
    {
        return;
    }
    else if (blockAWinnigMoveIfPossible(grid, side))
    {
        return;
    }
    else if (makeAForkingMoveIfPossible(grid, side))
    {
        return;
    }
    else if (blockAForkingMoveIfPossible(grid, side))
    {
        return;
    }
    else 
    {
        makeRamdomMove(grid, side);
    }
}

void computerMoveDifficluly4(Grid& grid, States side)
{
    if (makeAWinnigMoveIfPossible(grid, side))
    {
        return;
    }
    else if (blockAWinnigMoveIfPossible(grid, side))
    {
        return;
    }
    else if (makeAForkingMoveIfPossible(grid, side))
    {
        return;
    }
    else if (blockAForkingMoveIfPossible(grid, side))
    {
        return;
    }
    else if (makeBestScoringMoveIfPossible(grid, side))
    {
        return;
    }
    else 
    {
        makeRamdomMove(grid, side);
    }
}

void computerMoveDifficluly5(Grid& grid, States side)
{
    if (makeAWinnigMoveIfPossible(grid, side))
    {
        return;
    }
    else if (blockAWinnigMoveIfPossible(grid, side))
    {
        return;
    }
    else if (makeAForkingMoveIfPossible(grid, side))
    {
        return;
    }
    else if (blockAForkingMoveIfPossible(grid, side))
    {
        return;
    }
    else if (makeImmediateAndForkingThreatIfPossible(grid, side))
    {
        return;
    }
    else if (blockImmediateAndForkingThreatIfPossible(grid, side))
    {
        return;
    }
    else if (makeDoubleForkingThreatIfPossile(grid, side))
    {
        return;
    }
    else if (blockDoubleForkingThreatIfPossile(grid, side))
    {
        return;
    }
    else if (makeSingleImmediateThreatIfPossible(grid, side))
    {
        return;
    }
    else if (blockSingleImmediateThreatIfPossible(grid, side))
    {
        return;
    }
    else if (makeSingleForkingThreatIfPossible(grid, side))
    {
        return;
    }
    else if (blockSingleForkingThreatIfPossile(grid, side))
    {
        return;
    }
    else if (makeBestScoringMoveIfPossible(grid, side))
    {
        return;
    }
    else 
    {
        makeRamdomMove(grid, side);
    }
}

void computerMove(Grid& grid, States side, int difficulty)
{
    switch (difficulty)
    {
        case 1: computerMoveDifficluly1(grid, side); break;
        case 2: computerMoveDifficluly2(grid, side); break;
        case 3: computerMoveDifficluly3(grid, side); break;
        case 4: computerMoveDifficluly4(grid, side); break;
        case 5: computerMoveDifficluly5(grid, side); break;
        default: std::cerr << "Difficulty not implemented\n"; break;
    }
}

void play(Grid& grid, Score& score, States playerToken, int difficulty)
{
    for (int i{};; ++i)
    {
        if ((i % 2) == static_cast<int>(playerToken))
        {
            playerMove(grid, playerToken);
            std::cout << grid << '\n';
            if (grid.hasWon(playerToken))
            {
                std::cout << "You win!\n";
                score.win();
                return;
            }
        }
        else
        {
            computerMove(grid,
                (playerToken == States::x ? States::o : States::x), 
                    difficulty);
            std::cout << grid << '\n';
            if (grid.hasWon(playerToken == States::x ? States::o : States::x))
            {
                std::cout << "I won\n";
                score.lose();
                return;
            }
        }
        if (grid.isFull())
        {
            std::cout << "That's a draw\n";
            score.draw();
            return;
        }
    }
}

void playPvP(Grid& grid, PvPScore& score)
{
    for (int i{};; ++i)
    {
        if ((i % 2) == 0)
        {
            std::cout << score.getFirstPlayerName() << " moves\n";
            playerMove(grid, States::x);
            std::cout << grid << '\n';
            if (grid.hasWon(States::x))
            {
                std::cout << score.getFirstPlayerName() << " wins\n";
                score.firstPlayerWins();
                return;
            }
        }
        else
        {
            std::cout << score.getSecondPlayerName() << " moves\n";
            playerMove(grid, States::o);
            std::cout << grid << '\n';
            if (grid.hasWon(States::o))
            {
                std::cout << score.getSecondPlayerName() << " wins\n";
                score.secondPlayerWins();
                return;
            }
        }
        if (grid.isFull())
        {
            std::cout << "That's a draw\n";
            score.draw();
            return;
        }
    }
}