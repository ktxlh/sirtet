#include <QFont>
#include <QFontDatabase>
#include <QObject>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "gameinstance.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    sirtet_game(nullptr)
{
    ui->setupUi(this);
    connect(this->ui->startButton, &QPushButton::clicked, this, &MainWindow::startButton_clicked_handler);
    ui->backgroundLabel -> setStyleSheet(QString::fromStdString("border-image: url(\":/resources/images/mainwindow_bg.png\");"));
}

MainWindow::~MainWindow()
{
    if (this->sirtet_game != nullptr) delete this->sirtet_game;

    delete this->ui;
}

void MainWindow::startButton_clicked_handler()
{
    if (this->sirtet_game) delete this->sirtet_game;
    this->sirtet_game = new GameInstance;
    this->sirtet_game->startGraphicUI();
    connect(this->sirtet_game->get_game_window(), &gamewindow::closed, this, &MainWindow::game_window_closed_handler);
    this->hide();
}

void MainWindow::game_window_closed_handler() {
    this->show();
}
