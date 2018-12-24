#ifndef PIECE_H
#define PIECE_H

#include <utility>
using namespace std;

class Piece{
public:

    Piece(const int (&board)[25][10], int type, int orientation = 0, pair<int, int> coordinate = make_pair(20, 4), bool alive = true);
    virtual ~Piece()=default;

    int get_type() const;
    int get_orientation() const;
    bool get_alive() const;
    pair<int, int> get_coordinate() const;
    virtual void get_occupied_coordinates(pair<int,int> args[4]) const = 0;

    void move_left();
    void move_right();
    void move_down();
    void drop();
    void rotate();

protected:
    const int (&board) [25][10];
    int type;
    int orientation;
    pair<int, int> coordinate;
    bool alive;
};

#endif // PIECE_H
