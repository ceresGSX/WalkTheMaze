#ifndef MAZEDATA_H
#define MAZEDATA_H

#include <QObject>

class MazeData : public QObject
{
    Q_OBJECT
public:
    explicit MazeData(QObject *parent = nullptr);
    int mp[13][13];
signals:

};

#endif // MAZEDATA_H
