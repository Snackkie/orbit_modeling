#include "point.h"

Point::Point()
{

}

Point::Point(QVector<double> _coord, double _mass, double _radius, QColor _color, bool isDecart)
{
    if(isDecart)
    {
        origCoord=_coord;
    }
    else
    {
        origCoord[0]=_coord[0]*cos(_coord[2]);
        origCoord[2]=_coord[0]*sin(_coord[2]);
        origCoord[1]=-_coord[1]*sin(_coord[2])-_coord[3]*cos(_coord[2]);
        origCoord[3]=_coord[1]*cos(_coord[2])+_coord[3]*sin(_coord[2]);
    }
    currCoord=origCoord;
    prevCoord=origCoord;

    radius=_radius;
    mass=_mass;
    color= _color;

    rectXY.setSize(QSize(2*radius,2*radius));
    rectXY.moveCenter(QPointF(currCoord[0],currCoord[1]));

    rectXZ.setSize(QSize(2*radius,2*radius));
    rectXZ.moveCenter(QPointF(currCoord[0],currCoord[2]));
}

void Point::updateCoord(QVector<Point*> pointVector, Point *planet, int id, double timeMultiplier, double G)
{
    double d_t=1;

    double x=prevCoord[0], y=prevCoord[1], z=prevCoord[2];

    //double critAcceleration=0.1*V(); //1% скорости
    double critDist=1000;

    double acceleration;
    double projectionX, projectionY, projectionZ;

    projectionX=(x-planet->prevCoord[0])/dist(planet);
    projectionY=(y-planet->prevCoord[1])/dist(planet);
    projectionZ=(z-planet->prevCoord[2])/dist(planet);


    acceleration=-G*planet->mass/pow(dist(planet),2);


    //r
    currCoord[0]=prevCoord[0] + timeMultiplier*d_t*prevCoord[3];
    currCoord[1]=prevCoord[1] + timeMultiplier*d_t*prevCoord[4];
    currCoord[2]=prevCoord[2] + timeMultiplier*d_t*prevCoord[5];
    //V
    currCoord[3]=prevCoord[3]+timeMultiplier*d_t*(acceleration*projectionX); // acceleration from planet
    currCoord[4]=prevCoord[4]+timeMultiplier*d_t*(acceleration*projectionY); // -/-
    currCoord[5]=prevCoord[5]+timeMultiplier*d_t*(acceleration*projectionZ); // -/-

    if(pointVector.size()!=1)
    {
        for(int i=0;i<pointVector.size();i++)
        {
            if(i!=id && dist(pointVector[i])!=0)
            {
                if(dist(pointVector[i])<critDist)
                {
                    acceleration=-G*pointVector[i]->mass/pow(dist(pointVector[i]),2);

                    projectionX=(x-pointVector[i]->prevCoord[0])/dist(pointVector[i]);
                    projectionY=(y-pointVector[i]->prevCoord[0])/dist(pointVector[i]);
                    projectionZ=(z-pointVector[i]->prevCoord[0])/dist(pointVector[i]);

                    currCoord[3]+=timeMultiplier*d_t*(acceleration*projectionX);
                    currCoord[4]+=timeMultiplier*d_t*(acceleration*projectionY);
                    currCoord[5]+=timeMultiplier*d_t*(acceleration*projectionZ);
                }
            }
        }
    }
    rectXY.moveCenter(QPointF(currCoord[0],currCoord[1]));
    rectXZ.moveCenter(QPointF(currCoord[0],currCoord[2]));
}

bool operator==(Point a, Point b)
{
    if(a.currCoord==b.currCoord && a.prevCoord==b.prevCoord)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool operator!=(Point a, Point b)
{
    if(a==b)
    {
        return false;
    }
    else
    {
        return true;
    }
}

double Point::dist(Point *b)
{
    return sqrt(pow(prevCoord[0]-b->prevCoord[0],2)+pow(prevCoord[1]-b->prevCoord[1],2)+pow(prevCoord[2]-b->prevCoord[2],2));
}

double Point::distXY(Point *b)
{
    return sqrt(pow(prevCoord[0]-b->prevCoord[0],2)+pow(prevCoord[1]-b->prevCoord[1],2));
}
double Point::distXZ(Point *b)
{
    return sqrt(pow(prevCoord[0]-b->prevCoord[0],2)+pow(prevCoord[2]-b->prevCoord[2],2));
}

double Point::V()
{
    return sqrt(pow(prevCoord[3],2)+pow(prevCoord[4],2)+pow(prevCoord[5],2));
}


