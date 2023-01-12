#include "enemy.h"
#include "mainwindow.h"
#include <QDebug>
#include <QtGlobal>
#include <QTime>
#include <QTimer>
#include <QMovie>
enemy::enemy(QString img)
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    QPixmap pix;
    bool ret = pix.load(img);
    if(!ret) qDebug()<<img<<"图片加载失败";

    setFixedSize(pix.width(),pix.height());

    enemy_rect = new QRect();

    enemy_timer = new QTimer(this);
    connect(enemy_timer,&QTimer::timeout,[=]{
        if(this->y()<864)
        {

            enemy_rect->moveTo(this->x(),this->y()+this->speed);
            this->move(this->x(),this->y()+this->speed);
        }
        else
        {
            this->enemy_reset();
        }
    });

}

void enemy::enemy_die()
{
    QTimer *stop_time = new QTimer(this);
    stop_time->start(100);

    enemy_rect->moveTo(this->start_x,this->start_y);
    this->enemy_timer->stop();
    connect(stop_time,&QTimer::timeout,[=]{
        QPixmap pix;
        //小飞机
        if(id==1)
        {
            if(i==6)
            {
                i=1;
                this->enemy_reset();
                stop_time->stop();
            }
            pix.load(QString(":/images/enemy_%1.png").arg(this->i++));
            this->setPixmap(pix);
        }
        //中型飞机
        else if(id ==2)
        {
            if(i==5)
            {
                i=0;
                this->enemy_reset();
                stop_time->stop();
            }
            pix.load(QString(":/images/enemyPlane3_die_%1.png").arg(this->i++));
            this->setPixmap(pix);
        }
        //大飞机
        else if(id ==3)
        {
            if(i==7)
            {
                i=0;
                this->enemy_reset();
                stop_time->stop();
            }
            pix.load(QString(":/images/enemyPlane2_die_%1.png").arg(this->i++));
            this->setPixmap(pix);
        }
        if(i==3) emit this->add_score();

    });
}

void enemy::enemy_reset()
{
    QPixmap pix;
    if(id == 1)  pix.load(":/images/enemy_1.png");
    else if(id ==2)  pix.load(":/images/enemyPlane3_die_0.png");
    else if(id ==3)  pix.load(":/images/enemyPlane2_die_0.png");

    this->setPixmap(pix);
    this->Is_exist = true;
    enemy_rect->moveTo(this->start_x,this->start_y);
    this->move(this->start_x,this->start_y);
    this->enemy_timer->stop();


}

void enemy::enemy_hurt()
{
    hp--;
    QPixmap pix;

    if(id==2)
    {
        //显示受击图片
        pix.load(":/images/enemyPlane3_hurt.png");
        this->setPixmap(pix);

        pix.load(":/images/enemyPlane3_die_0.png");

        //延时100ms后显示原图
        QTimer::singleShot(100,this,[=]{
             this->setPixmap(pix);
        });

        if(hp==0)
        {
            hp = 3;
            enemy_die();
        }
    }
    if(id ==3)
    {
        pix.load(":/images/enemyPlane2_hurt.png");
        this->setPixmap(pix);
        pix.load(":/images/enemyPlane2_die_0.png");
        QTimer::singleShot(100,this,[=]{
            this->setPixmap(pix);

        });

        if(hp==0)
        {
            hp = 8;
            enemy_die();
        }
    }
}
