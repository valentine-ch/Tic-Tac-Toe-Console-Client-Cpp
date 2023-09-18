#ifndef SCORE_H
#define SCORE_H

#include <iostream>

class Score
{
private:
    int m_wins{};
    int m_draws{};
    int m_losses{};
public:
    Score(int wins = 0, int draws = 0, int losses = 0);
    void win();
    void draw();
    void lose();
    friend std::ostream& operator<<(std::ostream& output, const Score& score);
};

#endif