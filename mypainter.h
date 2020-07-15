#ifndef MYPAINTER_H
#define MYPAINTER_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QList>
#include <pointdata.h>
#include <QTimer>
#include <lineinfo.h>
#include <QDateTime>

class MyPainter : public QWidget
{
    Q_OBJECT
public:
    explicit MyPainter(QWidget *parent = nullptr);

private:
    QPointF lastPoint;
    QPointF point;
    QList<PointData> pointList;
    QList<lineInfo> lineList;
    qreal lastWidth;
    qreal maxWidth;
    qreal minWidth;
    qreal speed;
    qreal duration;
    int frameCount;
    int fps;
    QPen pen;
    QTimer *timer;

    QPainterPath tempPath;
    QPainterPath allPath;


    qreal getWidthImpl(qint64 pt, qint64 ct);
    void getWidth1();
    void getWidth2();
    void getWidthBySegPath();
    void paint1();
    void paint2();
    void paint3();
    void paint4();
    void paintBySegPath();
    void addPoints(QVector<QPointF> &list, QPointF &p);
    void addPoints2(QVector<QPointF> &list, QPointF &p);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

signals:

public slots:
};

#endif // MYPAINTER_H
