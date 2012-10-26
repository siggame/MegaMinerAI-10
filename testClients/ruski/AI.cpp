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

//DISTANCE FORMULAS
float AI::distCC(Creature& c1, Creature& c2)
{
  return sqrt( pow(c1.x()-c2.x(),2)+pow(c1.y()-c2.y() ,2) );
}
float AI::distCP(Creature& c1,Plant& p1)
{
  return sqrt( pow(c1.x()-p1.x(),2)+pow(c1.y()-p1.y(),2) );
}
float AI::distPP(Point2D& p1, Point2D& p2)
{
  return sqrt( pow(p1.x-p2.x,2)+pow(p1.y-p2.y,2) );
}

Point2D AI::findValid(Point2D loc, Point2D death)
{
 Point2D far = loc;
 Point2D pt[4];
 pt[0].x = loc.x-1;    pt[0].y = loc.y; //LEFT
 pt[1].x = loc.x+1;    pt[1].y = loc.y; //RIGHT
 pt[2].x = loc.x;    pt[2].y = loc.y+1; //UP
 pt[3].x = loc.x;    pt[3].y = loc.y-1; //DOWN

 bool isValid;

 //CHECK
 for(int i = 0; i < 4; i++)
 {
  isValid = true;
  for(int ii = 0; ii < mappables.size(); ii++)
  {
   if(mappables[ii].x() == pt[i].x && mappables[ii].y() == pt[i].y)
   {
    isValid = false;
    break;
   }
  }
  //CHECK IF OFF SCREEN
  if( pt[i].x < 0 || pt[i].x > mapWidth() || pt[i].y < 0 || pt[i].y > mapHeight() )
  {
   isValid = false;
  }
  //THE POINT IS VALID
  if(isValid == true)
  { 
   //SWAP IF CURRENT IS FARTHER THAN FARTHEST
   if( distPP(far, death) < distPP(pt[i], death) )
    far = pt[i];
  }
 }
 return far;
}

//This function is called each time it is your turn.
//Return true to end your turn, return false to ask the server for updated information.
bool AI::run()
{
  //Gather creatures
  for(int i = 0; i < creatures.size(); i++)
  {
    if( creatures[i].owner() == playerID() )
      myCreatures.push_back( &creatures[i] );
    else
      enemyCreatures.push_back( &creatures[i] );
  }
  
  for(int i = 0; i < myCreatures.size(); i++)
  {
    //MOVE IF ENEMY IS NEARBY
    int enemyThresh = 2;
    for(int ii = 0; ii < enemyCreatures.size(); ii++)
    {
      //NEARBY ENEMY FOUND
      if( distCC(*myCreatures[i], *enemyCreatures[ii]) < enemyThresh )
      {
        Point2D loc;
        loc.x = myCreatures[i]->x();
        loc.y = myCreatures[i]->y();
        Point2D death;
        death.x = enemyCreatures[ii]->x();
        death.y = enemyCreatures[ii]->y();
        
        Point2D moveTo = findValid(loc, death);
        myCreatures[i]->move(moveTo.x, moveTo.y);
      }
    }
    
    //Move toward food.
    
    //Mate if close and near food.
  }
  
  

  return true;
}

//This function is run once, after your last turn.
void AI::end(){}
