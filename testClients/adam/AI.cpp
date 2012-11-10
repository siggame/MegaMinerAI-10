#include "AI.h"
#include "util.h"
#include <cmath>
#include <cstdlib>

//This is Adam Harter's AI.
//I don't recomend copying the style; it's awful and terrible, but I don't care.
//Good luck reading this, dude.

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
   int dist=999999;
   //Make formula better(?)
   for(int i=0;i<plants.size();i++)
   {
      float checkdist;
      if(plants[i].size()>0)
      {
         checkdist=(abs(c->x()-plants[i].x())+abs(c->y()-plants[i].y()))*3
                   -5*c->herbivorism();
         if(checkdist<dist)
         {
            dist=checkdist;
            returnP=&plants[i];
         }
      }
   }
   for(int i=0;i<creatures.size();i++)
   {
      if(creatures[i].owner()!=owner)
      {
         float checkdist;
         checkdist=(abs(c->x()-creatures[i].x())+abs(c->y()-creatures[i].y()))*3
                   -(10*(c->carnivorism()-creatures[i].defense()));

         int damage=10*(c->carnivorism()-creatures[i].defense());
         int distance=abs(c->x()-creatures[i].x())+abs(c->y()-creatures[i].y());


         if(damage>=creatures[i].currentHealth()&&
            distance-1<=c->movementLeft()&&
            c->currentHealth()/3>c->movementLeft())
         {
            return &creatures[i];
         }

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

//this function is amazingly awful
void testEat(Creature* c,int xadj,int yadj,int playerID,
             std::vector<Creature>& cs,AI* a)
{
   if(a->getCreatureAtLocation(c->x()+xadj,c->y()+yadj)==-1||
      cs[a->getCreatureAtLocation(c->x()+xadj,c->y()+yadj)].owner()!=playerID)
   {
      c->eat(c->x()+xadj,c->y()+yadj);
   }
}

bool safe(std::vector<Creature>& cs,Creature* c,int playerID)
{
   for(int i=0;i<cs.size();i++)
   {
      if(cs[i].owner()!=playerID)
      {
         int distance=abs(cs[i].x()-c->x())+abs(cs[i].y()-c->y());
         if(distance<=cs[i].speed())
         {
            return false;
         }
      }
   }
   return true;
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
         int distance=abs(yum->x()-best->x())+abs(yum->y()-best->y());
         bool eatStuff=best->movementLeft()<=distance;
         //add actual path finding... maybe
         for(int i=best->movementLeft();i>0;i--)
         {
            if(abs(yum->x()-best->x())+abs(yum->y()-best->y())!=1)
            {
               if(eatStuff)
               {
                  testEat(best,-1, 0,playerID(),creatures,this);
                  testEat(best, 1, 0,playerID(),creatures,this);
                  testEat(best, 0,-1,playerID(),creatures,this);
                  testEat(best, 0, 1,playerID(),creatures,this);
               }
               bool horizontalPass=true;
               bool verticalPass=true;
               bool moved;
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
                  verticalPass=best->move(best->x(),best->y()-1);
               }
               else if(!horizontalPass||yum->y()>best->y())
               {
                  verticalPass=best->move(best->x(),best->y()+1);
               }
            }
         }
         if(getPlantAtLocation(yum->x(),yum->y())==-1)
         {
            best->eat(yum->x(),yum->y());
         }
         else
         {
            if(best->maxHealth()-best->currentHealth()>=best->herbivorism()*5)
            {
               best->eat(yum->x(),yum->y());
            }
         }
         if(distance!=1)
         {
            testEat(best,-1, 0,playerID(),creatures,this);
            testEat(best, 1, 0,playerID(),creatures,this);
            testEat(best, 0,-1,playerID(),creatures,this);
            testEat(best, 0, 1,playerID(),creatures,this);
         }
         if(safe(creatures,best,playerID()))
         {
            int stuff=getCreatureAtLocation(best->x()+1,best->y());
            if(stuff!=-1)
            {
               best->breed(creatures[stuff]);
            }
            stuff=getCreatureAtLocation(best->x()-1,best->y());
            if(stuff!=-1)
            {
               best->breed(creatures[stuff]);
            }
            stuff=getCreatureAtLocation(best->x(),best->y()+1);
            if(stuff!=-1)
            {
               best->breed(creatures[stuff]);
            }
            stuff=getCreatureAtLocation(best->x(),best->y()-1);
            if(stuff!=-1)
            {
               best->breed(creatures[stuff]);
            }
         }
      }
   }
   return true;
}

//This function is run once, after your last turn.
void AI::end(){}
