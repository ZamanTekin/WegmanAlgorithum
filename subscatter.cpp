#include "subscatter.h"
#include <QDebug>
#include <ctime>
#include <vector>

SubScatter::SubScatter(const int &dimension1, const int &dimension2, const Eigen::MatrixXd &data, const int markersize, QWidget *parent) : QChartView(new QChart(), parent),
    m_scatter(0),
    dim1(dimension1),
    dim2(dimension2),
    size(markersize)
{
    m_scatter = new QScatterSeries();
//    m_scatter->setName("scatter1");
    for (int i = 0; i < data.rows(); i ++) {
            *m_scatter << QPointF(data(i,dim1), data(i,dim2));
    }

    //dynamic range
 /*   std::vector<double> separations;
    double sep = 0;
    double mean = 0;
    for(int i = 0; i < data.rows(); i++){
        for(int j = 0; j < data.rows(); j++){
            for(int k = 0; k < data.cols(); k++){
                sep += (data(j,k)-data(i,k))*(data(j,k)-data(i,k));
                mean += data(j,k);
            }
            separations.push_back(sep);
            sep = 0;
        }
    }
    mean = mean/(data.rows()*data.rows()*data.cols());
    sep = *max_element(separations.begin(),separations.end());*/

    m_scatter->setMarkerSize(size);
    chart()->addSeries(m_scatter);
    chart()->createDefaultAxes();
   /* chart()->axisX()->setRange(mean-sep/2, mean+sep/2);
    chart()->axisY()->setRange(mean-sep/2, mean+sep/2);*/

}

SubScatter::~SubScatter()
{
}


void SubScatter::updateDimensions(const int &dimension1, const int &dimension2)
{
    dim1 = dimension1;
    dim2 = dimension2;
    //maybe need to update data? would require current data to be st6ored to do here however
}
void SubScatter::updateDim1(const int &dimension1){
    dim1 = dimension1;
}
void SubScatter::updateDim2(const int &dimension2){
    dim2 = dimension2;
}

void SubScatter::updateData(const Eigen::MatrixXd &data)
{
    clock_t time0 = clock();
    chart()->removeAllSeries();

    m_scatter = new QScatterSeries();
    for (int i = 0; i < data.rows(); i ++) {
            *m_scatter << QPointF(data(i,dim1), data(i,dim2));
    }
    m_scatter->setMarkerSize(size);
    clock_t time1 = clock();
    chart()->addSeries(m_scatter);
    clock_t time2 = clock();
    qDebug() << (float)(time2-time0)/CLOCKS_PER_SEC << ", " << (float)(time1-time0)/CLOCKS_PER_SEC << ", " << (float)(time2-time1)/CLOCKS_PER_SEC;
}
void SubScatter::setMarkerSize(const int markersize){
    size = markersize;
    m_scatter->setMarkerSize(size);
}
