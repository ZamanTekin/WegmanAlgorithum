#include "subscatter.h"
#include <QDebug>
#include <ctime>
#include <vector>
#include <cmath>

SubScatter::SubScatter(const int &dimension1, const int &dimension2, const Eigen::MatrixXd &data, const int markersize, QWidget *parent) : QChartView(new QChart(), parent),
    m_scatter1(0),
    m_scatter2(0),
    dim1(dimension1),
    dim2(dimension2),
    size(markersize)
{
    //intialise scatterseries

    qDebug() << "initialising scatter";
    m_scatter1 = new QScatterSeries();
    for (int i = 0; i < data.rows(); i ++) {
        P p;
        p.point = QPointF(data(i,dim1), data(i,dim2));
        p.group = 1;
        *m_scatter1 << p.point;
        groups.push_back(p);
    }
    m_scatter2 = new QScatterSeries();

    //enable hardware acceleration for speed
    m_scatter1->setUseOpenGL(true);
    m_scatter2->setUseOpenGL(true);

    m_scatter1->setMarkerSize(size);
    m_scatter2->setMarkerSize(size);
    m_scatter1->setColor(Qt::blue);
    m_scatter2->setColor(Qt::red);
    chart()->addSeries(m_scatter1);
    chart()->addSeries(m_scatter2);
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
    QPointF origin = chart()->mapToValue(chart()->mapFromScene(this->mapToScene(mouseorigin)), m_scatter1);
    QPointF corner = chart()->mapToValue(chart()->mapFromScene(this->mapToScene(mousecorner)), m_scatter1);

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
                    it->group = m_group;
                    if (m_group == 1){
                        qDebug() << "moving to 1";
                        m_scatter2->remove(it->point);
                        m_scatter1->append(it->point);
                    }
                    else{
                        qDebug() << "moving to 2";
                        m_scatter1->remove(it->point);
                        m_scatter2->append(it->point);
                        it->group = 2;
                    }
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

    QList<QPointF> points1;
    QList<QPointF> points2;
    for (int i = 0; i < data.rows(); i ++) {
        groups[i].point=QPointF(data(i,dim1), data(i,dim2));
        if (groups[i].group == 1){
        points1.append(groups[i].point);
        }
        if (groups[i].group == 2){
        points2.append(groups[i].point);
        }
    }
//    clock_t time1 = clock();
    m_scatter1->replace(points1);
    m_scatter2->replace(points2);
//    clock_t time2 = clock();
//    qDebug() << (float)(time2-time0)/CLOCKS_PER_SEC << ", " << (float)(time1-time0)/CLOCKS_PER_SEC << ", " << (float)(time2-time1)/CLOCKS_PER_SEC;
}

// update seems redundant
void SubScatter::setMarkerSize(const int markersize){
    size = markersize;
    m_scatter1->setMarkerSize(size);
    m_scatter2->setMarkerSize(size);
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
