#ifndef CONNECT4_AI_H
#define CONNECT4_AI_H

#include <vector>
#include <map>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include "GameLogic.h"

#define AI_SEARCH_DEPTH 8


struct EvalResult {
    EvalResult() : move(-1, Players::neutral), eval(-std::numeric_limits<float>::infinity()) {};
    explicit EvalResult(float eval) : move(-1, Players::neutral), eval(eval) {};
    EvalResult(Move move, float eval) : move(move), eval(eval) {};

    Move move;
    float eval = NAN;

    friend bool operator<(const EvalResult &lhs, const EvalResult &rhs) { return lhs.eval < rhs.eval; };
    friend bool operator>(const EvalResult &lhs, const EvalResult &rhs) { return lhs.eval > rhs.eval; };
    friend bool operator<=(const EvalResult &lhs, const EvalResult &rhs) { return lhs.eval <= rhs.eval; };
    friend bool operator>=(const EvalResult &lhs, const EvalResult &rhs) { return lhs.eval >= rhs.eval; };
    friend bool operator==(const EvalResult &lhs, const EvalResult &rhs) { return lhs.eval == rhs.eval; };
    friend bool operator!=(const EvalResult &lhs, const EvalResult &rhs) { return lhs.eval != rhs.eval; }
};

struct DebugInfo {
    DebugInfo() : movesLookedAt(0), transpositionsUsed(0), transpositionsAdded(0), transpositionsOverridden(0) {};

    DebugInfo(int movesLookedAt, int transpositionsUsed, int transpositionsAdded, int transpositionsOverridden) :
            movesLookedAt(movesLookedAt), transpositionsUsed(transpositionsUsed),
            transpositionsAdded(transpositionsAdded),
            transpositionsOverridden(transpositionsOverridden) {};

    int movesLookedAt = 0;
    int transpositionsUsed = 0;
    int transpositionsAdded = 0;
    int transpositionsOverridden = 0;

    void reset() {
        movesLookedAt = 0;
        transpositionsUsed = 0;
        transpositionsAdded = 0;
        transpositionsOverridden = 0;
    }

    void printInfo() const {
        std::cout << "TranspositionsAdded: " << transpositionsAdded << std::endl
                  << "TranspositionsUpdated: " << transpositionsOverridden << std::endl
                  << "TranspositionsUsed: " << transpositionsUsed << std::endl
                  << "Looked at: " << movesLookedAt << " moves" << std::endl;
    }
};


class AI{
public:
    AI() = default;

    GameLogic game;

    void makeMove(int row);
    void unmakeMove(int row);

    void update(GameLogic _game);

    float staticEval();

    EvalResult search(int depth,
                      float alpha = -std::numeric_limits<float>::infinity(),
                      float beta = std::numeric_limits<float>::infinity());

    DebugInfo debugInfo;

    Move findBestMove();

    std::vector<Move> getPossibleMoves();
    void printBoard();


    std::vector<std::string> getTranspositions();

    static std::map<std::string, std::pair<float, int>> transpositions;
};


#endif //MEGATICTACTOE_AI_H
