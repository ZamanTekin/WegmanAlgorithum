#include "mainwindow.h"

#include <QApplication>
#include "masterplot.h"
#include "GTalgorithm.h"
#include "filemanager.h"

int main(int argc, char *argv[])
{
   QApplication a(argc, argv);

    MainWindow w;
    w.show();


   // MasterPlot testing
//    QMainWindow w;
//    filemanager::file f;
//    GT::GTalgorithm g(f.getdata(),f.getdim());

//    MasterPlot *m = new MasterPlot(g.iterate(0),5,&w);

//    w.setCentralWidget(m);

//    w.show();


    return a.exec();
}
