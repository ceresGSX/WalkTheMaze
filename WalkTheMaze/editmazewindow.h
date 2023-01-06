#ifndef EDITMAZEWINDOW_H
#define EDITMAZEWINDOW_H

#include <QWidget>

class EditMazeWindow : public QWidget
{
    Q_OBJECT
public:
    explicit EditMazeWindow(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *);//绘图事件
    bool is_read=false;
    bool is_save=false;
    int mp[13][13];

    void read();
    bool findway();
signals:
    void chooseBack();

};

#endif // EDITMAZEWINDOW_H
