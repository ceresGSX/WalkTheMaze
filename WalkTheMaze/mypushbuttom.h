#ifndef MYPUSHBUTTOM_H
#define MYPUSHBUTTOM_H

#include <QPushButton>

class MyPushButtom : public QPushButton
{
    Q_OBJECT
public:
    //explicit MyPushButtom(QWidget *parent = nullptr);
    MyPushButtom(QString img);
    QString imgPath;

signals:

};

#endif // MYPUSHBUTTOM_H
