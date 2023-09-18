#ifndef STRUCTURES_H
#define STRUCTURES_h

#include <string>

struct InivtationStatus
{
    std::string status;
    std::string gameId;
};

struct Invitation
{
    std::string id;
    std::string inviter;
    int gridSize;
    int winningLine;
    bool inviterPlayingX;
    std::string playAgainScheme;

    bool operator==(const Invitation& other) const
    {
        return (id == other.id);
    }

    bool operator!=(const Invitation& other) const
    {
        return !(id == other.id);
    }
};

struct PlayAgainStatus
{
    enum class Status
    {
        waiting, accepted, declined
    };

    Status status;
    std::string nextGameId;
    bool switchSides;
};

#endif