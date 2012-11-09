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
void AI::init()
{
  srand(time(NULL));
}

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
    if( myCreatures[i].cre->herbivorism() > myCreatures[i].cre->carnivorism() + 2 )
      myCreatures[i].type = "herbivore";
    else if( myCreatures[i].cre->herbivorism() + 2 < myCreatures[i].cre->carnivorism() )
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

  //CURRENT LOCATION
  Point2D cur;
  cur.x = myCreatures[i].cre->x();
  cur.y = myCreatures[i].cre->y();

  //CLOSEST PLANT
  Point2D cp;
  float cpDist;

  int attempts = 0;
  while(myCreatures[i].cre->movementLeft() > 0 && attempts < 100)
  {
    cp = closestPlant( cur.x, cur.y );
    cpDist = dist( cur.x, cur.y, cp.x, cp.y);

    //Attempt to eat
    if(cpDist == 1)
    {
      myCreatures[i].cre->eat(cp.x, cp.y);
    }

    //----MOVE LEFT AND RIGHT----
    if(cur.x != cp.x)
    {
      //MOVE RIGHT IF CURRENT LOCATION IS LESS THAN THE TARGET LOCATION
      if( cur.x < cp.x )
      {
        //MOVE RANDOMLY UP OR DOWN IF CANNOT MOVE
        if(! myCreatures[i].cre->move(cur.x + 1, cur.y) )
        {
          if(rand()%2 == 0)
            myCreatures[i].cre->move(cur.x, cur.y + 1);
          else
            myCreatures[i].cre->move(cur.x, cur.y - 1);
          attempts++;
        }
      }
      else if( cur.x > cp.x )
      {
        //MOVE RANDOMLY UP OR DOWN IF CANNOT MOVE
        if(! myCreatures[i].cre->move(cur.x - 1, cur.y) )
        {
          if(rand()%2 == 0)
            myCreatures[i].cre->move(cur.x, cur.y + 1);
          else
            myCreatures[i].cre->move(cur.x, cur.y - 1);
          attempts++;
        }
      }
    }
    //----MOVE UP AND DOWN----
    else
    {
      //MOVE UP IF CURRENT LOCATION IS LESS THAN THE TARGET LOCATION
      if( cur.y < cp.y )
      {
        //MOVE RANDOMLY UP OR DOWN IF CANNOT MOVE
        if(! myCreatures[i].cre->move(cur.x, cur.y - 1) )
        {
          if(rand()%2 == 0)
            myCreatures[i].cre->move(cur.x + 1, cur.y);
          else
            myCreatures[i].cre->move(cur.x - 1, cur.y);
          attempts++;
        }
      }
      else if( cur.x > cp.x )
      {
        //MOVE RANDOMLY UP OR DOWN IF CANNOT MOVE
        if(! myCreatures[i].cre->move(cur.x, cur.y + 1) )
        {
          if(rand()%2 == 0)
            myCreatures[i].cre->move(cur.x + 1, cur.y);
          else
            myCreatures[i].cre->move(cur.x - 1, cur.y);
          attempts++;
        }
      }
    }
  }

}
void AI::carnact(int i)
{
  herbact(i);
  return;
}
void AI::omniact(int i)
{
  herbact(i);
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