//Russley Shaw 11/4/2012
#include "AI.h"
#include "util.h"

AI::AI(Connection* conn) : BaseAI(conn) {}

const char* AI::username()
{
  return "Ruski";
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
  cout << turnNumber() << ": START TURN" << endl;
  update();
  for(int i = 0; i < myCreatures.size(); i++)
  {
    //HERBIVORE
    if( myCreatures[i].type.compare("herbivore") == 0 )
      herbact(i);
    //CARNIVORE
    else if( myCreatures[i].type.compare("carnivore") == 0 )
      carnact(i);
    //OMNIVORE
    else
      omniact(i);
  }
  cout << turnNumber() << ": END TURN" << endl;
  return true;
}

//This function is run once, after your last turn.
void AI::end(){}

//RUSKI FUNCTIONS
void AI::update()
{
  cout << "CLEARING CREATURE ARRAYS" << endl;
  myCreatures.erase(myCreatures.begin(), myCreatures.end());
  enemyCreatures.erase(enemyCreatures.begin(), enemyCreatures.end());

  cout << "BUILDING CREATURE ARRAYS" << endl;
  for(int i = 0; i < creatures.size(); i++)
  {
    //MY CREATURES
    if( creatures[i].owner() == playerID() )
    {
      myCreatures.push_back( RuskiCre(&creatures[i]) );
    }
    //ENEMY CREATURES
    else
    {
      enemyCreatures.push_back( &creatures[i] );
    }
  }
}

void AI::detType()
{
  cout << "DETERMINING TYPE" << endl;
  for(int i = 0; i < myCreatures.size(); i++)
  {
    //DETERMINE IF HERBIVORE OR CARNIVORE OR OMNIVORE
    if( myCreatures[i].cre->herbivorism() > myCreatures[i].cre->carnivorism() + 1 )
      myCreatures[i].type = "herbivore";
    else if( myCreatures[i].cre->herbivorism() + 1 < myCreatures[i].cre->carnivorism() )
      myCreatures[i].type = "carnivore";
    else
      myCreatures[i].type = "omnivore";
  }
  return;
}

void AI::herbact(int i)
{
  //Determine target
  Point2D next;
  Point2D cp = closestPlant( myCreatures[i].cre->x(), myCreatures[i].cre->x() );
  myCreatures[i].tgt = cp;

  int attempts = 0;
  while(myCreatures[i].cre->movementLeft() > 0 && attempts < 10)
  {
    //Recalculate closest
    cp = closestPlant( myCreatures[i].cre->x(), myCreatures[i].cre->y() );
    myCreatures[i].tgt = cp;

    //UP/DOWN
    if(myCreatures[i].cre->x() == cp.x)
    {
      //UP
      if(myCreatures[i].cre->y() < cp.y)
      {
        next.x = myCreatures[i].cre->x();
        next.y = myCreatures[i].cre->y() + 1;
        if(! myCreatures[i].cre->move(next.x, next.y) )
          attempts++;
      }
      //DOWN
      else
      {
        next.x = myCreatures[i].cre->x();
        next.y = myCreatures[i].cre->y() - 1;
        if(! myCreatures[i].cre->move(next.x, next.y) )
          attempts++;
      }
    }
    //LEFT/RIGHT
    else
    {
      //RIGHT
      if(myCreatures[i].cre->x() < cp.x)
      {
        next.x = myCreatures[i].cre->x() + 1;
        next.y = myCreatures[i].cre->y();
        if(! myCreatures[i].cre->move(next.x, next.y) )
          attempts++;
      }
      //LEFT
      else
      {
        next.x = myCreatures[i].cre->x() - 1;
        next.y = myCreatures[i].cre->y();
        if(! myCreatures[i].cre->move(next.x, next.y) )
          attempts++;
      }
    }
  }
  //If near target, eat
  cp = closestPlant( myCreatures[i].cre->x(), myCreatures[i].cre->y() );
  if( dist(myCreatures[i].cre->x(), myCreatures[i].cre->y(), cp.x, cp.y) )
  {
    cout << "ATTEMPTING TO EAT PLANT";
    myCreatures[i].cre->eat(cp.x, cp.y);
  }
  return;
}
void AI::carnact(int i)
{
  //Determine target
  Point2D next;
  Point2D cp = closestEnemy( myCreatures[i].cre->x(), myCreatures[i].cre->x() );
  myCreatures[i].tgt = cp;

  int attempts = 0;
  while(myCreatures[i].cre->movementLeft() > 0 && attempts < 10)
  {
    //Recalculate closest
    cp = closestEnemy( myCreatures[i].cre->x(), myCreatures[i].cre->y() );
    myCreatures[i].tgt = cp;

    //UP/DOWN
    if(myCreatures[i].cre->x() == cp.x)
    {
      //UP
      if(myCreatures[i].cre->y() < cp.y)
      {
        next.x = myCreatures[i].cre->x();
        next.y = myCreatures[i].cre->y() + 1;
        if(! myCreatures[i].cre->move(next.x, next.y) )
          attempts++;
      }
      //DOWN
      else
      {
        next.x = myCreatures[i].cre->x();
        next.y = myCreatures[i].cre->y() - 1;
        if(! myCreatures[i].cre->move(next.x, next.y) )
          attempts++;
      }
    }
    //LEFT/RIGHT
    else
    {
      //RIGHT
      if(myCreatures[i].cre->x() < cp.x)
      {
        next.x = myCreatures[i].cre->x() + 1;
        next.y = myCreatures[i].cre->y();
        if(! myCreatures[i].cre->move(next.x, next.y) )
          attempts++;
      }
      //LEFT
      else
      {
        next.x = myCreatures[i].cre->x() - 1;
        next.y = myCreatures[i].cre->y();
        if(! myCreatures[i].cre->move(next.x, next.y) )
          attempts++;
      }
    }
  }
  //If near target, eat
  cp = closestPlant( myCreatures[i].cre->x(), myCreatures[i].cre->y() );
  if( dist(myCreatures[i].cre->x(), myCreatures[i].cre->y(), cp.x, cp.y) )
  {
    cout << "ATTEMPTING TO EAT PLANT";
    myCreatures[i].cre->eat(cp.x, cp.y);
  }
  return;
}
void AI::omniact(int i)
{
  carnact(i);
  return;
}

float AI::dist(int x1, int y1, int x2, int y2)
{
  return sqrt( pow((x1-x2),2)+pow((y1-y2),2) );
}

Point2D AI::closestPlant(int x, int y)
{
  Point2D pt;
  double closest = 10000;
  double current = 0;

  for(int i = 0; i < plants.size(); i++)
  {
    //TODO: Check if the plant has already been targetted
    if(plants[i].size() < 1)
      continue;
    //If the distance between xy and the current closest is larger than the next plant
    current = dist(x,y,plants[i].x(),plants[i].y());
    if( current < closest )
    {
      closest = current;
      pt.x = plants[i].x();
      pt.y = plants[i].y();
    }
  }
  return pt;
}

Point2D AI::closestEnemy(int x, int y)
{
  Point2D pt;
  double closest = 10000;
  double current = 0;

  for(int i = 0; i < enemyCreatures.size(); i++)
  {
    //TODO: Check if the plant has already been targetted

    //If the distance between xy and the current closest is larger than the next plant
    current = dist(x,y,enemyCreatures[i]->x(),enemyCreatures[i]->y());
    if( current < closest )
    {
      closest = current;
      pt.x = enemyCreatures[i]->x();
      pt.y = enemyCreatures[i]->y();
    }
  }
  return pt;
}