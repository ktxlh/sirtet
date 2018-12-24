#ifndef L_H
#define L_H
#include "piece.h"

class L: public Piece{
public:
    L(const int (&board)[25][10], int type = 6, int orientation = 0, pair<int, int> coordinate = make_pair(23, 4), bool alive = true);
    virtual void get_occupied_coordinates(pair<int,int> args[4]) const override;
    /*virtual void move_left() override;
    virtual void move_right() override;
    virtual void move_down() override;
    virtual void drop() override;
    virtual void rotate() override;*/
};

#endif // L_H
