#include "mypushbuttom.h"
#include <QDebug>
//MyPushButtom::MyPushButtom(QWidget *parent)
//    : QPushButton{parent}
//{

//}

MyPushButtom::MyPushButtom(QString img)
{
    this->imgPath=img;
    QPixmap pix;
    bool ret=pix.load(img);
    if(!ret)
    {
        qDebug()<<"图片加载失败";
        return;
    }

    //设置图片固定大小
    this->setFixedSize(pix.width()*0.7,pix.height()*0.7);
    //设置不规则图片样式
    this->setStyleSheet("QPushButton{border:0px;}");
    //设置图标
    this->setIcon(pix);
    //设置图标大小
    this->setIconSize(QSize(pix.width()*0.7,pix.height()*0.7));
}
