#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <cluster.h>

#include <QThread>

class World : public QObject
{

Q_OBJECT

public:
    QVector<Point*> pointVector;
    QVector<Cluster*> clusterVector;
    QVector<Cluster*> clusterTLE;
    QVector<Cluster*> clusterNoTLE;

    int pointCount;

    double timeMultiplier;
    double timeEpoch;

    Point *planet;
    double G;

    World();

    void update();
    void addPoint(Point *newPoint);
    void addCluster(Cluster* newCluster);
    void reset();

public slots:
    void runUpdate();
};

#endif // WORLD_H
