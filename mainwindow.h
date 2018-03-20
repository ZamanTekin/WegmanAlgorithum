#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QFileDialog>
#include <QComboBox>
#include <QSpinBox>
#include <QAbstractSpinBox>
#include "subscatter.h"
#include "GTalgorithm.h"
#include "filemanager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void updateScatters();
    void setFPSCap(const int fps);

    /*//testing
    void showScatters();
    void getDim1();
    void getDim2();
    void getSize();*/

private:
    Ui::MainWindow *ui;
    SubScatter *mScatter;
    filemanager::file mFile;
    GT::GTalgorithm mGT;
    QTimer mTimer;
    int interval;
    bool paused;
};

#endif // MAINWINDOW_H
