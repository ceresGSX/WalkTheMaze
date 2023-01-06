#include "mazewindow.h"
#include "mypushbuttom.h"
#include <QPainter>
#include <QFile>
#include <QDebug>
#include "point.h"
#include <QQueue>
#include <QKeyEvent>
#include <QTimer>
#include <QLabel>
#include <QFont>

MazeWindow::MazeWindow(QWidget *parent)
    : QMainWindow{parent}
{
    setFixedSize(631,514);
    setWindowTitle("走迷宫");
    setWindowIcon(QIcon(":/image/mouse1.jpg"));

    //设置StrongFocus才能监听键盘事件
    setFocusPolicy(Qt::StrongFocus);

    //倒计时设置
    QFont ft;
    ft.setPointSize(15);
    ft.setFamily("KaiTi");

    QLabel *textlabel =new QLabel("剩余时间(s):",this);
    textlabel->setFont(ft);
    textlabel->move(10,20);
    textlabel->setFixedSize(120,50);

    QLabel *timelabel =new QLabel(this);
    timelabel->setFont(ft);
    timelabel->move(140,20);
    timelabel->setFixedSize(50,50);
    timer1 = new QTimer;
    timer1->start(1000);
    connect(timer1, &QTimer::timeout,this, [=](){
        if(remain_time>0)
        {
            remain_time--;
            timelabel->setText(QString::number(remain_time));
        }
        else
        {
            gameover=true;
            success=false;
            timer1->stop();
            update();
        }
    });

    //自动寻路
    MyPushButtom *btn_find = new MyPushButtom(":/image/btn_find.png");
    btn_find->setParent(this);
    btn_find->move(this->width()*0.85-btn_find->width()*0.5,this->height()*0.6);

    //点击自动寻路找到最短路径并标出
    connect(btn_find,&MyPushButtom::clicked,this,[=](){
        this->findway();
        if(this->reachable)
        {
            this->auto_findway=true;
            update();
        }
    });

    //返回按钮
    MyPushButtom *btn_back = new MyPushButtom(":/image/btn_back.png");
    btn_back->setParent(this);
    btn_back->move(this->width()*0.85-btn_back->width()*0.5,this->height()*0.8);

    //点击返回按钮回到主界面
    connect(btn_back,&MyPushButtom::clicked,this,[=](){
        emit this->chooseBack();
    });

}

void MazeWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix,pix1,pix2,pix3;
    pix.load(":/image/mouse3.jpg");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

    pix1.load(":/image/ground.jpg");
    pix2.load(":/image/wall.jpg");
    pix3.load(":/image/destination.jpg");

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

    tot=0;
    int x=0,y=100;
    for(int i=0;i<array.size();++i)
    {
        if(tot==13)
        {
            tot=0;
            x=0;
            y+=pix1.height()*0.4;
        }
        tot++;
        if(array[i]==1)
        {
            painter.drawPixmap(x,y,pix2.width()*0.4,pix2.height()*0.4,pix2);
            x+=pix2.width()*0.4;
        }
        else
        {
            painter.drawPixmap(x,y,pix1.width()*0.4,pix1.height()*0.4,pix1);
            x+=pix1.width()*0.4;
        }
    }

    //自动寻路的指示完成
    if(this->auto_findway)
    {
        QPixmap pix4;
        pix4.load(":/image/findway.jpg");
        int x=0,y=100;
        int x_pos=6,y_pos=6;
        while(x_pos!=12&&y_pos!=12)
        {
            painter.drawPixmap(x+y_pos*pix4.width()*0.4 , y+x_pos*pix4.height()*0.4 , pix4.width()*0.4 , pix4.height()*0.4 , pix4);
            int nx=pre[x_pos][y_pos].x;
            int ny=pre[x_pos][y_pos].y;
            x_pos=nx;
            y_pos=ny;
        }
    }

    //游戏结束
    if(gameover==true)
    {
        QPixmap pix6;
        //成功
        if(success)
        {
            pix6.load(":/image/succeed.png");
            painter.drawPixmap(450,40,pix6.width(),pix6.height(),pix6);
        }
        //失败
        else
        {
            pix6.load(":/image/failed.png");
            painter.drawPixmap(450,40,pix6.width(),pix6.height(),pix6);
        }
    }
    //画出目的地
    painter.drawPixmap(11*pix3.width()*0.4,100+11*pix3.height()*0.4,pix3.width()*0.4,pix3.height()*0.4,pix3);

    //画出老鼠
    QPixmap pix5;
    pix5.load(":/image/mouse4.png");
    painter.drawPixmap(this->mouse_x_pos*32,100+this->mouse_y_pos*32,32,32,pix5);
}


//自动寻路
void MazeWindow::findway()
{
    bool vis[13][13]={false};
    int sx[4]={-1,0,1,0};
    int sy[4]={0,1,0,-1};
    QQueue<point> q;

    vis[11][11]=true;
    pre[11][11]=point(12,12);
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
                pre[next_x][next_y]=point(nx,ny);
            }
        }
    }
    this->reachable=vis[6][6];
}

//监听键盘事件，实现移动
void MazeWindow::keyPressEvent(QKeyEvent *e)
{
    if(gameover==false)
    {
        if(e->key() == Qt::Key_W && mouse_y_pos>=2 && this->mp[mouse_y_pos-1][mouse_x_pos]==0)
        {
            mouse_y_pos--;
            update();
        }
        else if(e->key() == Qt::Key_S && mouse_y_pos<=10 && this->mp[mouse_y_pos+1][mouse_x_pos]==0)
        {
            mouse_y_pos++;
            update();
        }
        else if(e->key() == Qt::Key_A && mouse_x_pos>=2 && this->mp[mouse_y_pos][mouse_x_pos-1]==0)
        {
            mouse_x_pos--;
            update();
        }
        else if(e->key() == Qt::Key_D && mouse_x_pos<=10 && this->mp[mouse_y_pos][mouse_x_pos+1]==0)
        {
            mouse_x_pos++;
            update();
        }
        if(mouse_x_pos==11&&mouse_y_pos==11)
        {
            gameover=true;
            success=true;
            timer1->stop();
        }
    }
}
