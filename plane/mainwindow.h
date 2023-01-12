#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include "bullet.h"
#include "enemy.h"
#include "supply.h"
#include <QRect>
#include <QPainter>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

#define Speed 25
#define bullet_num 60
#define low_enemy_num 30
#define mid_enemy_num 15
#define high_enemy_num 5
#define all_enemy_num 50
#define doublefile_num 100

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void paintEvent(QPaintEvent *);

    void keyPressEvent(QKeyEvent *e);


    QLabel *plane;
    QRect *plane_rect;

    Bullet *bullet_label[bullet_num];

    DoubleBullet *doublebullet_label[doublefile_num];

    enemy *enemy_plane[all_enemy_num];

    supply_file *supfile;
    supply_boom *subboom;

    bool DoubleFile_mode = false;

private:
    Ui::MainWindow *ui;

    int score=0;

signals:
    void filewall();
    void game_over();
};
#endif // MAINWINDOW_H
