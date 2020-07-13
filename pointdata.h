#ifndef POINTDATA_H
#define POINTDATA_H

#include <QObject>
#include <QPointF>

class PointData : public QObject
{
    Q_OBJECT
public:
    explicit PointData(QObject *parent = nullptr);

    PointData(const PointData &d);

    QPointF p;
    qint64 t;
    qreal w;

signals:

public slots:
};

#endif // POINTDATA_H
