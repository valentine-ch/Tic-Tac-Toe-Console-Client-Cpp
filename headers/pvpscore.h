#ifndef PVP_SCORE_H
#define PVP_SCORE_H

#include <string>
#include <string_view>
#include <iostream>

class PvPScore
{
private:
    std::string m_firstPlayerName{};
    std::string m_secondPlayerName{};
    int m_firstPlayerWins{};
    int m_secondPlayerWins{};
    int m_draws{};
public:
    PvPScore(std::string_view firstPlayerName, std::string_view secondPlayerName);
    std::string_view getFirstPlayerName();
    std::string_view getSecondPlayerName();
    void firstPlayerWins();
    void secondPlayerWins();
    void draw();
    friend std::ostream& operator<<(std::ostream& output, PvPScore score);
};

#endif