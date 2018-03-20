#include "subscatter.h"
#include <QDebug>
#include <ctime>
#include <vector>
#include <cmath>

SubScatter::SubScatter(const int &dimension1, const int &dimension2, const Eigen::MatrixXd &data, const int markersize, QWidget *parent) : QChartView(new QChart(), parent),
    m_scatter(0),
    dim1(dimension1),
    dim2(dimension2),
    size(markersize)
{
    //intialise scatterseries
    m_scatter = new QScatterSeries();
    for (int i = 0; i < data.rows(); i ++) {
            *m_scatter << QPointF(data(i,dim1), data(i,dim2));
    }

    //enable hardware acceleration for speed
    m_scatter->setUseOpenGL(true);

    m_scatter->setMarkerSize(size);
    chart()->addSeries(m_scatter);
    chart()->createDefaultAxes();

    //enables zooming - drag with LMB for zoom in, right clkikc zzom out - implement custom zoom eventually
    this->setRubberBand(QChartView::RectangleRubberBand);

}

SubScatter::~SubScatter()
{
    // figure out what QT handles and what needs to go here
}


void SubScatter::updateDimensions(const int &dimension1, const int &dimension2)
{
    dim1 = dimension1;
    dim2 = dimension2;
    //maybe need to update data in some use cases
}
void SubScatter::updateDim1(const int &dimension1){
    dim1 = dimension1;
}
void SubScatter::updateDim2(const int &dimension2){
    dim2 = dimension2;
}

// updates data shown - deleting and adding new data slower, though recentres camera on data
void SubScatter::updateData(const Eigen::MatrixXd &data)
{
    clock_t time0 = clock();

    QList<QPointF> points;
    for (int i = 0; i < data.rows(); i ++) {
        points.append(QPointF(data(i,dim1), data(i,dim2)));
    }
    clock_t time1 = clock();
    m_scatter->replace(points);
    clock_t time2 = clock();
//    qDebug() << (float)(time2-time0)/CLOCKS_PER_SEC << ", " << (float)(time1-time0)/CLOCKS_PER_SEC << ", " << (float)(time2-time1)/CLOCKS_PER_SEC;
}

// update seems redundant
void SubScatter::setMarkerSize(const int markersize){
    size = markersize;
    m_scatter->setMarkerSize(size);
    chart()->update();
}
