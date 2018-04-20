#ifndef SUBSCATTER_H
#define SUBSCATTER_H

#include <Eigen/Dense>
#include <vector>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QChartGlobal>
#include <QtCharts/QChartView>
#include <QRubberBand>

QT_CHARTS_USE_NAMESPACE

// scatter plot to show specified dimensions of a larger data matrix
// doesn't store matrix, but takes from exterior
class SubScatter : public QChartView
{
    Q_OBJECT
public:
    explicit SubScatter(const int &dimension1, const int &dimension2, const Eigen::MatrixXd &data, const int markersize = 5, QWidget *parent = nullptr);
    ~SubScatter();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
//    QScatterSeries *m_scatter1;
//    QScatterSeries *m_scatter2;
    std::vector<QScatterSeries*> m_scatters;
    struct P{
        size_t group;
        QPointF point;
    };
    std::vector<P> groups;
    QRubberBand *rubberBand;
    QPoint mouseorigin;
    int dim1;
    int dim2;
    int size;
    int m_group;

signals:

private slots:
//    void handleSelect();

public slots:
    void updateDimensions(const int &dimension1, const int &dimension2);
    void updateDim1(const int &dimension1);
    void updateDim2(const int &dimension2);
    void updateData(const Eigen::MatrixXd &data);
    void setMarkerSize(const int markersize);
    void setGroup(const int &group);
    void createGroup(const QColor &color);
};

#endif // SUBSCATTER_H
