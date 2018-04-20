#include "subscatter.h"
#include <QDebug>
#include <ctime>
#include <vector>
#include <cmath>

SubScatter::SubScatter(const int &dimension1, const int &dimension2, const Eigen::MatrixXd &data, const int markersize, QWidget *parent) : QChartView(new QChart(), parent),
    dim1(dimension1),
    dim2(dimension2),
    size(markersize)
{
    //intialise scatterseries

    qDebug() << "initialising scatter";
    QScatterSeries *scatter = new QScatterSeries(this);
    for (int i = 0; i < data.rows(); i ++) {
        P p;
        p.point = QPointF(data(i,dim1), data(i,dim2));
        p.group = 1;
        *scatter << p.point;
        groups.push_back(p);
    }
    scatter->setUseOpenGL(true);
    scatter->setMarkerSize(size);
    scatter->setColor(Qt::blue);
    chart()->addSeries(scatter);
    m_scatters.push_back(scatter);

    chart()->setMargins(QMargins());
    chart()->legend()->setVisible(false);
    chart()->createDefaultAxes();

    //enables zooming - drag with LMB for zoom in, right click zoom out - implement custom zoom eventually
    this->setGroup(0);

//    connect(scene(), &QGraphicsScene::selectionChanged,
//            this, &SubScatter::handleSelect);

}

SubScatter::~SubScatter()
{
    // figure out what QT handles and what needs to go here
}

void SubScatter::mousePressEvent(QMouseEvent *event)
{
    mouseorigin = QPoint(event->pos());
    QChartView::mousePressEvent(event);
}

void SubScatter::mouseMoveEvent(QMouseEvent *event)
{
    QChartView::mouseMoveEvent(event);
}

void SubScatter::mouseReleaseEvent(QMouseEvent *event)
{
    QPoint mousecorner(event->pos());
    QPointF origin = chart()->mapToValue(chart()->mapFromScene(this->mapToScene(mouseorigin)), m_scatters[0]);
    QPointF corner = chart()->mapToValue(chart()->mapFromScene(this->mapToScene(mousecorner)), m_scatters[0]);

    if (m_group != 0){
    qDebug() << "Handling selection";
    for(auto it = groups.begin(); it != groups.end(); it++){
        if ((((it->point).x() < corner.x()) && ((it->point).x() > origin.x()) && (corner.x() > origin.x()))
        || (((it->point).x() > corner.x()) && ((it->point).x() < origin.x()) && (corner.x() < origin.x()))){
            if ((((it->point).y() < corner.y()) && ((it->point).y() > origin.y()) && (corner.y() > origin.y()))
            || (((it->point).y() > corner.y()) && ((it->point).y() < origin.y()) && (corner.y() < origin.y()))){
                qDebug() << "Handling selected point";
                qDebug() << (it->point).x() << ", " << (it->point).y() << ", " << origin.x() << ", " << origin.y();
                if (it->group != m_group){
                    qDebug() << "moving to " << m_group;
                    m_scatters[it->group-1]->remove(it->point);
                    m_scatters[m_group-1]->append(it->point);
                    it->group = m_group;
                }
            }
        }
    }
    }
    QChartView::mouseReleaseEvent(event);
}

//void SubScatter::handleSelect()
//{

//    qDebug() << "Handling selection";
//    for(auto it = groups.begin(); it != groups.end(); it++){
//        if (scene()->selectionArea().contains(it->point)){

//            if (it->group != m_group){
//                it->group = m_group;
//                if (m_group = 1){
//                    m_scatter2->remove(it->point);
//                    m_scatter1->append(it->point);
//                }
//                else{
//                    m_scatter1->remove(it->point);
//                    m_scatter2->append(it->point);
//                }
//            }
//        }
//    }
//}


void SubScatter::updateDimensions(const int &dimension1, const int &dimension2)
{
    dim1 = dimension1;
    dim2 = dimension2;
    chart()->update();
    //maybe need to store data to update in same frame
}
void SubScatter::updateDim1(const int &dimension1){
    dim1 = dimension1;
    chart()->update();
}
void SubScatter::updateDim2(const int &dimension2){
    dim2 = dimension2;
    chart()->update();
}

// updates data shown - deleting and adding new data slower, though recentres 'camera' on data
void SubScatter::updateData(const Eigen::MatrixXd &data)
{
//    clock_t time0 = clock();

    std::vector<QList<QPointF>> points(m_scatters.size());
    for (int i = 0; i < data.rows(); i ++) {
        groups[i].point=QPointF(data(i,dim1), data(i,dim2));
        points[groups[i].group-1].append(groups[i].point);
    }
//    clock_t time1 = clock();
    for (size_t i = 0; i < points.size(); i++){
        m_scatters[i]->replace(points[i]);
    }
//    clock_t time2 = clock();
//    qDebug() << (float)(time2-time0)/CLOCKS_PER_SEC << ", " << (float)(time1-time0)/CLOCKS_PER_SEC << ", " << (float)(time2-time1)/CLOCKS_PER_SEC;
}

// update seems redundant
void SubScatter::setMarkerSize(const int markersize){
    size = markersize;
    for (size_t i = 0; i < m_scatters.size(); i++){
        m_scatters[i]->setMarkerSize(size);
    }
    chart()->update();
}

void SubScatter::setGroup(const int &group)
{
    m_group = group;
    if (group == 0){
        this->setRubberBand(QChartView::RectangleRubberBand);
        this->setDragMode(QGraphicsView::NoDrag);
        qDebug() << "Zoom set";
    }
    else{
        this->setRubberBand(QChartView::NoRubberBand);
        this->setDragMode(QGraphicsView::RubberBandDrag);
        qDebug() << "Select " << m_group  << " set";
    }
}

void SubScatter::createGroup(const QColor &color)
{
    QScatterSeries *scatter = new QScatterSeries(this);
    scatter->setUseOpenGL(true);
    scatter->setMarkerSize(size);
    scatter->setColor(color);
    chart()->addSeries(scatter);
    chart()->createDefaultAxes();
    m_scatters.push_back(scatter);
}
