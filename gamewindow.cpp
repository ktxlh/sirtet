#include <QFont>
#include <string>
#include <QDebug>
#include <iostream>
#include <string>
#include <QString>
#include <QWidget>
#include <utility>

#include "gamewindow.h"
#include "gameinstance.h"
#include "ui_gamewindow.h"
#include "piece.h"

#include <typeinfo>
using namespace std;

gamewindow::gamewindow(QWidget *parent, GameInstance* _sirtet_game) :
    QWidget(parent),
    ui(new Ui::gamewindow),
    sirtet_game(_sirtet_game)
{
    setFocusPolicy( Qt::NoFocus );
    setFocus( Qt::PopupFocusReason );
    setEnabled( true );
    ui->setupUi(this);
    ui->gameOverLabel->setVisible(false);
    connect(this->ui->aiMoveButton, &QPushButton::clicked, this, &gamewindow::aiMoveButton_clicked_handler);
    make_grid();
}

gamewindow::~gamewindow() {
    delete ui;
    for (int i=0; i<20; i++)
        for (int j=0; j<10; j++)
            delete square[i][j];
}

void gamewindow::update_ui(){
    // Board
    int board[25][10];
    sirtet_game->get_board(board);
    for (int i=0; i<20; i++) {
        for (int j=0; j<10; j++) {
            this->square[i][j]->set_color(board[i][j]);
        }
    }


    // Next & Held
    int next = sirtet_game->get_next();
    int held = sirtet_game->get_held();

    try {
        ui -> nextPieceLabel -> setStyleSheet(QString::fromStdString("border-image: url(\":/resources/images/piece_" + to_string(next) + ".png\");"));
        ui -> heldPieceLabel  -> setStyleSheet(QString::fromStdString("border-image: url(\":/resources/images/piece_" + to_string(held) + ".png\");"));
    } catch (exception e) {
        cerr<<"ERROR: gamewindow::update_ui(): next="<<next<<", held="<<held<<". Exception: "<<e.what()<<endl;
    }

    // # Lines
    int value = sirtet_game->get_lines();
    ui -> linesLCD -> display(value);
    ui -> linesLCD -> show();
}

void gamewindow::show_game_over(){
    ui->gameOverLabel->setVisible(true);
    ui->gameOverLabel->raise();
    ui->aiMoveButton->setEnabled(false);
}

void gamewindow::closeEvent(QCloseEvent *event) {
    emit closed();
}

void gamewindow::keyPressEvent(QKeyEvent *event) {
    if(!sirtet_game->get_game_status()) return;
    switch (event->key()) {
    case Qt::Key_Up:
        sirtet_game->click_up(); break;
    case Qt::Key_Down:
        sirtet_game->click_down(); break;
    case Qt::Key_Left:
        sirtet_game->click_left(); break;
    case Qt::Key_Right:
        sirtet_game->click_right(); break;
    case 16777220:
        sirtet_game->click_space(); break;
    case Qt::Key_Shift:
        sirtet_game->click_shift(); break;
    }
}

void gamewindow::make_grid() {
    for (int i=0; i<20; i++) {
        for (int j=0; j<10; j++) {
            this->square[i][j] = new Square(this, i, j);
        }
    }
}

void gamewindow::aiMoveButton_clicked_handler(){
    this->sirtet_game->AI_move();
}

