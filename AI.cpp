#include <algorithm>
#include "AI.h"

std::map<std::string, std::pair<float, int>>AI::transpositions = {};

float AI::staticEval() {

    if (game.winner == game.curPlayer)
        return 100;


    return 0;
}

EvalResult AI::search(int depth, float alpha, float beta) {
    if (depth <= 0) {
        return EvalResult(staticEval());
    }

    float eval;
    Move bestMove = Move();
    for (Move m: getPossibleMoves()) {
        debugInfo.movesLookedAt += 1;

        // make the move
        game.grid[m.row][game.topSlots[m.row]] = m.player;
        game.topSlots[m.row]++;
        game.curPlayer = (game.curPlayer == Players::blue) ? Players::red : Players::blue;
        // evaluate

        if (game.checkWin(m) == game.curPlayer) {
            eval = beta;
        } else {
            eval = -search(depth - 1, alpha, -beta).eval;
        }

        // unmake Move
        game.grid[m.row][game.topSlots[m.row]] = m.player;
        game.topSlots[m.row]--;
        game.curPlayer = (game.curPlayer == Players::blue) ? Players::red : Players::blue;

        if (eval > alpha) {
            alpha = eval;
            bestMove = m;
            if (eval >= beta) {
                break;
            }
        }
    }
    return {bestMove, alpha};
}

std::vector<Move> AI::getPossibleMoves() {

    std::vector<Move> moves = {};

    for (int r = 0; r < GRID_SIZE_X; ++r) {
        if (game.topSlots[r] < GRID_SIZE_Y)
            moves.emplace_back(r, game.curPlayer);
    }

    return moves;
}

Move AI::findBestMove() {

    debugInfo.reset();

    EvalResult r = search(AI_SEARCH_DEPTH);

    debugInfo.printInfo();
    std::cout << "Move: " << r.move.row << "," << game.topSlots[r.move.row] << " - Eval: " << r.eval << std::endl;


    if (r.eval == -std::numeric_limits<float>::infinity()) {
        std::cout << "No valid move found!" << std::endl;
        return getPossibleMoves().front();
    }

    return r.move;
}


std::vector<std::string> AI::getTranspositions() {
    std::vector<std::string> returnVector = {"", ""};

    for (int x = 0; x < GRID_SIZE_X; ++x) {
        for (int y = 0; y < GRID_SIZE_Y; ++y) {
            returnVector[0] += (char) game.grid[x][y];
            returnVector[1] += (char) game.grid[GRID_SIZE_X - x - 1][y];
        }
    }
    returnVector[0] += (char) game.curPlayer;
    returnVector[1] += (char) game.curPlayer;

    return returnVector;
}

void AI::update(GameLogic _game) {
    this->game = _game;
}
