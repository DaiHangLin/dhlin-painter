#include "lineinfo.h"

lineInfo::lineInfo(QObject *parent) : QObject(parent)
{

}

lineInfo::lineInfo(const lineInfo &line)
{
    this->path = line.getPath();
    this->w = line.getW();
}

qreal lineInfo::getW() const
{
    return w;
}

void lineInfo::setW(const qreal &value)
{
    w = value;
}

QPainterPath lineInfo::getPath() const
{
    return path;
}

void lineInfo::setPath(const QPainterPath &value)
{
    path = value;
}
