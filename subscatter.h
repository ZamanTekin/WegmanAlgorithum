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
    explicit SubScatter(const int &dimension1, const int &dimension2, const Eigen::MatrixXd &data, QWidget *parent = nullptr);
    ~SubScatter();

private:
    QScatterSeries *m_scatter;
    int dim1;
    int dim2;

signals:

public slots:
    void updateDimensions(const int &dimension1, const int &dimension2);
    void updateData(const Eigen::MatrixXd &data);
};

#endif // SUBSCATTER_H
