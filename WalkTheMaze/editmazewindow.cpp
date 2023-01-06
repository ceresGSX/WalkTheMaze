#include "editmazewindow.h"
#include <QPainter>
#include <QFile>
#include <QDebug>
#include "mypushbuttom.h"
#include "mazepushbutton.h"
#include <QQueue>
#include "point.h"
#include <QFont>
#include <QLabel>
#include <QFile>
#include <QDebug>

EditMazeWindow::EditMazeWindow(QWidget *parent)
    : QWidget{parent}
{
    setFixedSize(631,514);
    setWindowTitle("走迷宫");
    setWindowIcon(QIcon(":/image/mouse1.jpg"));

    if(is_read==false)
        read();

    //按钮的布置
    for(int i=1;i<=11;++i)
    {
        for(int j=1;j<=11;++j)
        {
            if(!((i==6&&j==6)||(i==11&&j==11)))
            {
                if(mp[i][j]==0)
                {
                    MazePushButton *mpb=new MazePushButton(":/image/ground.jpg",":/image/wall.jpg");
                    mpb->setParent(this);
                    mpb->move(j*32,100+i*32);
                    mpb->now=0;
                    mpb->x=i;
                    mpb->y=j;
                    connect(mpb,&MazePushButton::clicked,this,[=](){
                        if(is_save==false)
                        {
                            mpb->change();
                            this->mp[mpb->x][mpb->y]=this->mp[mpb->x][mpb->y]^1;
                        }
                    });
                }
                else
                {
                    MazePushButton *mpb=new MazePushButton(":/image/wall.jpg",":/image/ground.jpg");
                    mpb->setParent(this);
                    mpb->move(j*32,100+i*32);
                    mpb->now=1;
                    mpb->x=i;
                    mpb->y=j;
                    connect(mpb,&MazePushButton::clicked,this,[=](){
                        if(is_save==false)
                        {
                            mpb->change();
                            this->mp[mpb->x][mpb->y]=this->mp[mpb->x][mpb->y]^1;
                        }
                    });
                }
            }
        }
    }

    //返回按钮
    MyPushButtom *btn_back = new MyPushButtom(":/image/btn_back.png");
    btn_back->setParent(this);
    btn_back->move(this->width()*0.85-btn_back->width()*0.5,this->height()*0.8);

    //点击返回按钮回到主界面
    connect(btn_back,&MyPushButtom::clicked,this,[=](){
        emit this->chooseBack();
    });

    MyPushButtom *btn_save = new MyPushButtom(":/image/saveBtn_1.png");
    btn_save->setParent(this);
    btn_save->move(this->width()*0.85-btn_back->width()*0.5,this->height()*0.6);


    QFont ft;
    ft.setPointSize(15);
    ft.setFamily("KaiTi");

    QLabel *textlabel =new QLabel(this);
    textlabel->setFont(ft);
    textlabel->move(150,20);
    textlabel->setFixedSize(200,50);
    //点击保存按钮
    connect(btn_save,&MyPushButtom::clicked,this,[=](){
        if(is_save==false)
        {
            if(this->findway())
            {
                textlabel->setText("保存成功，请退出");
                is_save=true;
                QFile file("map.txt");
                if(file.open(QIODevice::WriteOnly))
                {
                    QTextStream out(&file);
                    for(int i=0;i<13;++i)
                        for(int j=0;j<13;++j)
                            out << mp[i][j]<<" ";
                    file.close();
                }
                else
                {
                    qDebug()<<"写文件时打开文件失败";
                    return;
                }
            }
            else
            {
                textlabel->setText("路不通，重新设置");
            }
        }
    });
}

void EditMazeWindow::paintEvent(QPaintEvent *)//绘图事件
{
    if(is_read==false)
    {
        this->read();
    }

    QPainter painter(this);
    QPixmap pix,pix1,pix2;
    pix.load(":/image/mouse3.jpg");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
    pix1.load(":/image/ground.jpg");
    pix2.load(":/image/wall.jpg");

    //绘制边界和起始结束点
    for(int i=1;i<=12;++i)
        painter.drawPixmap( 0 , 100+(i-1)*pix2.height()*0.4 , pix2.width()*0.4 , pix2.height()*0.4 , pix2 );
    for(int i=1;i<=12;++i)
        painter.drawPixmap( (i-1)*pix2.width()*0.4 , 100+12*pix2.height()*0.4 , pix2.width()*0.4 , pix2.height()*0.4 , pix2 );
    for(int i=1;i<=12;++i)
        painter.drawPixmap( 12*pix2.width()*0.4 , 100+i*pix2.height()*0.4 , pix2.width()*0.4 , pix2.height()*0.4 , pix2 );
    for(int i=1;i<=12;++i)
        painter.drawPixmap( i*pix2.width()*0.4 , 100 , pix2.width()*0.4 , pix2.height()*0.4 , pix2 );
    painter.drawPixmap( 6*pix1.width()*0.4 , 100+6*pix1.height()*0.4 , pix1.width()*0.4 , pix1.height()*0.4 , pix1);
    painter.drawPixmap( 11*pix1.width()*0.4 , 100+11*pix1.height()*0.4 , pix1.width()*0.4 , pix1.height()*0.4 , pix1);


}

//读取地图
void EditMazeWindow::read()
{
    QFile file("map.txt");
    QVector<int> array;
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        while(!file.atEnd())
        {
            int buf;
            QStringList list=stream.readAll().split(" ");
            QListIterator<QString> li(list);
            while(li.hasNext())
            {
                buf=li.next().toInt();
                array.append(buf);
            }
        }
        file.close();
    }
    else
    {
        qDebug()<<"未成功";
    }
    int tot=0;
    for(int i=0;i<=12;++i)
    {
        for(int j=0;j<=12;++j)
            this->mp[i][j]=array[tot++];
    }
    is_read=true;
}

bool EditMazeWindow::findway()
{
    bool vis[13][13]={false};
    int sx[4]={-1,0,1,0};
    int sy[4]={0,1,0,-1};
    QQueue<point> q;

    vis[11][11]=true;
    q.enqueue(point(11,11));
    while(!q.empty())
    {
        int nx=q.head().x;
        int ny=q.head().y;
        q.dequeue();
        for(int i=0;i<4;++i)
        {
            int next_x=nx+sx[i];
            int next_y=ny+sy[i];
            if(next_x>=1 && next_x<=11 && next_y>=1 && next_y<=11 && vis[next_x][next_y]==false && mp[next_x][next_y]==0)
            {
                q.enqueue(point(next_x,next_y));
                vis[next_x][next_y]=true;
            }
        }
    }
    return vis[6][6];
}
