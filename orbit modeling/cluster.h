#ifndef CLUSTER_H
#define CLUSTER_H

#include "point.h"
#include "coreLib.h"
#include "orbitLib.h"

class Cluster : public QObject
{

Q_OBJECT

public:
    Cluster(QVector<cSatellite> _tlePointVector, QColor color, Point *_planet, double G, double _timemultiplier, bool _isTle);

    Cluster(int N, QVector<double> mean, QVector<double> stddev, QColor color, Point *_planet, double G, double _timemultiplier);


    double timeMultiplier;
    Point *planet;
    double G;

    bool isTle;

    QVector<Point*> pointVector;

    QVector<cSatellite> tlePointVector;

    void addPoint(Point *newPoint);
    void update();
    void reset();
};

#endif // CLUSTER_H
