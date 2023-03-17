#include "AI.h"

std::map<std::string, std::pair<float, int>>AI::transpositions = {};

float AI::staticEval() {

    float eval = 0;

    // Count number of points per column
    for (int x = 0; x < GRID_SIZE_X; ++x) {
        int columCountBlue = 0;
        int columCountRed = 0;
        for (int y = 0; y < GRID_SIZE_Y; ++y) {
            if (game.grid[x][y] == game.curPlayer && columCountBlue < 4) {
                columCountBlue++;
            } else if (game.grid[x][y] != Players::neutral && columCountRed < 4) {
                columCountRed++;
            }
        }
        int columDif = columCountRed - columCountBlue;
        eval += (columDif < 0) ? -std::pow(2, -columDif) : std::pow(2, columDif);
    }
    // Count number of points per row
    for (int y = 0; y < GRID_SIZE_Y; ++y) {
        int rowCountBlue = 0;
        int rowCountRed = 0;
        for (int x = 0; x < GRID_SIZE_X; ++x) {
            if (game.grid[x][y] == game.curPlayer && rowCountBlue < 4) {
                rowCountBlue++;
            } else if (game.grid[x][y] == Players::neutral && rowCountRed < 4) {
                rowCountRed++;
            }
        }
        int rowDif = rowCountRed - rowCountBlue;
        eval += (rowDif < 0) ? -std::pow(2, -rowDif) : std::pow(2, rowDif);
    }


    return eval;
}

EvalResult AI::search(int depth, float alpha, float beta) {
    auto moves = getPossibleMoves();

    if (depth <= 0 || moves.empty()) {
        return EvalResult(staticEval());
    }


    EvalResult bestEval;
    for (Move m: moves) {
        debugInfo.movesLookedAt += 1;

//        std::cout << "checking move for " << (char) m.player;
        // make the move
        makeMove(m.row);

        EvalResult curEval;
        if (game.checkWin(m) != Players::neutral) {
//            printBoard();
            std::cout << "found win";
            curEval = {m, std::numeric_limits<float>::infinity()};
        } else {
//            std::cout << std::endl;
            curEval = EvalResult(m, -search(depth - 1, -alpha, -beta).eval);
//            std::cout << "Eval for other player: " << curEval.eval << std::endl;
        }

        // unmake Move
        unmakeMove(m.row);

        if (curEval.eval >= bestEval.eval) {
//            std::cout << "\tfound better move";
            bestEval = curEval;
            alpha = bestEval.eval;
            if (alpha >= beta) {
//                std::cout << "cutoff" << std::endl;
                //break;
            }
        }

//        std::cout << std::endl;
    }
    return bestEval;
}

std::vector<Move> AI::getPossibleMoves() {

    std::vector<Move> moves = {};

    for (int r = 0; r < GRID_SIZE_X; ++r) {
        if (game.topSlots[r] + 1 < GRID_SIZE_Y)
            moves.emplace_back(r, game.curPlayer);
    }

    return moves;
}

Move AI::findBestMove() {

    debugInfo.reset();

    std::cout << "Finding best move for: " << std::endl;
    printBoard();

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
    game.topSlots[row]++;
    game.grid[row][game.topSlots[row]] = game.curPlayer;
    game.curPlayer = (game.curPlayer == Players::blue) ? Players::red : Players::blue;
}

void AI::unmakeMove(int row) {
    game.grid[row][game.topSlots[row]] = Players::neutral;
    game.curPlayer = (game.curPlayer == Players::blue) ? Players::red : Players::blue;
    game.topSlots[row]--;
}
