#include "cluster.h"

Cluster::Cluster(QVector<cSatellite> _tlePointVector, QColor _color, Point *_planet, double _G, double _timeMultiplier, bool _isTle)
{
    timeMultiplier=_timeMultiplier;
    planet=_planet;
    G=_G;
    tlePointVector=_tlePointVector;

    isTle=_isTle;

    QVector<double> tempCoord;
    for(int i=0;i<tlePointVector.size();i++)
    {
        cEciTime eci=tlePointVector[i].PositionEci(0);
        tempCoord << eci.Position().m_x << eci.Position().m_y << eci.Position().m_z
                  << eci.Velocity().m_x << eci.Velocity().m_y << eci.Velocity().m_z;
        addPoint(new Point(tempCoord, 5,5*pow(10,1), _color,true));

        tempCoord.clear();
    }
}


Cluster::Cluster(int N, QVector<double> mean, QVector<double> stddev, QColor color, Point *_planet, double _G, double _timeMultiplier)
{
    timeMultiplier=_timeMultiplier;
    planet=_planet;
    G=_G;

    mean[2]=pi*mean[2]/180;
    stddev[2]=pi*stddev[2]/180;
    QVector<double> coord({0,0,0,0});
    for(int i = 0; i < N; ++i)
    {
        for(int i=0;i<4;i++)
        {
            coord[i]=mean[i]+((long double)rand()/RAND_MAX)*(stddev[i]-mean[i]);
        }

        Point *newpoint=new Point(coord,5,5*pow(10,1), color, false);

        addPoint(newpoint);
    }
}

void Cluster::addPoint(Point *newPoint)
{
    if(!pointVector.contains(newPoint))
    {
        pointVector.push_back(newPoint);
    }
}

void Cluster::update()
{
    for(int i=0;i<pointVector.size();i++)
    {
        pointVector[i]->prevCoord=pointVector[i]->currCoord;
    }
    for(int i=0;i<pointVector.size();i++)
    {
        pointVector[i]->updateCoord(pointVector, planet, i, timeMultiplier, G);
    }
}

void Cluster::reset()
{
    for(int i=0;i<pointVector.size();i++)
    {
        pointVector[i]->currCoord=pointVector[i]->origCoord;
        pointVector[i]->prevCoord=pointVector[i]->origCoord;
        pointVector[i]->rectXY.moveCenter(QPointF(pointVector[i]->currCoord[0],pointVector[i]->currCoord[1]));
        pointVector[i]->rectXZ.moveCenter(QPointF(pointVector[i]->currCoord[1],pointVector[i]->currCoord[2]));
    }
}
