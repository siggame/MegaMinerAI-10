//Russley Shaw 11/4/2012
#include "AI.h"
#include "util.h"

AI::AI(Connection* conn) : BaseAI(conn) {}

const char* AI::username()
{
  return "Ruski!";
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
  detType();
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
  cout << "ENDED CREATURE ARRAY CREATION" << endl;
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
  cout << "HERBIVORE FOR " << i << endl;
  //Determine target
  Point2D next;

  //CURRENT LOCATION
  Point2D cur;
  cur.x = myCreatures[i].cre->x();
  cur.y = myCreatures[i].cre->y();

  //CLOSEST PLANT
  Point2D cp;
  float cpDist;

  int attempt = 0;
  cout << "STARTING HERB WHILE LOOP" << endl;
  while(myCreatures[i].cre->movementLeft() > 0 && attempt < 100)
  {
    //UPDATE
    cur.x = myCreatures[i].cre->x();
    cur.y = myCreatures[i].cre->y();
    cp = closestPlant( cur.x, cur.y );
    cpDist = dist( cur.x, cur.y, cp.x, cp.y);
    
    cout<<"at "<<cur.x<<','<<cur.y<<" heading to "<<cp.x<<','<<cp.y<<endl;
    
    //ATTEMPT TO EAT
    if(cpDist == 1)
    {
      cout<<"uhhhhh"<<endl;
      myCreatures[i].cre->eat(cp.x, cp.y);
      return;
    }

    //MOVE RIGHT - CURRENT X LESS THAN CLOSEST X
    if( cur.x < cp.x )
    {
      cout << "HEADING RIGHT" << endl;
      if( !myCreatures[i].cre->move(cur.x + 1, cur.y) )
      {
        if(rand()%2 == 0)
          myCreatures[i].cre->move(cur.x, cur.y + 1);
        else
          myCreatures[i].cre->move(cur.x, cur.y - 1);
        cout << "FAILED MOVEMENT" << endl;
        attempt++;
      }
    }
    //MOVE LEFT IF NOT OVER LOCATION
    else if( cur.x > cp.x )
    {
      cout << "HEADING LEFT" << endl;
      if( !myCreatures[i].cre->move(cur.x - 1, cur.y) )
      {
        if(rand()%2 == 0)
          myCreatures[i].cre->move(cur.x, cur.y + 1);
        else
          myCreatures[i].cre->move(cur.x, cur.y - 1);
        cout << "FAILED MOVEMENT" << endl;
        attempt++;
      }
    }
    //DIRECTLY OVER - MOVE UP/DOWN
    //MOVE UP - CURRENT Y LESS THAN CLOSEST Y
    else if( cur.y > cp.y )
    {
      cout << "HEADING UP" << endl;
      if( !myCreatures[i].cre->move(cur.x, cur.y - 1) )
      {
        if(rand()%2 == 0)
          myCreatures[i].cre->move(cur.x + 1, cur.y);
        else
          myCreatures[i].cre->move(cur.x - 1, cur.y);
        cout << "FAILED MOVEMENT" << endl;
        attempt++;
      }
    }
    //MOVE DOWN - CURRENT Y GREATER THAN CLOSEST Y
    else if( cur.y < cp.y )
    {
      cout << "HEADING DOWN" << endl;
      if( !myCreatures[i].cre->move(cur.x, cur.y + 1) )
      {
        if(rand()%2 == 0)
          myCreatures[i].cre->move(cur.x + 1, cur.y);
        else
          myCreatures[i].cre->move(cur.x - 1, cur.y);
        cout << "FAILED MOVEMENT" << endl;
        attempt++;
      }
    }
    //DIRECTLY ON TOP OF TARGET
    else
    {
      cout << "LOL WHUT IS GOING ON IDK MAHN" << endl;
      //LOLWHUT
    }
    //ATTEMPT TO EAT
    cp = closestPlant( cur.x, cur.y );
    cpDist = dist( cur.x, cur.y, cp.x, cp.y);
    if(cpDist == 1)
    {
      myCreatures[i].cre->eat(cp.x, cp.y);
      return;
    }
  }
}
void AI::carnact(int i)
{
  cout << "CARNIVORE FOR " << i << endl;
  //Determine target
  Point2D next;

  //CURRENT LOCATION
  Point2D cur;
  cur.x = myCreatures[i].cre->x();
  cur.y = myCreatures[i].cre->y();

  //CLOSEST PLANT
  Point2D cp;
  float cpDist;

  int attempt = 0;
  cout << "STARTING HERB WHILE LOOP" << endl;
  while(myCreatures[i].cre->movementLeft() > 0 && attempt < 100)
  {
    //UPDATE
    cur.x = myCreatures[i].cre->x();
    cur.y = myCreatures[i].cre->y();
    cp = closestEnemy( cur.x, cur.y );
    cpDist = dist( cur.x, cur.y, cp.x, cp.y);
    
    cout<<"at "<<cur.x<<','<<cur.y<<" heading to "<<cp.x<<','<<cp.y<<endl;
    
    //ATTEMPT TO EAT
    if(cpDist == 1)
    {
      cout<<"uhhhhh"<<endl;
      myCreatures[i].cre->eat(cp.x, cp.y);
      return;
    }

    //MOVE RIGHT - CURRENT X LESS THAN CLOSEST X
    if( cur.x < cp.x )
    {
      cout << "HEADING RIGHT" << endl;
      if( !myCreatures[i].cre->move(cur.x + 1, cur.y) )
      {
        if(rand()%2 == 0)
          myCreatures[i].cre->move(cur.x, cur.y + 1);
        else
          myCreatures[i].cre->move(cur.x, cur.y - 1);
        cout << "FAILED MOVEMENT" << endl;
        attempt++;
      }
    }
    //MOVE LEFT IF NOT OVER LOCATION
    else if( cur.x > cp.x )
    {
      cout << "HEADING LEFT" << endl;
      if( !myCreatures[i].cre->move(cur.x - 1, cur.y) )
      {
        if(rand()%2 == 0)
          myCreatures[i].cre->move(cur.x, cur.y + 1);
        else
          myCreatures[i].cre->move(cur.x, cur.y - 1);
        cout << "FAILED MOVEMENT" << endl;
        attempt++;
      }
    }
    //DIRECTLY OVER - MOVE UP/DOWN
    //MOVE UP - CURRENT Y LESS THAN CLOSEST Y
    else if( cur.y > cp.y )
    {
      cout << "HEADING UP" << endl;
      if( !myCreatures[i].cre->move(cur.x, cur.y - 1) )
      {
        if(rand()%2 == 0)
          myCreatures[i].cre->move(cur.x + 1, cur.y);
        else
          myCreatures[i].cre->move(cur.x - 1, cur.y);
        cout << "FAILED MOVEMENT" << endl;
        attempt++;
      }
    }
    //MOVE DOWN - CURRENT Y GREATER THAN CLOSEST Y
    else if( cur.y < cp.y )
    {
      cout << "HEADING DOWN" << endl;
      if( !myCreatures[i].cre->move(cur.x, cur.y + 1) )
      {
        if(rand()%2 == 0)
          myCreatures[i].cre->move(cur.x + 1, cur.y);
        else
          myCreatures[i].cre->move(cur.x - 1, cur.y);
        cout << "FAILED MOVEMENT" << endl;
        attempt++;
      }
    }
    //DIRECTLY ON TOP OF TARGET
    else
    {
      cout << "LOL WHUT IS GOING ON IDK MAHN" << endl;
      //LOLWHUT
    }
    //ATTEMPT TO EAT
    cp = closestEnemy( cur.x, cur.y );
    cpDist = dist( cur.x, cur.y, cp.x, cp.y);
    if(cpDist == 1)
    {
      myCreatures[i].cre->eat(cp.x, cp.y);
      return;
    }
  }
  return;
}
void AI::omniact(int i)
{
  cout << "OMNIVORE FOR " << i << endl;
  //Determine target
  Point2D next;

  //CURRENT LOCATION
  Point2D cur;
  cur.x = myCreatures[i].cre->x();
  cur.y = myCreatures[i].cre->y();

  //CLOSEST PLANT
  Point2D cp;
  Point2D ce;
  Point2D tgt;
  float cpDist;
  float ceDist;

  int attempt = 0;
  cout << "STARTING HERB WHILE LOOP" << endl;
  while(myCreatures[i].cre->movementLeft() > 0 && attempt < 100)
  {
    //UPDATE
    cur.x = myCreatures[i].cre->x();
    cur.y = myCreatures[i].cre->y();
    
    cp = closestPlant( cur.x, cur.y );
    ce = closestEnemy( cur.x, cur.y );
    cpDist = dist( cur.x, cur.y, cp.x, cp.y);
    ceDist = dist( cur.x, cur.y, ce.x, ce.y);
    
    //ATTEMPT TO EAT
    if(ceDist == 1)
    {
      cout<<"ATTEMPTING TO EAT CREATURE"<<endl;
      myCreatures[i].cre->eat(ce.x, ce.y);
      return;
    }
    else if(cpDist == 1)
    {
        cout<<"ATTEMPTING TO EAT CREATURE"<<endl;
        myCreatures[i].cre->eat(cp.x, cp.y);
        return;
    }
    else
    {
      if(ceDist < cpDist + 3)
        tgt = ce;
      else
        tgt = cp;
    }

    //MOVE RIGHT - CURRENT X LESS THAN CLOSEST X
    if( cur.x < tgt.x )
    {
      cout << "HEADING RIGHT" << endl;
      if( !myCreatures[i].cre->move(cur.x + 1, cur.y) )
      {
        if(rand()%2 == 0)
          myCreatures[i].cre->move(cur.x, cur.y + 1);
        else
          myCreatures[i].cre->move(cur.x, cur.y - 1);
        cout << "FAILED MOVEMENT" << endl;
        attempt++;
      }
    }
    //MOVE LEFT IF NOT OVER LOCATION
    else if( cur.x > tgt.x )
    {
      cout << "HEADING LEFT" << endl;
      if( !myCreatures[i].cre->move(cur.x - 1, cur.y) )
      {
        if(rand()%2 == 0)
          myCreatures[i].cre->move(cur.x, cur.y + 1);
        else
          myCreatures[i].cre->move(cur.x, cur.y - 1);
        cout << "FAILED MOVEMENT" << endl;
        attempt++;
      }
    }
    //DIRECTLY OVER - MOVE UP/DOWN
    //MOVE UP - CURRENT Y LESS THAN CLOSEST Y
    else if( cur.y > tgt.y )
    {
      cout << "HEADING UP" << endl;
      if( !myCreatures[i].cre->move(cur.x, cur.y - 1) )
      {
        if(rand()%2 == 0)
          myCreatures[i].cre->move(cur.x + 1, cur.y);
        else
          myCreatures[i].cre->move(cur.x - 1, cur.y);
        cout << "FAILED MOVEMENT" << endl;
        attempt++;
      }
    }
    //MOVE DOWN - CURRENT Y GREATER THAN CLOSEST Y
    else if( cur.y < tgt.y )
    {
      cout << "HEADING DOWN" << endl;
      if( !myCreatures[i].cre->move(cur.x, cur.y + 1) )
      {
        if(rand()%2 == 0)
          myCreatures[i].cre->move(cur.x + 1, cur.y);
        else
          myCreatures[i].cre->move(cur.x - 1, cur.y);
        cout << "FAILED MOVEMENT" << endl;
        attempt++;
      }
    }
    //DIRECTLY ON TOP OF TARGET
    else
    {
      cout << "LOL WHUT IS GOING ON IDK MAHN" << endl;
      //LOLWHUT
    }
    
    //UPDATE
    cur.x = myCreatures[i].cre->x();
    cur.y = myCreatures[i].cre->y();
    
    cp = closestPlant( cur.x, cur.y );
    ce = closestEnemy( cur.x, cur.y );
    cpDist = dist( cur.x, cur.y, cp.x, cp.y);
    ceDist = dist( cur.x, cur.y, ce.x, ce.y);
    
    //ATTEMPT TO EAT
    if(ceDist == 1)
    {
      cout<<"ATTEMPTING TO EAT CREATURE"<<endl;
      myCreatures[i].cre->eat(ce.x, ce.y);
      return;
    }
    else if(cpDist == 1)
    {
      cout<<"ATTEMPTING TO EAT PLANT"<<endl;
      myCreatures[i].cre->eat(cp.x, cp.y);
      return;
    }
    else
    {
      if(ceDist < cpDist)
        tgt = ce;
      else
        tgt = cp;
    }
  }
  return;
}

float AI::dist(int x1, int y1, int x2, int y2)
{
  return sqrt( pow((x1-x2),2)+pow((y1-y2),2) );
}

Point2D AI::closestPlant(int x, int y)
{
  
  Point2D pt;
  double closest = 1000000000000;
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
  cout << "CLOSEST PLANT " << pt.x << " " << pt.y << endl;
  return pt;
}

Point2D AI::closestEnemy(int x, int y)
{
  cout << "CLOSEST ENEMY " << x << " " << y << endl;
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