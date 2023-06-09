#include "GameLogic.h"
#include "GameWindow.h"


Players GameLogic::checkWin(Move lastMove) {
    // check diagonal (top left to bottom right)
    int row = lastMove.row;
    int colum = topSlots[row];
    int markCount = -1;
    while(row >= 0 && colum < GRID_SIZE_Y && grid[row][colum] == lastMove.player) {
        markCount++;
        row--;
        colum++;
    }
    row = lastMove.row;
    colum = topSlots[row];
    while(row < GRID_SIZE_X && colum >= 0 && grid[row][colum] == lastMove.player) {
        markCount++;
        row++;
        colum--;
    }
    if (markCount >= MARKS_TO_WIN)
        return lastMove.player;

    // check diagonal (top right to bottom left)
    row = lastMove.row;
    colum = topSlots[row];
    markCount = -1;
    while(row >= 0 && colum >= 0 && grid[row][colum] == lastMove.player) {
        markCount++;
        row--;
        colum--;
    }
    row = lastMove.row;
    colum = topSlots[row];
    while(row < GRID_SIZE_X && colum < GRID_SIZE_Y && grid[row][colum] == lastMove.player) {
        markCount++;
        row++;
        colum++;
    }
    if (markCount >= MARKS_TO_WIN)
        return lastMove.player;

    // check horizontal
    row = lastMove.row;
    colum = topSlots[row];
    markCount = -1;
    while(row >= 0 && grid[row][colum] == lastMove.player) {
        markCount++;
        row--;
    }
    row = lastMove.row;
    colum = topSlots[row];
    while(row < GRID_SIZE_X && grid[row][colum] == lastMove.player) {
        markCount++;
        row++;
    }
    if (markCount >= MARKS_TO_WIN)
        return lastMove.player;

    // check vertical
    row = lastMove.row;
    colum = topSlots[row];
    markCount = -1;
    while(colum >= 0 && grid[row][colum] == lastMove.player) {
        markCount++;
        colum--;
    }
    row = lastMove.row;
    colum = topSlots[row];
    while(colum < GRID_SIZE_X && grid[row][colum] == lastMove.player) {
        markCount++;
        colum++;
    }
    if (markCount >= MARKS_TO_WIN)
        return lastMove.player;

    return Players::neutral;
}

void GameLogic::update(int row) {
    if (row < 0 || row >= GRID_SIZE_X)
        return;

    if (topSlots[row] + 1 < GRID_SIZE_Y) {
        topSlots[row]++;
        grid[row][topSlots[row]] = curPlayer;
        winner = checkWin(Move(row, curPlayer));
        curPlayer = (curPlayer == Players::blue) ? Players::red : Players::blue;
    }
}

void GameLogic::init() {
    curPlayer = Players::blue;
    winner = Players::neutral;
    for (int x = 0; x < GRID_SIZE_X; ++x) {
        topSlots[x] = -1;
        for (int y = 0; y < GRID_SIZE_Y; ++y) {
            grid[x][y] = Players::neutral;
        }
    }
}



