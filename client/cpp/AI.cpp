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
  for(int ii=0;ii<creatures.size();ii++)
  {
    if (creatures[ii].owner() == playerID())
    {
      int plantIn = getPlantAtLocation(creatures[ii].x()+1,creatures[ii].y());
      if ((plantIn == -1 || (plantIn !=1 && plants[plantIn].size()==0)) && getCreatureAtLocation(creatures[ii].x()+1,creatures[ii].y()) == -1)
      {
        if(0<=creatures[ii].x()+1 && creatures[ii].x()+1<mapWidth() && 0<=creatures[ii].y() && creatures[ii].y()<mapHeight())
        {
          if (creatures[ii].currentHealth()>healthPerMove() && creatures[ii].movementLeft()>0)
	  {
	    creatures[ii].move(creatures[ii].x()+1,creatures[ii].y());
          }
	}
      }      
      plantIn = getPlantAtLocation(creatures[ii].x()+1,creatures[ii].y());
      int creatIn = getCreatureAtLocation(creatures[ii].x()+1,creatures[ii].y());
      if (plantIn != -1 && plants[plantIn].size()>0 && creatures[ii].canEat()==1)
      {
        creatures[ii].eat(plants[plantIn].x(),plants[plantIn].y());
      }
      else if (creatIn!=-1 && creatures[creatIn].owner()!=playerID() && creatures[ii].canEat()==1)
      {
        creatures[ii].eat(creatures[creatIn].x(),creatures[creatIn].y());
      }
      else if (creatIn!=-1 && creatures[creatIn].owner()==playerID() && creatures[ii].canBreed()==1 && creatures[creatIn].canBreed()==1)
      {
        if (creatures[ii].currentHealth()>=healthPerBreed() && creatures[creatIn].currentHealth()>=healthPerBreed())
        {
          creatures[ii].breed(creatures[creatIn]);
        }
      }
    }
  }
  return true;
}

//This function is run once, after your last turn.
void AI::end(){}
