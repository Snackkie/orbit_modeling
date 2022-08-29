#ifndef POINT_H
#define POINT_H

#include <QVector>
#include <QColor>
#include <QPainterPath>
#include <iostream>
const double pi = 3.14159265359;

class Point
{
public:
    QVector<double> currCoord{0,0,0,0,0,0};
    QVector<double> prevCoord{0,0,0,0,0,0};
    QVector<double> origCoord{0,0,0,0,0,0};

    double mass, radius;
    QColor color;
    QRectF rectXY;
    QRectF rectXZ;

    Point();
    Point(QVector<double> _coord, double _mass, double _radius, QColor _color, bool isDecart);

    void updateCoord(QVector<Point*> pointVector, Point *planet, int id, double timeMultiplier, double G);
    double dist(Point *b);
    double distXY(Point *b);
    double distXZ(Point *b);
    double V();
};

bool operator==(Point a, Point b);
bool operator!=(Point a, Point b);

#endif // POINT_H
