#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QColorDialog>
#include <QMessageBox>
#include <ctime>

MainWindow::MainWindow(QWidget *parent) :
    //intialise member data & objects
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mFile(),
    mGT(mFile.getdata(),mFile.getdim()),
    interval(10),
    paused(true),
    framecount(0)
{
    ui->setupUi(this);

    // create intial scatter plot
    mScatter = new SubScatter(1,2,mGT.iterateabsolute(0),5,this);

//    // get dimension labels
    QStringList labels;
    for (size_t i = 0; i < mFile.getdim(); i++){
        labels << QString::fromStdString(mFile.getlabel(i));
    }

    //intialise widgets
    ui->filenamebox->setText(QString::fromStdString(mFile.getfilename()));

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

    static QString fpslabeltext = QStringLiteral("Actual FPS: %1");
    ui->fpslabel->setText(fpslabeltext.arg(QString::number(60, 'f', 1)));

    static QString iterationlabeltext = QStringLiteral("Iteration: %1");
    ui->iterationlabel->setText(iterationlabeltext.arg(QString::number(0, 'i', 0)));

    ui->iterationbox->setValidator( new QIntValidator(this) );
    ui->iterationbox->setText(QString::number(0));

    ui->groupBox->addItem("Zoom");
    ui->groupBox->addItem("Group 1");
    ui->groupBox->setCurrentIndex(0);

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
            m_fpsTimer.restart();
            paused = false;
        }
        });

    connect(ui->filedialogButton, &QPushButton::clicked,
            [=] {
        ui->filenamebox->setText(QFileDialog::getOpenFileName(NULL,"open file","/home","(*.txt *.csv)"));
    });

    connect(ui->openButton, &QPushButton::clicked,
            [=]{
        this->changeFile(ui->filenamebox->text());
    });

    connect(ui->showbasisButton, &QPushButton::clicked,
            this, &MainWindow::showBasis);

    connect(ui->zoomButton, &QPushButton::clicked,
            [=]{
        ui->groupBox->setCurrentIndex(0);
        mScatter->setGroup(0);
    });


    connect( ui->groupBox, static_cast<void (QComboBox::*)(int index)>(&QComboBox::currentIndexChanged),
             mScatter, &SubScatter::setGroup);

    connect(ui->newgroupButton, &QPushButton::clicked,
            [=]{
            mScatter->createGroup(QColorDialog::getColor(Qt::blue,this));
            QString str = "Group ";
            str += QString::number(ui->groupBox->count());
            ui->groupBox->addItem(str);
            ui->groupBox->setCurrentIndex(ui->groupBox->count()-1);
            mScatter->setGroup(ui->groupBox->currentIndex());
    });

    connect(ui->biggerplusButton, &QPushButton::clicked,
            [this]{
        this->updateScatters(100);
    });
    connect(ui->bigplusButton, &QPushButton::clicked,
            [this]{
        this->updateScatters(10);
    });
    connect(ui->plusButton, &QPushButton::clicked,
            [this]{
        this->updateScatters(1);
    });
    connect(ui->minusButton, &QPushButton::clicked,
            [this]{
        this->updateScatters(-1);
    });
    connect(ui->bigminusButton, &QPushButton::clicked,
            [this]{
        this->updateScatters(-10);
    });
    connect(ui->biggerminusButton, &QPushButton::clicked,
            [this]{
        this->updateScatters(-100);
    });

    connect(ui->iterationButton, &QPushButton::clicked,
            [this]{
        this->setScatters((ui->iterationbox->text()).toInt());
    });



    //timer controls scatter update rate
    mTimer.setInterval(interval);
    mTimer.setSingleShot(false);
    connect(&mTimer, &QTimer::timeout,
            [this]{
        this->updateScatters();
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateScatters(const int i)
{
    mScatter->updateData(mGT.iterate(i));

    static QString fpslabeltext = QStringLiteral("Actual FPS: %1");

    static QString iterationlabeltext = QStringLiteral("Iteration: %1");
    ui->iterationlabel->setText(iterationlabeltext.arg(QString::number(mGT.getIteration(), 'i', 0)));

    framecount++;
    if (m_fpsTimer.elapsed() >= 1000) {
        qreal fps = qreal(1000.0 * (qreal(framecount) / qreal(m_fpsTimer.elapsed())));
        ui->fpslabel->setText(fpslabeltext.arg(QString::number(fps, 'f', 1)));
        m_fpsTimer.restart();
        framecount = 0;
    }
}

void MainWindow::setScatters(const int i){
    mScatter->updateData(mGT.set(i));

    static QString iterationlabeltext = QStringLiteral("Iteration: %1");
    ui->iterationlabel->setText(iterationlabeltext.arg(QString::number(mGT.getIteration(), 'i', 0)));

    framecount = 0;
    m_fpsTimer.restart();
}

void MainWindow::setFPSCap(const int fps)
{
    mTimer.setInterval(1000/fps);
}

void MainWindow::changeFile(const QString filename){
    mFile.newfile(filename.toStdString());
    mGT.changeData(mFile.getdata(),mFile.getdim());

    //reinitialise axes widgets
    QStringList labels;
    for (size_t i = 0; i < mFile.getdim(); i++){
        labels << QString::fromStdString(mFile.getlabel(i));
    }

    for (int i = ui->dim1->count(); i >= 0; i--){
        ui->dim1->removeItem(i);
    }
    for (int i = (ui->dim2->count())-1; i >= 0; i--){
        ui->dim2->removeItem(i);
    }
    ui->dim1->addItems(labels);
    ui->dim1->setCurrentIndex(0);
    ui->dim2->addItems(labels);
    ui->dim2->setCurrentIndex(1);

    // plot new data
    mScatter->updateData(mGT.iterate(0));

    framecount = 0;
    m_fpsTimer.restart();
}

void MainWindow::showBasis(){

    if(paused==false){
        ui->pauseButton->setText("Play");
        mTimer.stop();
        paused = true;
    }

    //CHECK MATHS ON THIS
    Eigen::MatrixXd basis = mGT.getBasis();
    QString axes = "X: ";
    for(int i = 0; i < ui->dim1->count(); i++){
        if(i!=0){
            axes.append(" + ");
        }
        axes.append(QString::number(basis(i,ui->dim1->currentIndex())));
        axes.append("x(");
        axes.append(ui->dim1->itemText(i));
        axes.append(")");
    }
    axes.append("\n\nY: ");
    for(int i = 0; i < ui->dim2->count(); i++){
        if(i!=0){
            axes.append(" + ");
        }
        axes.append(QString::number(basis(i,ui->dim2->currentIndex())));
        axes.append(" (");
        axes.append(ui->dim2->itemText(i));
        axes.append(")");
    }


//    stringstream basisStream;
//    basisStream << mGT.getBasis();
//    QString basis = QString::fromStdString(basisStream.str());

    QMessageBox msgBox;
    msgBox.setText(axes);
    msgBox.setTextInteractionFlags(Qt::TextSelectableByMouse);
    msgBox.exec();

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
