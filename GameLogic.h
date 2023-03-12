#ifndef CONNECT4_GAMELOGIC_H
#define CONNECT4_GAMELOGIC_H

#include <array>

#define GRID_SIZE_X 7
#define GRID_SIZE_Y 6
#define MARKS_TO_WIN 4

enum Players {
    neutral = '#', blue = 'b', red = 'r'
};


struct Move {
    Move() = default;
    Move(int _row, Players _player) : row(_row), player(_player) {};

    int row;
    Players player;
};

typedef std::array<std::array<Players, GRID_SIZE_Y>, GRID_SIZE_X> Grid;
typedef std::array<int, GRID_SIZE_X> TopSlots;

class GameLogic {
public:
    GameLogic() { init(); };

    void init();

    void update(int row);


    Players winner = Players::neutral;
    Players curPlayer = Players::blue;


    TopSlots topSlots{};
    Grid grid{}; // default all 0


    Players checkWin(Move lastMove);
};


#endif //MEGATICTACTOE_GAMELOGIC_H
