#include "masterplot.h"

#include <QDebug>

MasterPlot::MasterPlot(const Eigen::MatrixXd data, const int size, QWidget *parent):
    QWidget(parent),
    markersize(size),
    grid(new QGridLayout)
{
    int dimCount = data.cols();
    for (int i = 0; i < dimCount-1; i++){
        //build subhists here

        SubScatter *scatter = new SubScatter(i,i,data,size,this);
        grid->addWidget(scatter, i, i);
        scatters.append(scatter);


        for (int j = i+1; j < dimCount; j++){

            qDebug() << i << ',' << j;
            SubScatter *scatter = new SubScatter(i,j,data,size,this);
            grid->addWidget(scatter, j, i);
            scatters.append(scatter);
        }
    }

    // holdover for subhist
    SubScatter *scatter = new SubScatter(data.cols()-1,data.cols()-1,data,size,this);
    grid->addWidget(scatter, data.cols()-1, data.cols()-1);
    scatters.append(scatter);

    this->setLayout(grid);
}

void MasterPlot::updateData(const Eigen::MatrixXd data)
{
    for (auto i = scatters.begin(); i != scatters.end(); i++){
        (*i)->updateData(data);
    }
}

void MasterPlot::setMarkerSize(const int size)
{
    for (auto i = scatters.begin(); i != scatters.end(); i++){
        (*i)->setMarkerSize(size);
    }
}
