#include "supply.h"
#include <QPixmap>
#include <QDebug>
#include <QMatrix>
#include <QPainter>
supply_file::supply_file()
{
    QPixmap pix;
    pix.load(":/images/prop2.png");

    this->setPixmap(pix);
    this->setFixedSize(pix.width(),pix.height());

    this->move(this->x(),0-this->height());

    supFile_rect = new QRect(0,0,pix.width(),pix.width());

    timer = new QTimer(this);

    connect(timer,&QTimer::timeout,this,[=]()
    {
        if(this->y()<864)
        {
            this->move(this->x(),this->y()+3);
            supFile_rect->moveTo(this->x(),this->y()+3);
        }
        else
        {

            this->reset();
        }
    });

}

void supply_file::reset()
{
    is_exist = false;
    this->move(this->x(),0-this->height());
    supFile_rect->moveTo(this->x(),0-this->height());
    timer->stop();
}


supply_boom::supply_boom()
{
    QPixmap pix;
    bool ret = pix.load(":/images/prop1.png");
    if(!ret) qDebug()<<"pix load error";

    this->setPixmap(pix);
    this->setFixedSize(pix.width(),pix.height());
    this->move(this->x(),0-this->height());

    supBoom_rect = new QRect(0,0,pix.width(),pix.width());

    timer = new QTimer(this);

    connect(timer,&QTimer::timeout,this,[=]()
    {
        if(this->y()<864)
        {
            this->move(this->x(),this->y()+3);
            supBoom_rect->moveTo(this->x(),this->y()+3);

        }
        else
        {

            this->reset();
        }
    });

}

void supply_boom::reset()
{
    is_exist = false;
    this->move(this->x(),0-this->height());
    supBoom_rect->moveTo(this->x(),0-this->height());
    timer->stop();
}



