#include "piece.h"
#include <iostream>

using namespace std;


Piece::Piece(const int (&board)[25][10], int type, int orientation, pair<int, int> coordinate, bool alive):
    board(board),
    type(type),
    orientation(orientation),
    coordinate(coordinate),
    alive(alive)
{

};

int Piece::get_type() const{
    return this->type;
}

int Piece::get_orientation() const{
    return this->orientation;
}

bool Piece::get_alive() const{
    return this->alive;
}

pair<int, int> Piece::get_coordinate() const{
    return this->coordinate;
}

void Piece::move_left(){
    // 1. Moving doesn't make it out of boundary
    // 2. Moving doesn't make cover occupied squares
    pair<int, int> occupied[4];
    get_occupied_coordinates(occupied);

    // Left <==> .second-1
    bool invalid=false;
    for(int j=0; j<4 && !invalid; ++j){
        int new_y = occupied[j].first,
            new_x = occupied[j].second-1;
        if(new_x<0 || board[new_y][new_x]>0) invalid=true;
    }
    if(!invalid) --coordinate.second;
}

void Piece::move_right(){
    pair<int, int> occupied[4];
    get_occupied_coordinates(occupied);

    // Right <==> .second+1
    bool invalid=false;
    for(int j=0; j<4 && !invalid; ++j){
        int new_y = occupied[j].first,
            new_x = occupied[j].second+1;
        if(new_x>=10 || board[new_y][new_x]>0) invalid=true;
    }
    if(!invalid) ++coordinate.second;
}

void Piece:: move_down(){                   // 判斷電死
    pair<int, int> occupied[4];
    get_occupied_coordinates(occupied);

    // Down <==> .first-1
    for(int j=0; j<4 && alive; ++j){
        int new_y = occupied[j].first-1,
            new_x = occupied[j].second;
        if(new_y<0 || board[new_y][new_x]>0) alive=false;
    }
    if(alive) --coordinate.first;
}

void Piece::drop(){                         // 直接電死
    while(alive)
    {
        move_down();
    }

}

void Piece::rotate(){
    {
        orientation = (orientation+1)%4;        // Do
        pair<int, int> new_occupied[4];
        get_occupied_coordinates(new_occupied);
        bool invalid = false;
        for(int j=0; j<4 && !invalid; ++j){
            int new_y = new_occupied[j].first,
                new_x = new_occupied[j].second;
            if(new_x<0 || new_x>=10 || new_y<0 || new_y>=25 || board[new_y][new_x]>0) invalid=true;
        }
        if(!invalid) return;
        else orientation = (orientation+4-1)%4;      // Undo
    }

    // Invalid. Try moving left or right by 1
    int try_this_order[4] = {-1, 1, -2, 2};
    int num_of_try = 2;
    if(type == 1) num_of_try = 4;
    for(int j=0; j< num_of_try; ++j){
        int dx = try_this_order[j];

        coordinate.second += dx;                // Do
        orientation = (orientation+1)%4;        // Do
        pair<int, int> new_occupied[4];
        get_occupied_coordinates(new_occupied);
        bool invalid = false;
        for(int j=0; j<4 && !invalid; ++j){
            int new_y = new_occupied[j].first,
                new_x = new_occupied[j].second;
            if(new_x<0 || new_x>=10 || new_y<0 || new_y>=25 || board[new_y][new_x]>0) invalid=true;
        }
        if(!invalid) return;
        else{
            orientation = (orientation+4-1)%4;      // Undo
            coordinate.second -= dx;                // Undo
        }
    }
}
