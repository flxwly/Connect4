#ifndef CONNECT4_AI_H
#define CONNECT4_AI_H

#include <vector>
#include <map>
#include <cmath>
#include <iostream>
#include "GameLogic.h"

#define AI_SEARCH_DEPTH 16


struct EvalResult {
	explicit EvalResult(float eval) : move(), eval(eval) {};
	EvalResult(Move move, float eval) : move(move), eval(eval) {};

	Move move;
	float eval;
};

struct DebugInfo {
    DebugInfo() : movesLookedAt(0), transpositionsUsed(0), transpositionsAdded(0), transpositionsOverridden(0) {};
    DebugInfo(int movesLookedAt, int transpositionsUsed, int transpositionsAdded, int transpositionsOverridden) :
    movesLookedAt(movesLookedAt), transpositionsUsed(transpositionsUsed), transpositionsAdded(transpositionsAdded),
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

    void printInfo() {
        std::cout << "TranspositionsAdded: " << transpositionsAdded << std::endl
                  << "TranspositionsUpdated: " << transpositionsOverridden << std::endl
                  << "TranspositionsUsed: " << transpositionsUsed << std::endl
                  << "Looked at: " << movesLookedAt << " moves" << std::endl;
    }
};


class AI {
public:
    AI() = default;

    GameLogic game;
    void update(GameLogic _game);

    float staticEval();
	EvalResult search(int depth,
		float alpha = -std::numeric_limits<float>::infinity(),
		float beta = std::numeric_limits<float>::infinity());

    DebugInfo debugInfo;
	Move findBestMove();

    std::vector<Move> getPossibleMoves();

    std::vector<std::string> getTranspositions();
    static std::map<std::string, std::pair<float, int>> transpositions;

    void printBoard();
};


#endif //MEGATICTACTOE_AI_H
