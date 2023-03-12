#include "AI.h"

std::map<std::string, std::pair<float, int>>AI::transpositions = {};

float AI::staticEval() {

    float eval = 0;

    // Count number of points per column
    for (int x = 0; x < GRID_SIZE_X; ++x) {
        int columCount = 0;
        for (int y = 0; y < GRID_SIZE_Y; ++y) {
            if (game.grid[x][y] == Players::blue) {
                columCount++;
            } else if (game.grid[x][y] != Players::red) {
                columCount--;
            }
        }

        eval += (columCount < 0) ? -std::pow(2, -columCount) : std::pow(2, columCount);
    }
    // Count number of points per row
    for (int y = 0; y < GRID_SIZE_Y; ++y) {
        int rowCount = 0;
        for (int x = 0; x < GRID_SIZE_X; ++x) {
            if (game.grid[x][y] == Players::blue) {
                rowCount++;
            } else if (game.grid[x][y] != Players::red) {
                rowCount--;
            }
        }

        eval += (rowCount < 0) ? -std::pow(2, -rowCount) : std::pow(2, rowCount);
    }


    return eval;
}

EvalResult AI::search(int depth, float alpha, float beta) {
    if (depth <= 0) {
        return EvalResult(staticEval() * ((game.curPlayer == Players::blue) ? 1.0f : -1.0f));
    }

    EvalResult curEval;
    EvalResult bestEval;
    for (Move m: getPossibleMoves()) {
        debugInfo.movesLookedAt += 1;

        // make the move
        makeMove(m.row);

        // evaluate
        if (game.checkWin(m) == m.player) {
            std::cout << "Found winning move" << std::endl;
            printBoard();
            curEval = EvalResult(m, -beta);
        } else {
            curEval = EvalResult(m, -search(depth - 1, -alpha, -beta).eval);
        }
        // unmake Move
        unmakeMove(m.row);


        bestEval = std::max(bestEval, curEval);
        alpha = std::max(alpha, bestEval.eval);
        if (alpha >= beta)
            break;
    }
    return bestEval;
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

    //debugInfo.printInfo();
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

void AI::printBoard() {
    std::cout << "------board------" << std::endl;
    for (int y = GRID_SIZE_Y - 1; y >= 0; --y) {
        for (int x = 0; x < GRID_SIZE_X; ++x) {
            std::cout << (char) game.grid[x][y];
        }
        std::cout << std::endl;
    }
    std::cout << "-----------------" << std::endl;
}

void AI::makeMove(int row) {
    game.grid[row][game.topSlots[row]] = game.curPlayer;
    game.curPlayer = (game.curPlayer == Players::blue) ? Players::red : Players::blue;
    game.topSlots[row]++;
}

void AI::unmakeMove(int row) {
    game.topSlots[row]--;
    game.grid[row][game.topSlots[row]] = Players::neutral;
    game.curPlayer = (game.curPlayer == Players::blue) ? Players::red : Players::blue;
}
