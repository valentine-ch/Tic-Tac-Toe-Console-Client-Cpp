#include "score.h"

Score::Score(int wins, int draws, int losses)
    : m_wins{wins}, m_draws{draws}, m_losses{losses}                       
{
}

void Score::win() {++m_wins;}
void Score::draw() {++m_draws;}
void Score::lose() {++m_losses;}

std::ostream& operator<<(std::ostream& output, const Score& score)
{
    output << score.m_wins << " wins, " << score.m_losses << " losses, " <<
        score.m_draws << " draws";
    return output;
}