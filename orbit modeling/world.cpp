#include "world.h"
#include <iostream>

World::World()
{
    timeEpoch=0;
    timeMultiplier=20;
    G = 6.67408*pow(10, -20); // km^3/kg/c^2
    planet=new Point({0,0,0,0,0,0}, 5.9722*pow(10,24), 	6371, QColor(100,20,100), true);
}

void World::update()
{

    for(int i=0;i<clusterVector.size();i++)
    {
        if(!clusterVector[i]->isTle)
        {
            clusterVector[i]->update();
        }
        else
        {
            for(int j=0; j<clusterVector[i]->pointVector.size();j++)
            {
                double timeEpochMinutes=timeEpoch/60;
                cEciTime eci=clusterVector[i]->tlePointVector[j].PositionEci(timeEpochMinutes);
                clusterVector[i]->pointVector[j]->rectXY.moveCenter(QPointF(eci.Position().m_x,eci.Position().m_y));
                clusterVector[i]->pointVector[j]->rectXZ.moveCenter(QPointF(eci.Position().m_x,eci.Position().m_z));
            }
        }
    }
    timeEpoch+=timeMultiplier;
}

void World::addCluster(Cluster *newCluster)
{

    if(!clusterVector.contains(newCluster))
    {
        clusterVector.push_back(newCluster);
    }
}

void World::reset()
{

    for(int i=0;i<clusterVector.size();i++)
    {
        clusterVector[i]->reset();
    }
    timeEpoch=0;
}

void World::runUpdate()
{
    update();
}
