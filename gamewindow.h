#ifndef gamewindow_H
#define gamewindow_H

#include <QWidget>
#include <QCloseEvent>
#include <QFont>

#include "square.h"

namespace Ui {
    class gamewindow;
}

class Square;
class GameInstance;

class gamewindow : public QWidget
{
    Q_OBJECT

public:
    explicit gamewindow(QWidget *parent = nullptr, GameInstance* _sirtet_game=nullptr);
    ~gamewindow() override;

    void update_ui();
    void show_game_over();

private:

    void closeEvent(QCloseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void make_grid();

    Ui::gamewindow *ui;
    Square* square[25][10];
    GameInstance* sirtet_game;

private slots:
    void aiMoveButton_clicked_handler();

signals:
    void closed();
};

#endif // gamewindow_H
