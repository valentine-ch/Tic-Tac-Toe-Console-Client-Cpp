#include "pvpscore.h"

PvPScore::PvPScore(std::string_view firstPlayerName, 
    std::string_view secondPlayerName)
        : m_firstPlayerName{firstPlayerName},
            m_secondPlayerName{secondPlayerName}
{
}

std::string_view PvPScore::getFirstPlayerName() {return m_firstPlayerName;}
std::string_view PvPScore::getSecondPlayerName() {return m_secondPlayerName;}
void PvPScore::firstPlayerWins() {++m_firstPlayerWins;}
void PvPScore::secondPlayerWins() {++m_secondPlayerWins;}
void PvPScore::draw() {++m_draws;}

std::ostream& operator<<(std::ostream& output, PvPScore score)
{
    output << score.m_firstPlayerName << " has won " << score.m_firstPlayerWins
        << " times, " << score.m_secondPlayerName << " has won " <<
        score.m_secondPlayerWins << " and there were " << score.m_draws <<
        " draws";
    return output;
}