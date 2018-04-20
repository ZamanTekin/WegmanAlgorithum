#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QFileDialog>
#include <QComboBox>
#include <QSpinBox>
#include <QAbstractSpinBox>
#include <QElapsedTimer>
#include "subscatter.h"
#include "GTalgorithm.h"
#include "filemanager.h"
#include "masterplot.h"

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
    void updateScatters(const int i=1);
    void setScatters(const int i=0);
    void setFPSCap(const int fps);
    void changeFile(const QString filename);
    void showBasis();

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
    QElapsedTimer m_fpsTimer;
    int framecount;

};

#endif // MAINWINDOW_H
