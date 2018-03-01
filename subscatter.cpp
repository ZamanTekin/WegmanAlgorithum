#include "subscatter.h"

SubScatter::SubScatter(const int &dimension1, const int &dimension2, const Eigen::MatrixXd &data, QWidget *parent) : QChartView(new QChart(), parent),
    m_scatter(0),
    dim1(dimension1),
    dim2(dimension2)
{
    m_scatter = new QScatterSeries();
//    m_scatter->setName("scatter1");
    for (int i = 0; i < data.rows(); i ++) {
            *m_scatter << QPointF(data(i,dim1), data(i,dim2));
    }

    m_scatter->setMarkerSize(4);
    chart()->addSeries(m_scatter);
    chart()->createDefaultAxes();
    chart()->axisX()->setRange(-60, 60);
    chart()->axisY()->setRange(-60, 60);

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
void SubScatter::updateData(const Eigen::MatrixXd &data)
{
    chart()->removeAllSeries();
    m_scatter = new QScatterSeries();
    for (int i = 0; i < data.rows(); i ++) {
            *m_scatter << QPointF(data(i,dim1), data(i,dim2));
    }
    m_scatter->setMarkerSize(4);
    chart()->addSeries(m_scatter);
}
