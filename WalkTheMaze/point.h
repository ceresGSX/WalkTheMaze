#ifndef POINT_H
#define POINT_H

//#include <QObject>

class point /*: public QObject*/
{
//    Q_OBJECT
public:
//    explicit Point(QObject *parent = nullptr);
    point();
    point(int x,int y);
    int x;
    int y;
//signals:

};

#endif // POINT_H
