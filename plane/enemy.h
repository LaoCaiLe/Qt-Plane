#ifndef ENEMY_H
#define ENEMY_H

#include <QLabel>
#include <QRect>
class enemy : public QLabel
{
    Q_OBJECT
public:
//    explicit enemy(QWidget *parent = nullptr);
    enemy(QString img);
    void enemy_die();
    void enemy_reset();

    void enemy_hurt();

    int speed;  //敌机速度
    int hp;   //敌机生命
    int id;

    int start_x;
    int start_y;

    int i=2;
    QTimer *enemy_timer;
    QRect *enemy_rect;

    bool Is_exist=true;

signals:
    void add_score();

};

#endif // ENEMY_H
