#include <string>
#include "playagainschemes.h"

std::string playAgainSchemeToString(PlayAgainSchemes scheme)
{
    switch (scheme)
    {
        case PlayAgainSchemes::same: return "same";
        case PlayAgainSchemes::alternating: return "alternating";
        case PlayAgainSchemes::winnerPlaysX: return "winner_plays_x";
        case PlayAgainSchemes::winnerPlaysO: return "winner_plays_o";
        default: return "unknown";
    }
}