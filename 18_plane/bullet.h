#ifndef BULLET_H
#define BULLET_H

#include <QLabel>
#include <QRect>



class Bullet : public QLabel
{
    Q_OBJECT
public:
//    explicit Bullet(QWidget *parent = nullptr);
    Bullet();
    void out();

    bool is_free=true;

    QTimer *timer;
    QRect *Bullet_rect;

signals:
//    void filewall();
};

class DoubleBullet : public QLabel
{
    Q_OBJECT
public:
    DoubleBullet();
    void out();

    bool is_free=true;

    QTimer *timer;
    QRect *DoubleBullet_rect;

signals:
//    void filewall();
};

#endif // BULLET_H
