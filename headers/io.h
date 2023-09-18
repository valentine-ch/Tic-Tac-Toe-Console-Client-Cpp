#ifndef IO_H
#define IO_H

#include <iostream>
#include <string>
#include <limits>
#include <string_view>
#include "constants.h"
#include "states.h"
#include "playagainschemes.h"

void clearInputBuffer();
void printErrorMessage();
void invalidInput();
std::string getCellFromUser();
char getChar(std::string_view prompt);
int getNumber(std::string_view prompt);
std::string getString(std::string_view prompt);
std::string getFirstPlayerName();
std::string getSecondPlayerName();
std::pair<std::string, std::string> askForUserCredentials();
std::string getServerAdress();
std::pair<int, int> getGridProperties();
bool askIfPlayAgain();
int askForDifficultyLevel();
States askForPlayerToken();
PlayAgainSchemes askForPlayAgainScheme();
void printMove(std::string_view whose, char row, int column);

#endif