#include "gameinstance.h"
#include "gamewindow.h"
#include "piece.h"
#include "i.h"
#include "l.h"
#include "o.h"
#include "r.h"
#include "s.h"
#include "t.h"
#include "z.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <QTimer>
#include <QObject>
using namespace std;

GameInstance::GameInstance():
    game_window(new gamewindow(nullptr, this)),
    active_piece(nullptr),
    cursor(7),
    lines(0),
    game_status(1),
    hold_piece(0),
    clicked_hold(0),
    timer(new QTimer(this))
{
    srand(time(0));
    for(int i=0; i<7; i++) next_on[i] = 0;
    for(int i=0; i<25; i++)
    {
        for(int j=0; j<10; j++)
        {
            board[i][j] = 0;
        }
    }
    move_next_to_board();
    game_window->update_ui();
    connect(timer, SIGNAL(timeout()), this, SLOT(timer_handler()));
}

GameInstance::~GameInstance()
{
    delete game_window;
    game_window = nullptr;
}

void GameInstance::startGraphicUI()
{
    this->game_window->show();
    timer->start(800);
}

void GameInstance::get_board(int arr[25][10]) const
{
    for(int i=0; i<25; i++)
    {
        for(int j=0; j<10; j++)
        {
            if(board[i][j] == -1) arr[i][j] = active_piece->get_type();
            else arr[i][j] = board[i][j];
        }
    }

    pair<int, int> coordinates[4];
    active_piece->get_occupied_coordinates(coordinates);
    while(1)
    {
        if(coordinates[0].first < 0 || coordinates[1].first < 0 || coordinates[2].first < 0 || coordinates[3].first < 0
                || board[coordinates[0].first][coordinates[0].second] > 0
                || board[coordinates[1].first][coordinates[1].second] > 0
                || board[coordinates[2].first][coordinates[2].second] > 0
                || board[coordinates[3].first][coordinates[3].second] > 0)
        {
            coordinates[0].first++;
            coordinates[1].first++;
            coordinates[2].first++;
            coordinates[3].first++;
            break;
        }
        coordinates[0].first--;
        coordinates[1].first--;
        coordinates[2].first--;
        coordinates[3].first--;
    }
    for(int i=0; i<4; i++)
    {
        if(!arr[coordinates[i].first][coordinates[i].second]) arr[coordinates[i].first][coordinates[i].second] = -2;
    }

}

int GameInstance::get_current_type() const
{
    return active_piece->get_type();
}

gamewindow* GameInstance::get_game_window() const
{
    return game_window;
}

Piece const* GameInstance::get_active_piece() const
{
    return active_piece;
}

int GameInstance::get_lines() const
{
    return lines;
}

bool GameInstance::get_game_status() const
{
    return game_status;
}

int GameInstance::get_next()
{
    if(cursor == 7) add_queue();
    return next_on[cursor];
}

int GameInstance::get_held() const
{
    return hold_piece;
}

void GameInstance::click_left()
{
    active_piece->move_left();
    refresh_board(1);
}

void GameInstance::click_right()
{
    active_piece->move_right();
    refresh_board(1);
}

void GameInstance::click_down()
{
    active_piece->move_down();
    refresh_board(1);
}

void GameInstance::click_up()
{
    active_piece->rotate();
    refresh_board(1);
}

void GameInstance::click_space()
{
    while(active_piece->get_alive())
    {
        active_piece->move_down();
        if(!active_piece->get_alive()) break;
        refresh_board(0);
    }

    refresh_board(1);
}

void GameInstance::click_shift()
{
    if(clicked_hold) return;
    clicked_hold = 1;
    if(hold_piece == 0)
    {
        hold_piece = active_piece->get_type();
        for(int i=0; i<25; i++)
        {
            for(int j=0; j<10; j++)
            {
                if(board[i][j] == -1) board[i][j] = 0;
            }
        }
        delete active_piece;
        move_next_to_board();
    }
    else
    {
        int temp_type = hold_piece;
        hold_piece = active_piece->get_type();
        for(int i=0; i<25; i++)
        {
            for(int j=0; j<10; j++)
            {
                if(board[i][j] == -1) board[i][j] = 0;
            }
        }
        delete active_piece;
        move_piece_to_board(temp_type);
    }
    refresh_board(1);
}

void GameInstance::AI_move()
{
    bool current_board[25][10];
    for(int i=0; i<25; i++)
    {
        for(int j=0; j<10; j++)
        {
            if(board[i][j] == 0 || board[i][j] == -1) current_board[i][j] = 0;
            else current_board[i][j] = 1;
        }
    }

    int max_score = -(1<<30) - 1;
    int best_right = -1;
    int best_orientation = -1;
    int best_change = -1;
    for(int change = 0; change < 2; change++)
    {
        for(int right = 0; right < 10; right++)
        {
            for(int ori = 0; ori<4; ori++)
            {
                pair<int, int> blocks[4];
                int current_type = active_piece->get_type();
                if(change == 1)
                {
                    if(hold_piece) current_type = hold_piece;
                    else current_type = get_next();
                }
                int current_orientation = ori;
                switch (current_type) {
                case 1:
                    if(current_orientation % 2 == 0)
                    {
                        blocks[0] = make_pair(24, 0);
                        blocks[1] = make_pair(23, 0);
                        blocks[2] = make_pair(22, 0);
                        blocks[3] = make_pair(21, 0);
                    }
                    else
                    {
                        blocks[0] = make_pair(24, 0);
                        blocks[1] = make_pair(24, 1);
                        blocks[2] = make_pair(24, 2);
                        blocks[3] = make_pair(24, 3);
                    }
                    break;
                case 2:
                    blocks[0] = make_pair(24, 0);
                    blocks[1] = make_pair(24, 1);
                    blocks[2] = make_pair(23, 0);
                    blocks[3] = make_pair(23, 1);
                    break;
                case 3:
                    if(current_orientation == 0)
                    {
                        blocks[0] = make_pair(24, 1);
                        blocks[1] = make_pair(23, 0);
                        blocks[2] = make_pair(23, 1);
                        blocks[3] = make_pair(23, 2);
                    }
                    else if(current_orientation == 1)
                    {
                        blocks[0] = make_pair(24, 0);
                        blocks[1] = make_pair(23, 0);
                        blocks[2] = make_pair(23, 1);
                        blocks[3] = make_pair(22, 0);
                    }
                    else if(current_orientation == 2)
                    {
                        blocks[0] = make_pair(24, 0);
                        blocks[1] = make_pair(24, 1);
                        blocks[2] = make_pair(24, 2);
                        blocks[3] = make_pair(23, 1);
                    }
                    else
                    {
                        blocks[0] = make_pair(24, 1);
                        blocks[1] = make_pair(23, 0);
                        blocks[2] = make_pair(23, 1);
                        blocks[3] = make_pair(22, 1);
                    }
                    break;
                case 4:
                    if(current_orientation % 2 == 0)
                    {
                        blocks[0] = make_pair(24, 1);
                        blocks[1] = make_pair(24, 2);
                        blocks[2] = make_pair(23, 0);
                        blocks[3] = make_pair(23, 1);
                    }
                    else
                    {
                        blocks[0] = make_pair(24, 0);
                        blocks[1] = make_pair(23, 0);
                        blocks[2] = make_pair(23, 1);
                        blocks[3] = make_pair(22, 1);
                    }
                    break;
                case 5:
                    if(current_orientation % 2 == 0)
                    {
                        blocks[0] = make_pair(24, 0);
                        blocks[1] = make_pair(24, 1);
                        blocks[2] = make_pair(23, 1);
                        blocks[3] = make_pair(23, 2);
                    }
                    else
                    {
                        blocks[0] = make_pair(24, 1);
                        blocks[1] = make_pair(23, 0);
                        blocks[2] = make_pair(23, 1);
                        blocks[3] = make_pair(22, 0);
                    }
                    break;
                case 6:
                    if(current_orientation == 0)
                    {
                        blocks[0] = make_pair(24, 0);
                        blocks[1] = make_pair(23, 0);
                        blocks[2] = make_pair(22, 0);
                        blocks[3] = make_pair(22, 1);
                    }
                    else if(current_orientation == 1)
                    {
                        blocks[0] = make_pair(24, 0);
                        blocks[1] = make_pair(24, 1);
                        blocks[2] = make_pair(24, 2);
                        blocks[3] = make_pair(23, 0);
                    }
                    else if(current_orientation == 2)
                    {
                        blocks[0] = make_pair(24, 0);
                        blocks[1] = make_pair(24, 1);
                        blocks[2] = make_pair(23, 1);
                        blocks[3] = make_pair(22, 1);
                    }
                    else
                    {
                        blocks[0] = make_pair(24, 2);
                        blocks[1] = make_pair(23, 0);
                        blocks[2] = make_pair(23, 1);
                        blocks[3] = make_pair(23, 2);
                    }
                    break;
                case 7:
                    if(current_orientation == 0)
                    {
                        blocks[0] = make_pair(24, 0);
                        blocks[1] = make_pair(24, 1);
                        blocks[2] = make_pair(23, 0);
                        blocks[3] = make_pair(22, 0);
                    }
                    else if(current_orientation == 1)
                    {
                        blocks[0] = make_pair(24, 0);
                        blocks[1] = make_pair(24, 1);
                        blocks[2] = make_pair(24, 2);
                        blocks[3] = make_pair(23, 2);
                    }
                    else if(current_orientation == 2)
                    {
                        blocks[0] = make_pair(24, 1);
                        blocks[1] = make_pair(23, 1);
                        blocks[2] = make_pair(22, 0);
                        blocks[3] = make_pair(22, 1);
                    }
                    else
                    {
                        blocks[0] = make_pair(24, 0);
                        blocks[1] = make_pair(23, 0);
                        blocks[2] = make_pair(23, 1);
                        blocks[3] = make_pair(23, 2);
                    }
                    break;
                }


                bool temp_board[25][10];
                for(int i=0; i<25; i++)
                {
                    for(int j=0; j<10; j++)
                    {
                        temp_board[i][j] = 0;
                    }
                }
                for(int i=0; i<right; i++)
                {
                    if(blocks[0].second < 9 && blocks[1].second < 9 && blocks[2].second < 9 && blocks[3].second < 9)
                    {
                        blocks[0].second++;
                        blocks[1].second++;
                        blocks[2].second++;
                        blocks[3].second++;
                    }
                }


                while(1)
                {
                    if(blocks[0].first < 0 || blocks[1].first < 0 || blocks[2].first < 0 || blocks[3].first < 0
                            || current_board[blocks[0].first][blocks[0].second]
                            || current_board[blocks[1].first][blocks[1].second]
                            || current_board[blocks[2].first][blocks[2].second]
                            || current_board[blocks[3].first][blocks[3].second])
                    {
                        blocks[0].first++;
                        blocks[1].first++;
                        blocks[2].first++;
                        blocks[3].first++;
                        break;
                    }
                    blocks[0].first--;
                    blocks[1].first--;
                    blocks[2].first--;
                    blocks[3].first--;
                }

                for(int i=0; i<4; i++) temp_board[blocks[i].first][blocks[i].second] = 1;
                for(int i=0; i<25; i++)
                {
                    for(int j=0; j<10; j++)
                    {
                        if(current_board[i][j]) temp_board[i][j] = 1;
                    }
                }
                int score = AI_calculate_board_score(temp_board);

                if(score > max_score)
                {
                    best_right = right;
                    best_orientation = ori;
                    best_change = change;
                    max_score = score;
                }
            }
        }
    }

    if(best_change)
    {
        click_shift();
        return;
    }

    while(active_piece->get_orientation()!=best_orientation)
    {
        active_piece->rotate();
        refresh_board(0);
    }
    for(int i=0; i<10; i++)
    {
        active_piece->move_left();
        refresh_board(0);
    }
    for(int i=0; i<best_right; i++)
    {
        active_piece->move_right();
        refresh_board(0);
    }
    click_space();

}

int GameInstance::clear_lines()
{
    int ret = 0;
    int row = 0;
    while(row < 25)
    {
        int cnt = 0;
        for(int j=0; j<10; j++)
        {
            if(board[row][j]!=0) cnt++;
        }
        if(cnt == 10)
        {
            ret++;
            for(int i=row; i<24; i++)
            {
                for(int j=0; j<10; j++)
                {
                    board[i][j] = board[i+1][j];
                }
            }
            for(int j=0; j<10; j++)
            {
                board[24][j] = 0;
            }
        }
        else row++;
    }
    return ret;
}

void GameInstance::add_queue()
{
    int temp[7] = {1, 2, 3, 4, 5, 6, 7};
    random_shuffle(temp, temp + 7);
    for(int i=0; i<7; i++)
    {
        next_on[i] = temp[i];
    }
    cursor = 0;
}

void GameInstance::move_next_to_board()
{
    move_piece_to_board(get_next());
    cursor++;
}

void GameInstance::move_piece_to_board(int piece_type)
{
    int target = piece_type;
    switch (target) {
    case 1:
        active_piece = new I(board, target);
        break;
    case 2:
        active_piece = new O(board, target);
        break;
    case 3:
        active_piece = new T(board, target);
        break;
    case 4:
        active_piece = new S(board, target);
        break;
    case 5:
        active_piece = new Z(board, target);
        break;
    case 6:
        active_piece = new L(board, target);
        break;
    case 7:
        active_piece = new R(board, target);
        break;
    }

    pair<int, int> temp[4];
    active_piece->get_occupied_coordinates(temp);
    for(int i=0; i<4; i++)
    {
        board[temp[i].first][temp[i].second] = -1;
    }
}

bool GameInstance::check_game_over() const
{
    for(int i=20; i<25; i++)
    {
        for(int j=0; j<10; j++)
        {
            if(board[i][j]) return 1;
        }
    }
    return 0;

}

void GameInstance::refresh_board(bool update_ui)
{
    if(active_piece->get_alive())
    {
        for(int i=0; i<25; i++)
        {
            for(int j=0; j<10; j++)
            {
                if(board[i][j] == -1) board[i][j] = 0;
            }
        }
        pair<int, int> coordinates[4];
        active_piece->get_occupied_coordinates(coordinates);
        for(int i=0; i<4; i++)
        {
            board[coordinates[i].first][coordinates[i].second] = -1;
        }

    }
    else
    {
        clicked_hold = 0;
        for(int i=0; i<25; i++)
        {
            for(int j=0; j<10; j++)
            {
                if(board[i][j] == -1) board[i][j] = active_piece->get_type();
            }
        }
        delete active_piece;
        active_piece = nullptr;
        if(check_game_over())
        {
            game_status = 0;
            game_window->show_game_over();

            return;
        }
        lines += clear_lines();
        move_next_to_board();
    }
    if(update_ui) game_window->update_ui();
}

int GameInstance::AI_calculate_board_score(bool arr[25][10]) const
{

    for(int i=20; i<25; i++)
    {
        for(int j=0; j<10; j++)
        {
            if(arr[i][j]!=0) return -(1<<30);
        }
    }

    int height[10];
    for(int j=0; j<10; j++)
    {
        int acc_height = -1;
        for(int i = 24; i>=0; i--)
        {
            if(arr[i][j]!=0)
            {
                acc_height = i;
                break;
            }
        }
        height[j] = acc_height + 1;
    }
    int aggregate_height = 0;
    for(int j=0; j<10; j++)
    {
        aggregate_height += height[j];
    }
    int complete_lines = 0;
    for(int i=0; i<25; i++)
    {
        int cnt = 0;
        for(int j=0; j<10; j++)
        {
            if(arr[i][j]!=0) cnt++;
        }
        if(cnt == 10) complete_lines++;
    }
    int holes = 0;
    for(int j=0; j<10; j++)
    {
        for(int i=0; i<height[j]-1; i++)
        {
            if(arr[i][j] == 0) holes++;
        }
    }
    int bumpiness = 0;
    for(int j=0; j<9; j++)
    {
        bumpiness += abs(height[j] - height[j+1]);
    }
    int score = (-510066) * aggregate_height + (760666) * complete_lines
                + (-356630) * holes + (-184483) * bumpiness;

    return score;
}

void GameInstance::timer_handler()
{
    if(!game_status) timer->stop();
    else click_down();
}
