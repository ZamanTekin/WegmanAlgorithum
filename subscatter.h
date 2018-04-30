#ifndef SUBSCATTER_H
#define SUBSCATTER_H

#include <Eigen/Dense>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QChartGlobal>
#include <QtCharts/QChartView>

QT_CHARTS_USE_NAMESPACE

class SubScatter : public QChartView
{
    Q_OBJECT
public:
    explicit SubScatter(const int &dimension1, const int &dimension2, const Eigen::MatrixXd &data, const int markersize = 5, QWidget *parent = nullptr);
    ~SubScatter();

private:
    QScatterSeries *m_scatter;
    int dim1;
    int dim2;
    int size;

signals:

public slots:
    void updateDimensions(const int &dimension1, const int &dimension2);
    void updateDim1(const int &dimension1);
    void updateDim2(const int &dimension2);
    void updateData(const Eigen::MatrixXd &data);
    void setMarkerSize(const int markersize);
};

#endif // SUBSCATTER_H
