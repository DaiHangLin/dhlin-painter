#include "pointdata.h"

PointData::PointData(QObject *parent) : QObject(parent)
{

}

PointData::PointData(const PointData &d)
{
    this->p = d.p;
    this->t = d.t;
    this->w = d.w;
}
