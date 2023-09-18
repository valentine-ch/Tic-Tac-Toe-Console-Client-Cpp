#ifndef PLAY_AGAIN_SCHEMES_H
#define PLAY_AGAIN_SCHEMES_H

enum class PlayAgainSchemes
{
    same,
    alternating,
    winnerPlaysX,
    winnerPlaysO,
};

std::string playAgainSchemeToString(PlayAgainSchemes scheme);

#endif