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
  return true;
}

//This function is run once, after your last turn.
void AI::end(){}

//RUSKI FUNCTIONS
void AI::update()
{
  enemyCreatures.clear();

  for(int i = 0; i < creatures.size(); i++)
  {
    //MY CREATURES
    if( creatures[i].owner() == playerID() )
    {
      bool newCre = true;
      for(int ii = 0; ii < myCreatures.size(); ii++)
      {
        if( creatures[i].id() == myCreatures[i].cre->id() )
          newCre = false;
      }
      if(newCre == false )
        myCreatures.push_back( RuskiCre(&creatures[i]) );
    }
    //ENEMY CREATURES
    else
    {
      enemyCreatures.push_back( &creatures[i] );
    }
  }

  //REMOVE DEAD CREATURES
  for(int i = 0; i < myCreatures.size(); i++)
  {
    bool deadCre = true;
    for(int ii = 0; ii < creatures.size(); ii++)
    {
      if( myCreatures[i].cre->id() == creatures[ii].id() )
        deadCre = false;
    }
    if( deadCre == true )
      myCreatures.erase(myCreatures.begin()+i);
  }
  return;
}

void AI::detType()
{
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