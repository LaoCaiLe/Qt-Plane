#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QLabel>
#include <QDebug>
#include <QKeyEvent>
#include <QtGlobal>
#include <QTimer>
#include <QTime>
#include <QRect>
#include <QMouseEvent>
#include <QEvent>
#include <QPainter>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //设置界面大小
    setFixedSize(591,864);
    //设置图标
    setWindowIcon(QIcon(":/images/planeNormal_2.png"));
    //设置窗口标题
    setWindowTitle("飞机大战");

    //    //创建该界面的菜单栏
    //    QMenuBar *bar = menuBar();
    //    setMenuBar(bar);

    //    //创建开始菜单
    //    QMenu *startMenu = bar->addMenu("菜单");

    //    //创建退出菜单项
    //    QAction *quitaction = startMenu->addAction("退出");

    //    connect(quitaction,&QAction::triggered,[=]{
    //        this->close();
    //    });

    //画飞机图
    QPixmap pix;
    pix.load(":/images/planeNormal_2.png");

    //飞机label
    plane = new QLabel(this);
    plane->setFixedSize(pix.width(),pix.height());
    plane->setPixmap(pix);
    plane->move(this->width()*0.5-pix.width()*0.5, this->height()-pix.height());

    plane_rect = new QRect(plane->x(),plane->y(),plane->width(),plane->height());

    //分数显示
    QLabel *score_board = new QLabel(this);
    QFont *font = new QFont("Arial",15);
    score_board->setFont(*font);
    score_board->setFixedSize(150,30);
    score_board->setText(QString("score: %1").arg(this->score));

    //弹药补给
    supfile = new supply_file();
    supfile->setParent(this);

    subboom = new supply_boom();
    subboom->setParent(this);

    //随机种子
    qsrand(time(NULL));

    pix.load(":/images/bullet2.png");
    for(int i=0;i<bullet_num;i++)
    {
        bullet_label[i] = new Bullet();
        bullet_label[i]->setParent(this);
        bullet_label[i]->Bullet_rect->setRect(0,0,pix.width(),pix.height());
    }

    pix.load(":/images/cartridge_power.png");
    for(int i=0;i<doublefile_num;i++)
    {
        doublebullet_label[i] = new DoubleBullet();
        doublebullet_label[i]->setParent(this);
        doublebullet_label[i]->DoubleBullet_rect->setRect(0,0,pix.width(),pix.height());
    }

    connect(this,&MainWindow::filewall,[=]{
        int i;
        QPixmap pix;
        if(!DoubleFile_mode)
        {
            pix.load(":/images/bullet2.png");
            for(i=0;i<bullet_num;i++)
            {
                if(bullet_label[i]->is_free == true)
                {
                    bullet_label[i]->is_free = false;
                    bullet_label[i]->setPixmap(pix);
                    break;
                }
            }
            if(i<bullet_num)
            {
                bullet_label[i]->Bullet_rect->moveTo(plane->x()+plane->width()*0.5-pix.width()*0.5, plane->y()-pix.height());
                bullet_label[i]->move(plane->x()+plane->width()*0.5-pix.width()*0.5, plane->y()-pix.height());
                bullet_label[i]->timer->start(30);

            }
        }
        else
        {
            pix.load(":/images/cartridge_power.png");
            for(i=0;i<doublefile_num-1;i++)
            {
                if(doublebullet_label[i]->is_free == true && doublebullet_label[i+1]->is_free == true)
                {
                    doublebullet_label[i]->is_free = false;
                    doublebullet_label[i+1]->is_free  = false;
                    doublebullet_label[i]->setPixmap(pix);
                    doublebullet_label[i+1]->setPixmap(pix);
                    break;
                }
            }
            if(i<doublefile_num-1)
            {
                doublebullet_label[i]->DoubleBullet_rect->moveTo(plane->x()+plane->width()*0.33-pix.width()*0.5, plane->y()-pix.height());
                doublebullet_label[i]->move(plane->x()+plane->width()*0.3-pix.width()*0.5, plane->y()-pix.height());
                doublebullet_label[i]->timer->start(30);

                doublebullet_label[i+1]->DoubleBullet_rect->moveTo(plane->x()+plane->width()*0.66-pix.width()*0.5, plane->y()-pix.height());
                doublebullet_label[i+1]->move(plane->x()+plane->width()*0.7-pix.width()*0.5, plane->y()-pix.height());
                doublebullet_label[i+1]->timer->start(30);
            }
        }
    });

    //产生随机数
    int start_x[all_enemy_num];
    int speed[low_enemy_num+mid_enemy_num];
    for(int i=0; i<all_enemy_num; i++)
    {
        start_x[i]= qrand()%(this->width()-pix.width()*2);
        speed[i] = 3+qrand()%(8-3);
    }

    pix.load(":/images/enemy_1.png");
    for(int i=0; i<low_enemy_num; i++)
    {
        enemy_plane[i] = new enemy(":/images/enemy_1.png");
        enemy_plane[i]->setParent(this);
        enemy_plane[i]->id = 1;
        enemy_plane[i]->start_x = start_x[i];
        enemy_plane[i]->start_y = -enemy_plane[i]->height();
        enemy_plane[i]->speed = speed[i];
        enemy_plane[i]->move(enemy_plane[i]->start_x,enemy_plane[i]->start_y);
        enemy_plane[i]->enemy_rect->setRect(0,0,pix.width(),pix.height());
        enemy_plane[i]->enemy_rect->moveTo(enemy_plane[i]->start_x,enemy_plane[i]->start_y);
        enemy_plane[i]->setPixmap(pix);

        connect(enemy_plane[i],&enemy::add_score,[=]{
            this->score+=10;
            score_board->setText(QString("score: %1").arg(this->score));

        });
    }

    pix.load(":/images/enemyPlane3_die_0.png");
    for(int i=low_enemy_num; i<low_enemy_num+mid_enemy_num; i++)
    {
        enemy_plane[i] = new enemy(":/images/enemyPlane3_die_0.png");
        enemy_plane[i]->setParent(this);
        enemy_plane[i]->id = 2;
        enemy_plane[i]->hp = 3;
        if(start_x[i]>this->width()-pix.width())
            start_x[i]-=pix.width();
        enemy_plane[i]->start_x = start_x[i];
        enemy_plane[i]->start_y = -enemy_plane[i]->height();
        enemy_plane[i]->speed = speed[i];
        enemy_plane[i]->move(enemy_plane[i]->start_x,enemy_plane[i]->start_y);
        enemy_plane[i]->enemy_rect->setRect(0,0,pix.width(),pix.height());
        enemy_plane[i]->enemy_rect->moveTo(enemy_plane[i]->start_x,enemy_plane[i]->start_y);
        enemy_plane[i]->setPixmap(pix);
        connect(enemy_plane[i],&enemy::add_score,[=]{
            this->score+=30;
            score_board->setText(QString("score: %1").arg(this->score));
        });
    }


    pix.load(":/images/enemyPlane2_die_0.png");
    for(int i=low_enemy_num+mid_enemy_num; i<all_enemy_num; i++)
    {
        enemy_plane[i] = new enemy(":/images/enemyPlane2_die_0.png");
        enemy_plane[i]->setParent(this);
        enemy_plane[i]->id = 3;
        enemy_plane[i]->hp = 8;
        if(start_x[i]>this->width()-pix.width())
            start_x[i]-=pix.width();
        enemy_plane[i]->start_x = start_x[i];
        enemy_plane[i]->start_y = -enemy_plane[i]->height();
        enemy_plane[i]->speed = 3;
        enemy_plane[i]->move(enemy_plane[i]->start_x,enemy_plane[i]->start_y);
        enemy_plane[i]->enemy_rect->setRect(0,0,pix.width(),pix.height());
        enemy_plane[i]->enemy_rect->moveTo(enemy_plane[i]->start_x,enemy_plane[i]->start_y);
        enemy_plane[i]->setPixmap(pix);
        connect(enemy_plane[i],&enemy::add_score,[=]{
            this->score+=60;
            score_board->setText(QString("score: %1").arg(this->score));
        });
    }

    QTimer *time = new QTimer(this);
    QTimer *time1 = new QTimer(this);
    time->start(500);
    time1->start(5);

    connect(time, &QTimer::timeout, [=]{
        int i = qrand()%(all_enemy_num);
        if(enemy_plane[i]->Is_exist)
        {
            enemy_plane[i]->Is_exist = false;
            enemy_plane[i]->enemy_timer->start(30);
        }
    });

    connect(time1, &QTimer::timeout, [=]{
        for(int i=0; i<all_enemy_num; i++)
        {
            if(enemy_plane[i]->Is_exist == false)
            {
                //检测我机与敌机是否碰撞
                if(enemy_plane[i]->enemy_rect->intersects(*plane_rect))
                {
                    //发送游戏结束信号
                    //emit this->game_over();
                }
                if(!DoubleFile_mode)
                {
                    for(int j=0; j<bullet_num; j++)
                    {
                        if(bullet_label[j]->is_free == false)
                        {
                            //检测子弹是否与敌机碰撞
                            if(enemy_plane[i]->enemy_rect->intersects(*bullet_label[j]->Bullet_rect))
                            {
                                if(i<30)
                                    enemy_plane[i]->enemy_die();
                                else
                                    enemy_plane[i]->enemy_hurt();
                                bullet_label[j]->out();
                            }
                        }
                    }
                }
                else
                {
                    for(int j=0; j<doublefile_num; j++)
                    {
                        if(doublebullet_label[j]->is_free == false)
                        {
                            //检测子弹是否与敌机碰撞
                            if(enemy_plane[i]->enemy_rect->intersects(*doublebullet_label[j]->DoubleBullet_rect))
                            {
                                if(i<30)
                                    enemy_plane[i]->enemy_die();
                                else
                                    enemy_plane[i]->enemy_hurt();
                                doublebullet_label[j]->out();
                            }
                        }
                    }
                }
            }
        }

        if(supfile->is_exist && plane_rect->intersects(*supfile->supFile_rect))
        {
            supfile->reset();
            DoubleFile_mode = true;
            QTimer::singleShot(18000,this,[=](){
                DoubleFile_mode = false;
            });
        }
        if(subboom->is_exist && plane_rect->intersects(*subboom->supBoom_rect))
        {
            subboom->reset();
            for(int i=0; i<all_enemy_num; i++)
            {
                if(enemy_plane[i]->Is_exist == false)

                {
                    enemy_plane[i]->enemy_die();
                }
            }
        }
    });

    connect(this,&MainWindow::game_over,this,[=]()
    {
        int i;
        time->stop();
        qDebug()<<"plane die";
        for(i=0; i<all_enemy_num; i++)
        {
            if(enemy_plane[i]->Is_exist == false)
            {
                enemy_plane[i]->enemy_timer->stop();
            }
        }
        for(i=0; i<bullet_num; i++)
        {
            if(bullet_label[i]->is_free == false)
            {
                bullet_label[i]->timer->stop();
            }
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;

    //画背景图
    bool ret = pix.load(":/images/background.jpg");
    if(!ret) qDebug()<<"background 图片加载失败";
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    switch(e->key())
    {
    //上下左右移动
    case Qt::Key_Up:
        if(plane->y()>Speed)
            plane->move(plane->x(),plane->y()-Speed);
        break;

    case Qt::Key_Down:
        if(plane->y()<this->height() -plane->height())
            plane->move(plane->x(),plane->y()+Speed);
        break;

    case Qt::Key_Left:
        if(plane->x()>0)
            plane->move(plane->x()-Speed,plane->y());
        break;

    case Qt::Key_Right:
        if(plane->x()<this->width() -plane->width())
            plane->move(plane->x()+Speed,plane->y());
        break;

        //发射子弹
    case Qt::Key_Space:

        emit this->filewall();
        break;

    case Qt::Key_Enter:
        qDebug()<<"enter";
        for(int i=0; i<all_enemy_num; i++)
        {
            if(enemy_plane[i]->Is_exist == false)
            {
                enemy_plane[i]->enemy_die();
            }
        }
        break;
    case Qt::Key_A:
        qDebug()<<"Key_A";
        if(!supfile->is_exist)
        {
            supfile->move(qrand()%(this->width()-supfile->width()),supfile->y());
            supfile->timer->start(30);
            supfile->is_exist = true;
        }
        break;
    case Qt::Key_B:
        qDebug()<<"Key_B";
        if(!subboom->is_exist)
        {
            subboom->move(qrand()%(this->width()-subboom->width()),subboom->y());
            subboom->timer->start(30);
            subboom->is_exist = true;
        }
        break;

    default:break;
    }
    plane_rect->moveTo(plane->x(),plane->y());
}

