#include "mypainter.h"

MyPainter::MyPainter(QWidget *parent)
    : QWidget(parent)
    , lastPoint()
    , point()
    , pointList(QList<PointData>())
    , lineList(QList<lineInfo>())
    , lastWidth(1)
    , maxWidth(7)
    , minWidth(1)
    , speed(0.5)
    , duration(240)
    , frameCount(12)
    , fps(60)
    , pen(QPen(QBrush("red"), 1, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin))
    , timer(nullptr)
    , tempPath(QPainterPath())
{
    timer = new QTimer();
    connect(timer, &QTimer::timeout, [=]() {
        getWidth1();
        update();
    });
}

qreal MyPainter::getWidthImpl(qint64 pt, qint64 now)
{
    qint64 delta = now - pt;
    if (delta <= 0.0000001) {
        return minWidth;
    }
    qreal index = delta / frameCount;
    qreal frameWidth = (maxWidth - minWidth) * qreal(1.0) / frameCount;
    qreal w = qMin(frameWidth * index + minWidth, maxWidth);

    return w;
}

void MyPainter::getWidth1()
{
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    for (int i = 0; i < pointList.size(); i ++) {
        PointData &data = pointList[i];
        if (data.w >= maxWidth) {
            continue;
        }
        data.w = getWidthImpl(data.t, now);
    }
}

void MyPainter::getWidth2()
{
    for (int i = 0; i < pointList.size(); i ++) {
        PointData &data = pointList[i];
        data.w = qMin(data.w + speed, maxWidth);
    }
}

void MyPainter::getWidthBySegPath()
{
    QPainterPath path;
    lineList.clear();
    if (pointList.size() > 0) {
        path.moveTo(pointList[0].p);
        pointList[0].w = qMin(pointList[0].w + speed, maxWidth);
    }
    for (int i = 1; i < pointList.size(); i ++) {
        PointData d1 = pointList[i - 1];
        PointData d2 = pointList[i];
        qreal lw = d1.w;
        qreal w = qMin(d2.w + speed, maxWidth);
        pointList[i].w = w;
        if (lw - w <= 0.0000001) {
            path.lineTo(d2.p);
        } else {
            lineInfo i;
            i.setPath(path);
            i.setW(lw);
            lineList << i;
            path.clear();
            path.moveTo(d1.p);
        }
    }
}

void MyPainter::paint1()
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    QPainterPath path;
    if (pointList.size() <= 0) {
        return;
    }
    path.moveTo(pointList[0].p);
    for (int i = 1; i < pointList.size(); i ++) {
        PointData &d1 = pointList[i - 1];
        PointData &d2 = pointList[i];
        pen.setWidthF(d1.w);
        painter.setPen(pen);
//        path.quadTo(d1.p, d2.p);
//        if (d1.w - d2.w > 0.0000001) {
//            painter.strokePath(path, pen);
//            path.clear();
//            path.moveTo(d2.p);
//        } else {
//            path.lineTo(d2.p);
//        }
        painter.drawLine(d1.p, d2.p);
    }

}

void MyPainter::addPoints(QVector<QPointF> &list, QPointF &p)
{
    if (list.isEmpty()) {
        tempPath.moveTo(p);
        list << p;
        return;
    }

    tempPath.quadTo(lastPoint, (lastPoint + p) / 2);


    for (qreal t = 0; t <= 1; t = t + 0.1) {
        qDebug() << "tempPath.pointAtPercent(t)" << tempPath.pointAtPercent(t);
        list << tempPath.pointAtPercent(t);
    }
    QPointF e = tempPath.currentPosition();
    tempPath.clear();
    tempPath.moveTo(e);
}

void MyPainter::addPoints2(QVector<QPointF> &list, QPointF &p)
{
    if (list.isEmpty()) {
        list << p;
        return;
    }
    QPointF bp = list.last();
    QPointF cp = lastPoint;
    QPointF ep = (lastPoint + p) / 2;
    for (qreal t = 0; t < 1; t += 0.01) {
        qreal x = (1-t) * (1-t)*bp.rx()
            + 2 * t * (1-t) * cp.rx()
            + t * t * ep.rx();
        qreal y = (1-t) * (1-t)*bp.ry()
            + 2 * t * (1-t) * cp.ry()
            + t * t * ep.ry();
        list << QPointF(x, y);
    }
}

void MyPainter::paint2()
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    QPainterPath path;
    pen.setWidthF(14);
    painter.setPen(pen);
    QPointF p0(50, 50);
    QPointF p1(100, 100);
    QPointF p2(50, 150);
    QPointF p3(0, 100);

    QVector<QPointF> list = QVector<QPointF>();

//    list << p0 << p1 << p2 << p3;

    addPoints2(list, p0);
    lastPoint = p0;
    addPoints2(list, p1);
    lastPoint = p1;
    addPoints2(list, p2);
    lastPoint = p2;
    addPoints2(list, p3);
    lastPoint = p3;
    addPoints2(list, p3);
    qDebug() << "list last" << list.last();
            pen.setWidthF(13);
        pen.setColor(Qt::blue);
        painter.setPen(pen);
      // painter.drawPolyline(list);

//    path.moveTo(p0);
//    for(int i=1; i< list.size(); i++) {
//        QPointF &d1 = list[i - 1];
//        QPointF &d2 = list[i];

//        pen.setWidthF(13);
//        pen.setColor(Qt::blue);
//        painter.setPen(pen);

//        painter.drawLine(d1, d2);

////        path.quadTo(d1, (d1 + d2) / 2);

////        path.lineTo(d2);
////        painter.strokePath(path, pen);

////        QPointF endPoint = path.currentPosition();
////        path.clear();
////        path.moveTo(endPoint);
//    }

//    painter.strokePath(path, pen);

    for (int i = 0; i < list.size(); i ++) {
        QPointF &d1 = list[i];
        pen.setWidthF(12);
        pen.setColor(Qt::black);
        painter.setPen(pen);
        painter.drawPoint(d1);
    }

    pen.setColor(Qt::red);
    painter.setPen(pen);
    painter.drawPoint(p0);
    painter.drawPoint(p1);
    painter.drawPoint(p2);
     painter.drawPoint(p3);

}

void MyPainter::paint3()
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    QPainterPath path;
    if (pointList.size() <= 0) {
        return;
    }
    path.moveTo(pointList[0].p);
    for (int i = 1; i < pointList.size(); i ++) {
        PointData &d1 = pointList[i - 1];
        PointData &d2 = pointList[i];
        pen.setWidthF(d1.w);
        painter.setPen(pen);
        path.quadTo(d1.p, (d1.p + d2.p) / 2);
        painter.drawPath(path);
        QPointF endPoint = path.currentPosition();
        path &= QPainterPath();
        path.moveTo(endPoint);
    }
}

void MyPainter::paint4()
{
    QPainter p(this);
    QPainterPath p1(QPointF(10,10));
    p1.quadTo(QPointF(100,50), QPointF(100,200));
    p1.moveTo(50, 10);
    p1.quadTo(QPointF(150,50), QPointF(150,200));
    QPen pen(Qt::black);
    pen.setWidth(10);
    p.strokePath(p1, pen);
}

void MyPainter::paintBySegPath()
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    for (int i = 0; i < lineList.size(); i ++) {
        lineInfo line = lineList.at(i);
        QPainterPath path = line.getPath();

        qreal w = line.getW();
        pen.setWidthF(w);
        painter.strokePath(path, pen);
    }
}

void MyPainter::paintEvent(QPaintEvent *)
{
//    paint3();
    paint2();
}

void MyPainter::mousePressEvent(QMouseEvent *event)
{
    lastPoint = event->pos();
    pointList.clear();
    PointData d;
    d.p = event->pos();
    d.t = QDateTime::currentMSecsSinceEpoch();
    d.w = 1;
    pointList << d;
    update();
    timer->start(1000 / fps);
}

void MyPainter::mouseMoveEvent(QMouseEvent *event)
{
    point = event->pos();
    PointData d;
    d.p = event->pos();
    d.t = QDateTime::currentMSecsSinceEpoch();
    d.w = 1;
    pointList << d;
    update();
}

void MyPainter::mouseReleaseEvent(QMouseEvent *event)
{
    point = event->pos();
    PointData d;
    d.p = event->pos();
    d.t = QDateTime::currentMSecsSinceEpoch();
    d.w = 1;
    pointList << d;
    update();
    timer->stop();
}
