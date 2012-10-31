#include "AI.h"
#include "util.h"

AI::AI(Connection* conn) : BaseAI(conn) {}

const char* AI::username()
{
  return "Shell AI";
}

const char* AI::password()
{
  return "password";
}

//This function is run once, before your first turn.
void AI::init(){}

//This function is called each time it is your turn.
//Return true to end your turn, return false to ask the server for updated information.
bool AI::run()
{
  for(int ii=0; ii<creatures.size();ii++)
  {/*
    if (creatures[ii].owner() == playerID())
    {
      if (getPlantAtLocation(creatures[ii].x()+1,creatures[ii].y()) == NULL)// && getCreatureAtLocation(creatures[ii].x()+1,creatures[ii].y) == NULL)
      {
        if(0<creatures[ii].x()+1<mapWidth() && 0<creatures[ii].y<mapHeight())
	{
          creatures[ii].move(creatures[ii].x()+1,creatures[ii].y());
	}
      }
      /*
		Plant plant = getPlantAtLocation(creatures[ii].x()+1,creatures[ii].y());
		Creature creat = getCreatureAtLocation(creatures[ii].x()+1,creatures[ii].y());
		if (plant != NULL && plant.size()>0)
		{
			creatures[ii].eat(creatures[ii].x()+1,creatures[ii].y());
		}
		else if (creat!=NULL && creat.owner()!=playerID())
		{
			creatures[ii].eat(creatures[ii].x()+1,creatures[ii].y());
		}
		else if (creat!=NULL && creat.owner()==playerID())
		{
			creatures[ii].breed(creat);
		}*/
	}
  return true;
}

//This function is run once, after your last turn.
void AI::end(){}
