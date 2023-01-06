#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mazewindow.h"
#include "editmazewindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void paintEvent(QPaintEvent *);

    MazeWindow *mazewindow = NULL;
    EditMazeWindow *editmazewindow=NULL;

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
