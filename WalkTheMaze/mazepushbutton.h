#ifndef MAZEPUSHBUTTON_H
#define MAZEPUSHBUTTON_H

#include <QPushButton>

class MazePushButton : public QPushButton
{
    Q_OBJECT
public:
//    explicit MazePushButton(QWidget *parent = nullptr);
    MazePushButton(QString img1,QString img2);
    QString imgPath1;
    QString imgPath2;
    int now;
    int x;
    int y;
    void change();
signals:

};

#endif // MAZEPUSHBUTTON_H
