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
    QPen pen;
    QTimer *timer;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

signals:

public slots:
};

#endif // MYPAINTER_H
