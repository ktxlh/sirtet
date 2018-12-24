#ifndef GAMEINSTANCE_H
#define GAMEINSTANCE_H

#include <QObject>

#include "piece.h"
#include <queue>
#include <cstdlib>
#include <algorithm>
#include <QTimer>


using namespace std;

class gamewindow;

class GameInstance : public QObject{
    Q_OBJECT
public:
    GameInstance();
    ~GameInstance();

    void startGraphicUI();

    void get_board(int arr[25][10]) const;
    int get_current_type() const;
    Piece const* get_active_piece() const;
    gamewindow* get_game_window() const;
    int get_lines() const;
    bool get_game_status() const;
    int get_next();
    int get_held() const;
    void click_left();
    void click_right();
    void click_down();
    void click_up();
    void click_space();
    void click_shift();
    void AI_move();


private:
    QTimer* timer;
    gamewindow* game_window;
    Piece* active_piece;
    int lines;
    int board[25][10];
    int next_on[7];
    int cursor;
    int hold_piece;
    bool clicked_hold;
    bool game_status;
    int clear_lines();
    void add_queue();
    void move_next_to_board();
    void move_piece_to_board(int piece_type);
    bool check_game_over() const;
    void refresh_board(bool update_ui);

    int AI_calculate_board_score(bool arr[25][10]) const;

private slots:
    void timer_handler();

};

#endif // GAMEINSTANCE_H
