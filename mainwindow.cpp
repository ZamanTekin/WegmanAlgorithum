#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <ctime>

MainWindow::MainWindow(QWidget *parent) :
    //intialise member data & objects
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mFile(),
    mGT(mFile.getdata(),mFile.getdim()),
    interval(10),
    paused(true)
{
    ui->setupUi(this);

    // create intial scatter plot
    mScatter = new SubScatter(1,2,mGT.iterateabsolute(0),5,this);

    // get dimension labels
    QStringList labels;
    for (size_t i = 0; i < mFile.getdim(); i++){
        labels << QString::fromStdString(mFile.getlabel(i));
    }

    //intialise widgets
    ui->dim1->addItems(labels);
    ui->dim1->setCurrentIndex(0);
    ui->dim1label->setText("First axis");

    ui->dim2->addItems(labels);
    ui->dim2->setCurrentIndex(1);
    ui->dim2label->setText("Second axis");

    ui->sizegetter->setMinimum(1);
    ui->sizegetter->setMaximum(20);
    ui->sizegetter->setValue(5);
    static QString sizegetterlabeltext = QStringLiteral("Point size: %1");
    ui->sizegetterlabel->setText(sizegetterlabeltext.arg(QString::number(ui->sizegetter->value())));

    ui->fpscap->setMinimum(1);
    ui->fpscap->setMaximum(300);
    ui->fpscap->setValue(60);
    this->setFPSCap(60);
    static QString fpscaplabeltext = QStringLiteral("FPS cap: %1");
    ui->fpscaplabel->setText(fpscaplabeltext.arg(QString::number(ui->fpscap->value())));

    ui->scatterLayout->addWidget(mScatter);

    //connect widgets
    connect( ui->dim1, static_cast<void (QComboBox::*)(int index)>(&QComboBox::currentIndexChanged),
             mScatter, &SubScatter::updateDim1);
    connect( ui->dim2, static_cast<void (QComboBox::*)(int index)>(&QComboBox::currentIndexChanged),
             mScatter, &SubScatter::updateDim2);
    connect( ui->sizegetter, &QSlider::valueChanged,
             mScatter, &SubScatter::setMarkerSize);
    connect( ui->fpscap, &QSlider::valueChanged,
             this, &MainWindow::setFPSCap);
    connect( ui->sizegetter, &QSlider::valueChanged,
             [=](const int &value){
        ui->sizegetterlabel->setText(sizegetterlabeltext.arg(QString::number(value)));
    });
    connect( ui->fpscap, &QSlider::valueChanged,
             [=](const int &value){
        ui->fpscaplabel->setText(fpscaplabeltext.arg(QString::number(value)));
    });

    // pause/play button simply stops/starts timer
    connect(ui->pauseButton, &QPushButton::clicked,
            [this]{
        if(paused==false){
            ui->pauseButton->setText("Play");
            mTimer.stop();
            paused = true;
        }
        else{
            ui->pauseButton->setText("Pause");
            mTimer.start();
            paused = false;
        }
        });


    //timer controls scatter update rate
    mTimer.setInterval(interval);
    mTimer.setSingleShot(false);
    connect(&mTimer, &QTimer::timeout,
            this,&MainWindow::updateScatters);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateScatters()
{
    mScatter->updateData(mGT.iterate());
}

void MainWindow::setFPSCap(const int fps)
{
    mTimer.setInterval(1000/fps);
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
