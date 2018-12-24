// haha

#include "r.h"

R::R(const int (&board)[25][10], int type, int orientation , pair<int, int> coordinate , bool alive):
    Piece(board, type, orientation, coordinate, alive){
    for(int i=0; i<5; i++)
    {
        if(alive) move_down();
        else break;
    }
}

void R::get_occupied_coordinates(pair<int, int> args[4]) const {
    switch (orientation) {
    case 0:
        //        o o
        //        @
        //        o
        args[0] = make_pair(coordinate.first+1,coordinate.second+1);
        args[1] = make_pair(coordinate.first+1,coordinate.second);
        args[2] = make_pair(coordinate.first,coordinate.second);
        args[3] = make_pair(coordinate.first-1,coordinate.second);
        break;
    case 1:
        //        o @ o
        //            o
        args[0] = make_pair(coordinate.first,coordinate.second-1);
        args[1] = make_pair(coordinate.first,coordinate.second);
        args[2] = make_pair(coordinate.first,coordinate.second+1);
        args[3] = make_pair(coordinate.first-1,coordinate.second+1);
        break;
    case 2:
        //          o
        //          @
        //        o o
        args[0] = make_pair(coordinate.first+1,coordinate.second);
        args[1] = make_pair(coordinate.first,coordinate.second);
        args[2] = make_pair(coordinate.first-1,coordinate.second);
        args[3] = make_pair(coordinate.first-1,coordinate.second-1);
        break;
    case 3:
        //        o
        //        o @ o
        args[0] = make_pair(coordinate.first+1,coordinate.second-1);
        args[1] = make_pair(coordinate.first,coordinate.second-1);
        args[2] = make_pair(coordinate.first,coordinate.second);
        args[3] = make_pair(coordinate.first,coordinate.second+1);
        break;
    }
}

