#include "mainwindow.h"
#include "subscatter.h"
#include "filemanager.h"
#include "GTalgorithm.h"

#include <QApplication>
#include <QTimer>
#include <QDebug>
#include <ctime>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();

    filemanager::file f("M:\\MPhys2\\WegmanTour\\WegmanTour\\CrystalDemoData\\pollen.txt");
    GT::GTalgorithm g(f.getdata(),f.getdim());

    SubScatter *subscatter = new SubScatter(1,2,g.iterate());
    QMainWindow window;
    window.setCentralWidget(subscatter);
    window.resize(400, 300);
    window.show();

    QTimer timer;
    timer.setSingleShot(false);
    timer.start(100);
    QObject::connect(&timer, &QTimer::timeout,
            subscatter, [&g,subscatter]{
        clock_t time2 = clock();
        Eigen::MatrixXd it = g.iterate();
        clock_t time0 = clock();
        subscatter->updateData(it);
        clock_t time1 = clock();
        qDebug() << (float)(time0-time2)/CLOCKS_PER_SEC << ", "<<(float)(time1-time0)/CLOCKS_PER_SEC;
    });

    return a.exec();
}
