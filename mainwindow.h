#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mazegraphicsview.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void enableBuildButton(bool state);
private slots:
    void on_buildButton_released();

    void on_findWayButton_released();

private:
    Ui::MainWindow *ui;
    MazeGraphicsView* m_graphView;
};

#endif // MAINWINDOW_H
