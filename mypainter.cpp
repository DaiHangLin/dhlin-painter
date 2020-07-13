#include "mypainter.h"

MyPainter::MyPainter(QWidget *parent)
    : QWidget(parent)
    , lastPoint()
    , point()
    , pointList(QList<PointData>())
    , lineList(QList<lineInfo>())
    , lastWidth(1)
    , maxWidth(10)
    , minWidth(1)
    , speed(0.5)
    , pen(QPen(QBrush("red"), 1, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin))
    , timer(nullptr)
{
    timer = new QTimer();
    connect(timer, &QTimer::timeout, [=]() {
        QPainterPath path;
        lineList.clear();
        for (int i = 0; i < pointList.size(); i ++) {
            pointList[i].w = qMin(pointList[i].w + speed, maxWidth);
        }
//        if (pointList.size() > 0) {
//            path.moveTo(pointList[0].p);
//            pointList[0].w = qMin(pointList[0].w + speed, maxWidth);
//        }
//        for (int i = 1; i < pointList.size(); i ++) {
//            PointData d1 = pointList[i - 1];
//            PointData d2 = pointList[i];
//            qreal lw = d1.w;
//            qreal w = qMin(d2.w + speed, maxWidth);
//            pointList[i].w = w;
//            if (lw - w <= 0.0000001) {
//                path.lineTo(d2.p);
//            } else {
//                lineInfo i;
//                i.setPath(path);
//                i.setW(lw);
//                lineList << i;
//                path.clear();
//                path.moveTo(d1.p);
//            }
//        }
        update();
    });
}

void MyPainter::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    QPainterPath path;
    if (pointList.size() <= 0) {
        return;
    }
    path.moveTo(pointList[0].p);
    for (int i = 1; i < pointList.size(); i ++) {
        PointData d1 = pointList[i - 1];
        PointData d2 = pointList[i];
        pen.setWidthF(d1.w);
        painter.setPen(pen);
        if (d1.w - d2.w <= 0.0000001) {
            path.lineTo(d2.p);
        } else {
            painter.strokePath(path, pen);
            path.clear();
            path.moveTo(d1.p);
        }
        if (d1.w < maxWidth)
            painter.drawLine(d1.p, d2.p);
    }
    painter.strokePath(path, pen);
//    qDebug() << "line list size" << lineList.size();
//    for (int i = 0; i < lineList.size(); i ++) {
//        lineInfo line = lineList.at(i);
//        QPainterPath path = line.getPath();

//        qreal w = line.getW();
//        //qDebug() << "w" << w;
//        pen.setWidthF(w);
//        painter.strokePath(path, pen);
//    }
}

void MyPainter::mousePressEvent(QMouseEvent *event)
{
    lastPoint = event->pos();
    pointList.clear();
    PointData d;
    d.p = event->pos();
    d.t = clock();
    d.w = 1;
    pointList << d;
    update();
    timer->start(20);
}

void MyPainter::mouseMoveEvent(QMouseEvent *event)
{
    point = event->pos();
    PointData d;
    d.p = event->pos();
    d.t = clock();
    d.w = 1;
    pointList << d;
    update();
}

void MyPainter::mouseReleaseEvent(QMouseEvent *event)
{
    point = event->pos();
    PointData d;
    d.p = event->pos();
    d.t = clock();
    d.w = 1;
    pointList << d;
    update();
    timer->stop();
}
