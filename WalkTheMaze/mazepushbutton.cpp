#include "mazepushbutton.h"
#include <QDebug>
//MazePushButton::MazePushButton(QWidget *parent)
//    : QWidget{parent}
//{

//}

MazePushButton::MazePushButton(QString img1,QString img2)
{
    this->imgPath1=img1;
    this->imgPath2=img2;

    QPixmap pix;
    bool ret=pix.load(img1);
    if(!ret)
    {
        qDebug()<<"图片加载失败";
        return;
    }

    //设置图片固定大小
    this->setFixedSize(pix.width()*0.4,pix.height()*0.4);
    //设置不规则图片样式
    this->setStyleSheet("QPushButton{border:0px;}");
    //设置图标
    this->setIcon(pix);
    //设置图标大小
    this->setIconSize(QSize(pix.width()*0.4,pix.height()*0.4));
}

void MazePushButton::change()
{
    QString tmp=this->imgPath1;
    imgPath1=imgPath2;
    imgPath2=tmp;
    QPixmap pix;
    pix.load(imgPath1);
    this->setIcon(pix);
    this->now=this->now^1;
}
