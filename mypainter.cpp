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
        PointData d1 = pointList[i - 1];
        PointData d2 = pointList[i];
        pen.setWidthF(d1.w);
        painter.setPen(pen);
        path.quadTo(d1.p, d2.p);
        if (d1.w - d2.w > 0.0000001) {
            painter.strokePath(path, pen);
            path.clear();
            path.moveTo(d2.p);
        } else {
            path.lineTo(d2.p);
        }
    }
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
    paint1();
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
