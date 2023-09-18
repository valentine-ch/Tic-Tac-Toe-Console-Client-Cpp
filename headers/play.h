#ifndef PLAY_H
#define PLAY_H

#include <iostream>
#include <array>
#include <string>
#include <sstream>
#include <random>
#include <ctime>
#include "states.h"
#include "grid.h"
#include "io.h"
#include "score.h"
#include "pvpscore.h"

// void playerMove(Grid& grid);
// int generateRandomNumber(int min, int max);
// void computerMove(Grid& grid);
void play(Grid& grid, Score& score, States playerToken, int difficulty);
void playPvP(Grid& grid, PvPScore& score);

#endif