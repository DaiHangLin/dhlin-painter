#ifndef LINEINFO_H
#define LINEINFO_H

#include <QObject>
#include <QPainterPath>

class lineInfo : public QObject
{
    Q_OBJECT
public:
    explicit lineInfo(QObject *parent = nullptr);
    lineInfo(const lineInfo &line);

    qreal getW() const;
    void setW(const qreal &value);

    QPainterPath getPath() const;
    void setPath(const QPainterPath &value);

private:
    QPainterPath path;
    qreal w;

signals:

public slots:
};

#endif // LINEINFO_H
