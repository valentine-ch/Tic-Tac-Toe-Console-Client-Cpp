#include "io.h"

void clearInputBuffer()
{
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void printErrorMessage()
{
    std::cerr << "Invalid input, try again\n";
}

void invalidInput()
{
    std::cin.clear();
    clearInputBuffer();
    printErrorMessage();
}

template <typename T>
T getUserInput(std::string_view prompt)
{
    while (true)
    {
        if (!prompt.empty())
        {
            std::cout << prompt << '\n';
        }
        T input{};
        std::cin >> input;
        if (std::cin.fail())
        {
            invalidInput();
            continue;
        }
        else
        {
            clearInputBuffer();
            return input;
        }
    }
}

std::string getCellFromUser()
{
    return getUserInput<std::string>("Enter a cell");
}

char getChar(std::string_view prompt)
{
    return std::tolower(getUserInput<char>(prompt));
}

int getNumber(std::string_view prompt)
{
    return getUserInput<int>(prompt);
}

std::string getString(std::string_view prompt)
{
    return getUserInput<std::string>(prompt);
}

std::string getFirstPlayerName()
{
    return getUserInput<std::string>(
        "Enter first player's name (moves first)");
}

std::string getSecondPlayerName()
{
    return getUserInput<std::string>(
        "Enter second player's name (moves second)");
}

std::string getServerAdress()
{
    std::string input{getUserInput<std::string>("Enter server adress")};
    std::string toRemove{"https://"};
    if (input.compare(0, toRemove.length(), toRemove) == 0)
    {
        input.erase(0, toRemove.length());
    }
    return input;
}

std::pair<std::string, std::string> askForUserCredentials()
{
    std::string username{getUserInput<std::string>("Enter username")};
    std::string password{getUserInput<std::string>("Enter password")};
    return {username, password};
}

bool askIfPlayAgain()
{
    while (true)
    {
        char input{getChar("Would you like to try again? (y/n)")};
        switch (input)
        {
            case 'y': return true;
            case 'n': return false;
            default: printErrorMessage(); continue;
        }
    }
}

int askForDifficultyLevel()
{
    while (true)
    {
        int userInput{getNumber("Chose difficulty (1 or 5)")};
        if ((userInput >= 1) && (userInput <= 5))
        {
            return userInput;
        }
        else if (userInput == 404)
        {
            std::cout << "Difficulty not found\n";
        }
        printErrorMessage();
    }
}

States askForPlayerToken()
{
    while (true)
    {
        char userInput{getChar("Chose your token (X/O), X moves first")};
        switch (userInput)
        {
            case 'x': return States::x;
            case 'o': return States::o;
            default: printErrorMessage(); continue;
        }
    }
}

PlayAgainSchemes askForPlayAgainScheme()
{
    while (true)
    {
        char input{getChar("Choose a play again scheme: "
            "s - same, a - alternating, x - winner plays X, o - winner plays O")};
        switch (input)
        {
            case 's': return PlayAgainSchemes::same;
            case 'a': return PlayAgainSchemes::alternating;
            case 'x': return PlayAgainSchemes::winnerPlaysX;
            case 'o': return PlayAgainSchemes::winnerPlaysO;
            default: printErrorMessage(); continue;
        }
    }
}

std::pair<int, int> getGridProperties()
{
    while (true)
    {
        int size{getNumber("Enter the size of a grid (3 to 26)")};
        if ((size < minGridSize) || (size > maxGridSize))
        {
            printErrorMessage();
            continue;
        }
        if (size == minGridSize)
        {
            return {minGridSize, minGridSize};
        }
        while (true)
        {
            int winningLine{getNumber(
                "Enter the winnig line length (in between 3 and grid size)")};
            if ((winningLine >= minGridSize) && (winningLine <= size))
            {
                return {size, winningLine};
            }
            printErrorMessage();
        }
    }
}

void printMove(std::string_view whose, char row, int column)
{
    std::cout << whose << " move: row "
        << row << ", column " << column << '\n';
}