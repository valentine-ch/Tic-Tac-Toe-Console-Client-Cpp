#include <iostream>
#include <utility>
#include "io.h"
#include "play.h"
#include "states.h"
#include "grid.h"
#include "score.h"
#include "pvpscore.h"
#include "help.h"
#include "constants.h"
#include "online.h"

Grid createGrid()
{
    auto gridProperties{getGridProperties()};
    return Grid{gridProperties.first, gridProperties.second};
}

void startSinglePlayer()
{
    Grid grid{createGrid()};
    States playerToken{askForPlayerToken()};
    int difficulty{askForDifficultyLevel()};
    Score score;
    do
    {
        std::cout << grid << '\n';
        play(grid, score, playerToken, difficulty);
        grid.clear();
        std::cout << score << '\n';
    }
    while (askIfPlayAgain());
}

void startPvP()
{
    Grid grid{createGrid()};
    PvPScore score{getFirstPlayerName(), getSecondPlayerName()};
    do
    {
        std::cout << grid << '\n';
        playPvP(grid, score);
        grid.clear();
        std::cout << score << '\n';
    }
    while (askIfPlayAgain());
}

void showHelp()
{
    std::cout << helpText << '\n';
}

void mainMenu()
{
    while (true)
    {
        char userAction{getChar(
            "Enter s for singleplayer, l for local multiplayer, "
            "o for online multiplayer, h for help or e to exit")};
        switch (userAction)
        {
            case 's': startSinglePlayer(); break;
            case 'l': startPvP(); break;
            case 'o': online(); break;
            case 'h': showHelp(); break;
            case 'e': return;
            default: printErrorMessage(); break;;
        }
    }
}

int main()
{
    // if std::cin.sync_with_stdio is true,
    // std::cin.rdbuf()->in_avail() always returns 0
    std::cin.sync_with_stdio(false);
    mainMenu();
    return 0;
}