#include "bullet.h"
#include "enemy.h"
#include <QTimer>
#include "mainwindow.h"
#include <QDebug>

#define bullet_speed 20
//Bullet::Bullet(QWidget *parent) : QLabel(parent)
//{

//}
Bullet::Bullet()
{
    //加载子弹图片
    QPixmap pix;
    pix.load(":/images/bullet2.png");
    setFixedSize(pix.width(),pix.height());

    //设置定时开火
    timer = new QTimer(this);
    Bullet_rect = new QRect();

    connect(timer,&QTimer::timeout,[=](){
        if(this->y()>-this->height())
        {
            Bullet_rect->moveTo(this->x(),this->y()-bullet_speed);           
            this->move(this->x(),this->y()-bullet_speed);
        }
        else
        {
            this->move(-this->x(),this->y());
            Bullet_rect->moveTo(-this->x(),this->y());
            this->is_free = true;
            timer->stop();
        }
    });
}
DoubleBullet::DoubleBullet()
{
    //加载子弹图片
    QPixmap pix;
    pix.load(":/images/cartridge_power.png");
    setFixedSize(pix.width(),pix.height());

    //设置定时开火
    timer = new QTimer(this);
    DoubleBullet_rect = new QRect();

    connect(timer,&QTimer::timeout,[=](){
        if(this->y()>-this->height())
        {
            DoubleBullet_rect->moveTo(this->x(),this->y()-bullet_speed);
            this->move(this->x(),this->y()-bullet_speed);
        }
        else
        {
            this->move(-this->x(),this->y());
            DoubleBullet_rect->moveTo(-this->x(),this->y());
            this->is_free = true;
            timer->stop();
        }
    });
}

void Bullet::out()
{
    Bullet_rect->moveTo(-this->x(),this->y());
    this->move(-this->x(),this->y());
    this->is_free = true;
    timer->stop();
}
void DoubleBullet::out()
{
    DoubleBullet_rect->moveTo(-this->x(),this->y());
    this->move(-this->x(),this->y());
    this->is_free = true;
    timer->stop();
}

