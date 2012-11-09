#include "AI.h"
#include "util.h"
#include <cmath>

AI::AI(Connection* conn) : BaseAI(conn) {}

const char* AI::username()
{
  return "YO";
}

const char* AI::password()
{
  return "password";
}

Creature* best=NULL;
Mappable* yum=NULL;

//This function is run once, before your first turn.
void AI::init()
{
   for(int i=0;i<creatures.size();i++)
   {
      if(creatures[i].owner()==playerID())
      {
         if(best==NULL||best->herbivorism()<creatures[i].herbivorism())
         {
            best=&creatures[i];
         }
      }
   }
}

Mappable* findNearest(Creature* c,std::vector<Plant>& plants,
                      std::vector<Creature>& creatures,int owner)
{
   Mappable* returnP;
   float dist=9999999999999999;
   for(int i=0;i<plants.size();i++)
   {
      float checkdist;
      checkdist=sqrt(pow(plants[i].x()-c->x(),2)+pow(plants[i].y()-c->y(),2))
                /(c->herbivorism()*5);
      if(checkdist<dist)
      {
         dist=checkdist;
         returnP=&plants[i];
      }
   }
   for(int i=0;i<creatures.size();i++)
   {
      if(creatures[i].owner()!=owner)
      {
         float checkdist;
         checkdist=sqrt(pow(creatures[i].x()-c->x(),2)+
                        pow(creatures[i].y()-c->y(),2))
                   /(c->carnivorism()*10);
         if(checkdist<dist)
         {
            dist=checkdist;
            returnP=&creatures[i];
         }
      }
   }
   return returnP;
}

Mappable* checkThere(Mappable* m,std::vector<Creature>& creatures,
                     std::vector<Plant>& plants)
{
   for(int i=0;i<creatures.size();i++)
   {
      if(m==&creatures[i])
      {
         return m;
      }
   }
   for(int i=0;i<plants.size();i++)
   {
      if(m==&plants[i])
      {
         return m;
      }
   }
   return NULL;
}

int getIndex(Mappable* m,std::vector<Plant>& plants,
             std::vector<Creature>& creatures)
{
   for(int i=0;i<plants.size();i++)
   {
      if(m==&plants[i])
      {
         return i;
      }
   }
   for(int i=0;i<creatures.size();i++)
   {
      if(m==&creatures[i])
      {
         return i+plants.size();
      }
   }
   return -1;
}


//This function is called each time it is your turn.
//Return true to end your turn, return false to ask the server for updated information.
bool AI::run()
{
   bool taken[creatures.size()+plants.size()];
   for(int i=0;i<creatures.size();i++)
   {
      if(creatures[i].owner()==playerID())
      {
         best=&creatures[i];
         yum=findNearest(best,plants,creatures,playerID());
         taken[getIndex(yum,plants,creatures)]=true;
         for(int i=best->movementLeft();i>0;i--)
         {
            bool horizontalPass=true;
            if(yum->x()<best->x())
            {
               horizontalPass=best->move(best->x()-1,best->y());
            }
            else if(yum->x()>best->x())
            {
               horizontalPass=best->move(best->x()+1,best->y());
            }
            if(!horizontalPass||yum->y()<best->y())
            {
               best->move(best->x(),best->y()-1);
            }
            else if(!horizontalPass||yum->y()>best->y())
            {
               best->move(best->x(),best->y()+1);
            }
            if(best->canEat())
            {
               if(best->maxHealth()-best->energy()>=5*best->herbivorism())
               {
                  best->eat(yum->x(),yum->y());
               }
            }
         }
      }
   }
   return true;
   /*
  //Iterate through every creature
  for(int ii=0;ii<creatures.size();ii++)
  { //if I own the creature
    if (creatures[ii].owner() == playerID())
    { //check if there is a plant to that creature's left
      int plantIn = getPlantAtLocation(creatures[ii].x()+1,creatures[ii].y());
      //if there is no plant to my left, or there is a plant of size 0, and there is no creature to my left
      if ((plantIn == -1 || (plantIn !=1 && plants[plantIn].size()==0)) && getCreatureAtLocation(creatures[ii].x()+1,creatures[ii].y()) == -1)
      { //if x is in the range of the map, and y is in the range of the map
        if(0<=creatures[ii].x()+1 && creatures[ii].x()+1<mapWidth() && 0<=creatures[ii].y() && creatures[ii].y()<mapHeight())
        { //if I have ennough health to move, and have movment left
          if (creatures[ii].currentHealth()>healthPerMove() && creatures[ii].movementLeft()>0)
	  { //move creature to the left by incrementing its x cooridinate, and not changing its y
	    creatures[ii].move(creatures[ii].x()+1,creatures[ii].y());
          }
	}
      }

      //check if there is a plant to my left
      plantIn = getPlantAtLocation(creatures[ii].x()+1,creatures[ii].y());
      //check if there is a creature to my left
      int creatIn = getCreatureAtLocation(creatures[ii].x()+1,creatures[ii].y());
      //if there is a plant to my left, and its size is > 0, and this creature has not already eaten this turn
      if (plantIn != -1 && plants[plantIn].size()>0 && creatures[ii].canEat()==1)
      {
        //eat the plant, using its x and y
        creatures[ii].eat(plants[plantIn].x(),plants[plantIn].y());
      }
      //else if there is a creature to my left, and it is not my creature and this creature has not eaten yet
      else if (creatIn!=-1 && creatures[creatIn].owner()!=playerID() && creatures[ii].canEat()==1)
      { //take a bite out of this creature
        creatures[ii].eat(creatures[creatIn].x(),creatures[creatIn].y());
      }
      //else if there is a creature to my left, and it is my creature, and neither has bred this turn
      else if (creatIn!=-1 && creatures[creatIn].owner()==playerID() && creatures[ii].canBreed()==1 && creatures[creatIn].canBreed()==1)
      { //if both creatures have enough health to breed
        if (creatures[ii].currentHealth()>healthPerBreed() && creatures[creatIn].currentHealth()>healthPerBreed())
        { //breed with this creature
          creatures[ii].breed(creatures[creatIn]);
        }
      }
    }
  }
  */
}

//This function is run once, after your last turn.
void AI::end(){}
