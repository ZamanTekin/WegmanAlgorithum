#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <ctime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mFile(),
    mGT(mFile.getdata(),mFile.getdim())
{
    ui->setupUi(this);

    mScatter = new SubScatter(1,2,mGT.iterateabsolute(0),5,this);

    QStringList labels;
    for (size_t i = 0; i < mFile.getdim(); i++){
        labels << QString::fromStdString(mFile.getlabel(i));
    }
    ui->dim1->addItems(labels);
    ui->dim2->addItems(labels);
    ui->sizegetter->setMaximum(20);
    ui->scatterLayout->addWidget(mScatter);


    connect( ui->dim1, static_cast<void (QComboBox::*)(int index)>(&QComboBox::currentIndexChanged),
             mScatter, &SubScatter::updateDim1);
    connect( ui->dim2, static_cast<void (QComboBox::*)(int index)>(&QComboBox::currentIndexChanged),
             mScatter, &SubScatter::updateDim2);
    connect( ui->sizegetter, static_cast<void (QSpinBox::*)(int i)>(&QSpinBox::valueChanged),
             mScatter, &SubScatter::setMarkerSize);



    mTimer.setInterval(100);
    mTimer.setSingleShot(false);
    connect(&mTimer, &QTimer::timeout,
            this,&MainWindow::updateScatters);

    mTimer.start();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateScatters()
{
    mScatter->updateData(mGT.iterate());
}

/*void MainWindow::showScatters(){
    this->setCentralWidget(mScatter);
    this->update();
}

void MainWindow::getDim1(){
    this->setCentralWidget(dim1);
    this->update();
}
void MainWindow::getDim2(){
    this->setCentralWidget(dim2);
    this->update();
}
void MainWindow::getSize(){
    this->setCentralWidget(sizegetter);
    this->update();
}*/
