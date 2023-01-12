#ifndef SUPPLY_H
#define SUPPLY_H

#include <QLabel>
#include <QRect>
#include <QTimer>
class supply_file:public QLabel
{
public:
    supply_file();

    QTimer *timer;
    void reset();
    QRect *supFile_rect;

    bool is_exist = false;

//    void paintEvent(QPaintEvent *);
};

class supply_boom:public QLabel
{
public:
    supply_boom();
    QTimer *timer;
    void reset();
    QRect *supBoom_rect;

    bool is_exist = false;
};

#endif // SUPPLY_H
