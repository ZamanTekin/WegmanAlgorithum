#ifndef MASTERPLOT_H
#define MASTERPLOT_H

#include "subscatter.h"
//#include "subhist.h"
#include <QWidget>
#include <Eigen/Dense>
#include <QList>
#include <QGridLayout>

class MasterPlot : public QWidget
{
    Q_OBJECT
public:
    explicit MasterPlot(const Eigen::MatrixXd data, const int size = 5, QWidget *parent = nullptr);

private:
    QList<SubScatter*> scatters;
//    QList<SubHist> hists;
    int markersize;
    QGridLayout *grid;

signals:
    void graphClicked(int dim1, int dim2);

public slots:
    void updateData(const Eigen::MatrixXd data);
    void setMarkerSize(const int size);
};

#endif // MASTERPLOT_H
