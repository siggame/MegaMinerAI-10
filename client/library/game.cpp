//Copyright (C) 2009 - Missouri S&T ACM AI Team
//Please do not modify this file while building your AI
//See AI.h & AI.cpp for that
#pragma warning(disable : 4996)

#include <string>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <memory>

#include "game.h"
#include "network.h"
#include "structures.h"

#include "sexp/sfcompat.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#ifdef _WIN32
//Doh, namespace collision.
namespace Windows
{
    #include <Windows.h>
};
#else
#include <unistd.h>
#endif

#ifdef ENABLE_THREADS
#define LOCK(X) pthread_mutex_lock(X)
#define UNLOCK(X) pthread_mutex_unlock(X)
#else
#define LOCK(X)
#define UNLOCK(X)
#endif

using std::cout;
using std::cerr;
using std::endl;
using std::stringstream;
using std::string;
using std::ofstream;

DLLEXPORT Connection* createConnection()
{
  Connection* c = new Connection;
  c->socket = -1;
  #ifdef ENABLE_THREADS
  pthread_mutex_init(&c->mutex, NULL);
  #endif

  c->turnNumber = 0;
  c->playerID = 0;
  c->gameNumber = 0;
  c->mapWidth = 0;
  c->mapHeight = 0;
  c->healthPerBreed = 0;
  c->healthPerMove = 0;
  c->healthPerTurn = 0;
  c->baseHealth = 0;
  c->Mappables = NULL;
  c->MappableCount = 0;
  c->Creatures = NULL;
  c->CreatureCount = 0;
  c->Plants = NULL;
  c->PlantCount = 0;
  c->Players = NULL;
  c->PlayerCount = 0;
  return c;
}

DLLEXPORT void destroyConnection(Connection* c)
{
  #ifdef ENABLE_THREADS
  pthread_mutex_destroy(&c->mutex);
  #endif
  if(c->Mappables)
  {
    for(int i = 0; i < c->MappableCount; i++)
    {
    }
    delete[] c->Mappables;
  }
  if(c->Creatures)
  {
    for(int i = 0; i < c->CreatureCount; i++)
    {
    }
    delete[] c->Creatures;
  }
  if(c->Plants)
  {
    for(int i = 0; i < c->PlantCount; i++)
    {
    }
    delete[] c->Plants;
  }
  if(c->Players)
  {
    for(int i = 0; i < c->PlayerCount; i++)
    {
      delete[] c->Players[i].playerName;
    }
    delete[] c->Players;
  }
  delete c;
}

DLLEXPORT int serverConnect(Connection* c, const char* host, const char* port)
{
  c->socket = open_server_connection(host, port);
  return c->socket + 1; //false if socket == -1
}

DLLEXPORT int serverLogin(Connection* c, const char* username, const char* password)
{
  string expr = "(login \"";
  expr += username;
  expr += "\" \"";
  expr += password;
  expr +="\")";

  send_string(c->socket, expr.c_str());

  sexp_t* expression, *message;

  char* reply = rec_string(c->socket);
  expression = extract_sexpr(reply);
  delete[] reply;

  message = expression->list;
  if(message->val == NULL || strcmp(message->val, "login-accepted") != 0)
  {
    cerr << "Unable to login to server" << endl;
    destroy_sexp(expression);
    return 0;
  }
  destroy_sexp(expression);
  return 1;
}

DLLEXPORT int createGame(Connection* c)
{
  sexp_t* expression, *number;

  send_string(c->socket, "(create-game)");

  char* reply = rec_string(c->socket);
  expression = extract_sexpr(reply);
  delete[] reply;

  number = expression->list->next;
  c->gameNumber = atoi(number->val);
  destroy_sexp(expression);

  std::cout << "Creating game " << c->gameNumber << endl;

  c->playerID = 0;

  return c->gameNumber;
}

DLLEXPORT int joinGame(Connection* c, int gameNum, const char* playerType)
{
  sexp_t* expression;
  stringstream expr;

  c->gameNumber = gameNum;

  expr << "(join-game " << c->gameNumber << " "<< playerType << ")";
  send_string(c->socket, expr.str().c_str());

  char* reply = rec_string(c->socket);
  expression = extract_sexpr(reply);
  delete[] reply;

  if(strcmp(expression->list->val, "join-accepted") == 0)
  {
    destroy_sexp(expression);
    c->playerID = 1;
    send_string(c->socket, "(game-start)");
    return 1;
  }
  else if(strcmp(expression->list->val, "create-game") == 0)
  {
    std::cout << "Game did not exist, creating game " << c->gameNumber << endl;
    destroy_sexp(expression);
    c->playerID = 0;
    return 1;
  }
  else
  {
    cerr << "Cannot join game "<< c->gameNumber << ": " << expression->list->next->val << endl;
    destroy_sexp(expression);
    return 0;
  }
}

DLLEXPORT void endTurn(Connection* c)
{
  LOCK( &c->mutex );
  send_string(c->socket, "(end-turn)");
  UNLOCK( &c->mutex );
}

DLLEXPORT void getStatus(Connection* c)
{
  LOCK( &c->mutex );
  send_string(c->socket, "(game-status)");
  UNLOCK( &c->mutex );
}



DLLEXPORT int creatureMove(_Creature* object, int x, int y)
{
  stringstream expr;
  expr << "(game-move " << object->id
       << " " << x
       << " " << y
       << ")";
  LOCK( &object->_c->mutex);
  send_string(object->_c->socket, expr.str().c_str());
  UNLOCK( &object->_c->mutex);
  
  Connection * c = object->_c;
  //Cannot move a creature you do not own.
  if(object->owner !=c->playerID)
  {
    return 0;
  }
  //Cannot move if there is no movement left.
  else if(object->movementLeft<=0)
  {
   return 0;
  }
  //Cannot move out of bounds
  else if((x<0||x>c->mapWidth)||(y<0||y>c->mapHeight))
  {
    return 0;
  }
  else if (abs(object->x-x) + abs(object->y-y)!=1)
  {
   return 0;
  }
  //Do not move on top of another creature.
  for (int ii=0;ii<c->CreatureCount;ii++)
  {
   if (c->Creatures[ii].x == x && c->Creatures[ii].y == y)
   {
     return 0;
   }
  }
  //Do not move on top of a plant with size >0.
  for(int ii=0;ii<c->PlantCount;ii++)
  {
   if(c->Plants[ii].x == x && c->Plants[ii].y == y)
    {
      if (c->Plants[ii].size>0)
      {
        return 0;
      }  
    }
  }
  
  //Decrement energy and movement
  object->currentHealth = object->currentHealth-c->healthPerMove;
  object->movementLeft = object->movementLeft-1;
  
  //Apply new movement
  object->x = x;
  object->y = y;
  //here is a thing for reasons  
  return 1;
}

DLLEXPORT int creatureEat(_Creature* object, int x, int y)
{
  stringstream expr;
  expr << "(game-eat " << object->id
       << " " << x
       << " " << y
       << ")";
  LOCK( &object->_c->mutex);
  send_string(object->_c->socket, expr.str().c_str());
  UNLOCK( &object->_c->mutex);
  Connection * c = object->_c;
  //Can't control your opponents creatures
  if(object->owner != c->playerID)
  {
    return 0;
  }
  //can't make a dead creature eat
  else if(object->currentHealth<=0)
  {
    return 0;
  }
  //have to be adjacent to target
  else if(abs(object->x-x)+abs(object->y-y)!=1)
  {
    return 0;
  }
  //can only eat once per turn
  else if (!(object->canEat))
  {
    return 0;
  }
  bool looking = true;
  //see what it is you're trying to eat, if anything is there
  if(looking)
  {
    for(int ii=0;ii<c->CreatureCount;ii++)
    {
      if(c->Creatures[ii].x == x && c->Creatures[ii].y == y)
      {
        int damage = object->carnivorism-c->Creatures[ii].defense;
        //damage has to be at least one because of reasons
        if (damage<1)
        {
          damage = 1;
        }
        c->Creatures[ii].currentHealth -=damage;
        //check if you killed it
        if(c->Creatures[ii].currentHealth<=0)
        {
          object->currentHealth+=object->carnivorism*10;
          if(object->currentHealth>object->maxHealth)
          {
            object->currentHealth = object->maxHealth;
          }
        }
        object->canEat=false;
        looking=false;
        return 1;
      }
    }
  }
  if(looking)
  { 
    for(int ii=0;ii<c->PlantCount;ii++)
    {
      if(c->Plants[ii].x == x && c->Plants[ii].y == y)
      {
        //plant has to have size>0 to be eaten
        if(c->Plants[ii].size == 0)
        {
          return 0;
        }
        object->currentHealth+=object->herbivorism*5;
        if(object->currentHealth > object->maxHealth)
        {
          object->currentHealth=object->maxHealth;
        }
        c->Plants[ii].size-=1;
        object->canEat=false;
        looking=false;
        return 1;
      }
    }
  }
  if(looking)
  {
    return 0;
  }
}

DLLEXPORT int creatureBreed(_Creature* object, _Creature* mate)
{
  stringstream expr;
  expr << "(game-breed " << object->id
      << " " << mate->id
       << ")";
  LOCK( &object->_c->mutex);
  send_string(object->_c->socket, expr.str().c_str());
  UNLOCK( &object->_c->mutex);
  Connection * c = object->_c;
  //game state update
  
  //can't make your opponents creatures breed
  if(object->owner!=c->playerID)
  {
    return 0;
  }
  //you don't want to die breeding....probably worse ways to go
  else if(object->currentHealth<=c->healthPerBreed)
  {
   return 0;
  }
  //you don't want to kill your mate with your love
  else if(mate->currentHealth<=c->healthPerBreed)
  {
   return 0;
  }
  //they have to be adjacent 
  else if(abs(object->x-mate->x)+abs(object->y-mate->y)!=1)
  {
   return 0;
  }
  //no fraternizing with the enemy 
  else if(mate->owner!=c->playerID)
  {
   return 0;
  }
  //can only breed once per turn
  else if(!(object->canBreed && mate->canBreed))
  {
   return 0;
  }
  object->canBreed = false;
  mate->canBreed = false;
  object->canEat= false;
  mate->canEat = false;
  object->movementLeft = 0;
  mate->movementLeft = 0;
  object->currentHealth-=c->healthPerBreed;
  mate->currentHealth-=c->healthPerBreed;
  return 1;
}

DLLEXPORT _Plant* getPlantAtLocation(Connection* c, int x, int y)
{
  _Plant *p = NULL;
  for(int ii=0;ii<c->PlantCount;ii++)
  {
    if(c->Plants[ii].x == x && c->Plants[ii].y == y)
    {
      *p = c->Plants[ii];
    }
  }
  return p;
}
 
DLLEXPORT _Creature* getCreatureAtLocation(Connection* c, int x, int y)
{
  _Creature *cr = NULL;
  for(int ii=0;ii<c->CreatureCount;ii++)
  {
    if(c->Creatures[ii].x == x && c->Creatures[ii].y == y)
    {
      *cr = c->Creatures[ii];
    }
  }
  return cr;
}



DLLEXPORT int playerTalk(_Player* object, char* message)
{
  stringstream expr;
  expr << "(game-talk " << object->id
      << " \"" << escape_string(message) << "\""
       << ")";
  LOCK( &object->_c->mutex);
  send_string(object->_c->socket, expr.str().c_str());
  UNLOCK( &object->_c->mutex);
  return 1;
}


//Utility functions for parsing data
void parseMappable(Connection* c, _Mappable* object, sexp_t* expression)
{
  sexp_t* sub;
  sub = expression->list;

  object->_c = c;

  object->id = atoi(sub->val);
  sub = sub->next;
  object->x = atoi(sub->val);
  sub = sub->next;
  object->y = atoi(sub->val);
  sub = sub->next;

}
void parseCreature(Connection* c, _Creature* object, sexp_t* expression)
{
  sexp_t* sub;
  sub = expression->list;

  object->_c = c;

  object->id = atoi(sub->val);
  sub = sub->next;
  object->x = atoi(sub->val);
  sub = sub->next;
  object->y = atoi(sub->val);
  sub = sub->next;
  object->owner = atoi(sub->val);
  sub = sub->next;
  object->maxHealth = atoi(sub->val);
  sub = sub->next;
  object->currentHealth = atoi(sub->val);
  sub = sub->next;
  object->energy = atoi(sub->val);
  sub = sub->next;
  object->carnivorism = atoi(sub->val);
  sub = sub->next;
  object->herbivorism = atoi(sub->val);
  sub = sub->next;
  object->speed = atoi(sub->val);
  sub = sub->next;
  object->movementLeft = atoi(sub->val);
  sub = sub->next;
  object->defense = atoi(sub->val);
  sub = sub->next;
  object->canEat = atoi(sub->val);
  sub = sub->next;
  object->canBreed = atoi(sub->val);
  sub = sub->next;
  object->parentID = atoi(sub->val);
  sub = sub->next;

}
void parsePlant(Connection* c, _Plant* object, sexp_t* expression)
{
  sexp_t* sub;
  sub = expression->list;

  object->_c = c;

  object->id = atoi(sub->val);
  sub = sub->next;
  object->x = atoi(sub->val);
  sub = sub->next;
  object->y = atoi(sub->val);
  sub = sub->next;
  object->size = atoi(sub->val);
  sub = sub->next;
  object->growthRate = atoi(sub->val);
  sub = sub->next;
  object->turnsUntilGrowth = atoi(sub->val);
  sub = sub->next;

}
void parsePlayer(Connection* c, _Player* object, sexp_t* expression)
{
  sexp_t* sub;
  sub = expression->list;

  object->_c = c;

  object->id = atoi(sub->val);
  sub = sub->next;
  object->playerName = new char[strlen(sub->val)+1];
  strncpy(object->playerName, sub->val, strlen(sub->val));
  object->playerName[strlen(sub->val)] = 0;
  sub = sub->next;
  object->time = atof(sub->val);
  sub = sub->next;

}

DLLEXPORT int networkLoop(Connection* c)
{
  while(true)
  {
    sexp_t* base, *expression, *sub, *subsub;

    char* message = rec_string(c->socket);
    string text = message;
    base = extract_sexpr(message);
    delete[] message;
    expression = base->list;
    if(expression->val != NULL && strcmp(expression->val, "game-winner") == 0)
    {
      expression = expression->next->next->next;
      int winnerID = atoi(expression->val);
      if(winnerID == c->playerID)
      {
        cout << "You win!" << endl << expression->next->val << endl;
      }
      else
      {
        cout << "You lose. :(" << endl << expression->next->val << endl;
      }
      stringstream expr;
      expr << "(request-log " << c->gameNumber << ")";
      send_string(c->socket, expr.str().c_str());
      destroy_sexp(base);
      return 0;
    }
    else if(expression->val != NULL && strcmp(expression->val, "log") == 0)
    {
      ofstream out;
      stringstream filename;
      expression = expression->next;
      filename << expression->val;
      filename << ".gamelog";
      expression = expression->next;
      out.open(filename.str().c_str());
      if (out.good())
        out.write(expression->val, strlen(expression->val));
      else
        cerr << "Error : Could not create log." << endl;
      out.close();
      destroy_sexp(base);
      return 0;
    }
    else if(expression->val != NULL && strcmp(expression->val, "game-accepted")==0)
    {
      char gameID[30];

      expression = expression->next;
      strcpy(gameID, expression->val);
      cout << "Created game " << gameID << endl;
    }
    else if(expression->val != NULL && strstr(expression->val, "denied"))
    {
      cout << expression->val << endl;
      cout << expression->next->val << endl;
    }
    else if(expression->val != NULL && strcmp(expression->val, "status") == 0)
    {
      while(expression->next != NULL)
      {
        expression = expression->next;
        sub = expression->list;
        if(string(sub->val) == "game")
        {
          sub = sub->next;
          c->turnNumber = atoi(sub->val);
          sub = sub->next;

          c->playerID = atoi(sub->val);
          sub = sub->next;

          c->gameNumber = atoi(sub->val);
          sub = sub->next;

          c->mapWidth = atoi(sub->val);
          sub = sub->next;

          c->mapHeight = atoi(sub->val);
          sub = sub->next;

          c->healthPerBreed = atoi(sub->val);
          sub = sub->next;

          c->healthPerMove = atoi(sub->val);
          sub = sub->next;

          c->healthPerTurn = atoi(sub->val);
          sub = sub->next;

          c->baseHealth = atoi(sub->val);
          sub = sub->next;

        }
        else if(string(sub->val) == "Mappable")
        {
          if(c->Mappables)
          {
            for(int i = 0; i < c->MappableCount; i++)
            {
            }
            delete[] c->Mappables;
          }
          c->MappableCount =  sexp_list_length(expression)-1; //-1 for the header
          c->Mappables = new _Mappable[c->MappableCount];
          for(int i = 0; i < c->MappableCount; i++)
          {
            sub = sub->next;
            parseMappable(c, c->Mappables+i, sub);
          }
        }
        else if(string(sub->val) == "Creature")
        {
          if(c->Creatures)
          {
            for(int i = 0; i < c->CreatureCount; i++)
            {
            }
            delete[] c->Creatures;
          }
          c->CreatureCount =  sexp_list_length(expression)-1; //-1 for the header
          c->Creatures = new _Creature[c->CreatureCount];
          for(int i = 0; i < c->CreatureCount; i++)
          {
            sub = sub->next;
            parseCreature(c, c->Creatures+i, sub);
          }
        }
        else if(string(sub->val) == "Plant")
        {
          if(c->Plants)
          {
            for(int i = 0; i < c->PlantCount; i++)
            {
            }
            delete[] c->Plants;
          }
          c->PlantCount =  sexp_list_length(expression)-1; //-1 for the header
          c->Plants = new _Plant[c->PlantCount];
          for(int i = 0; i < c->PlantCount; i++)
          {
            sub = sub->next;
            parsePlant(c, c->Plants+i, sub);
          }
        }
        else if(string(sub->val) == "Player")
        {
          if(c->Players)
          {
            for(int i = 0; i < c->PlayerCount; i++)
            {
              delete[] c->Players[i].playerName;
            }
            delete[] c->Players;
          }
          c->PlayerCount =  sexp_list_length(expression)-1; //-1 for the header
          c->Players = new _Player[c->PlayerCount];
          for(int i = 0; i < c->PlayerCount; i++)
          {
            sub = sub->next;
            parsePlayer(c, c->Players+i, sub);
          }
        }
      }
      destroy_sexp(base);
      return 1;
    }
    else
    {
#ifdef SHOW_WARNINGS
      cerr << "Unrecognized message: " << text << endl;
#endif
    }
    destroy_sexp(base);
  }
}

DLLEXPORT _Mappable* getMappable(Connection* c, int num)
{
  return c->Mappables + num;
}
DLLEXPORT int getMappableCount(Connection* c)
{
  return c->MappableCount;
}

DLLEXPORT _Creature* getCreature(Connection* c, int num)
{
  return c->Creatures + num;
}
DLLEXPORT int getCreatureCount(Connection* c)
{
  return c->CreatureCount;
}

DLLEXPORT _Plant* getPlant(Connection* c, int num)
{
  return c->Plants + num;
}
DLLEXPORT int getPlantCount(Connection* c)
{
  return c->PlantCount;
}

DLLEXPORT _Player* getPlayer(Connection* c, int num)
{
  return c->Players + num;
}
DLLEXPORT int getPlayerCount(Connection* c)
{
  return c->PlayerCount;
}


DLLEXPORT int getTurnNumber(Connection* c)
{
  return c->turnNumber;
}
DLLEXPORT int getPlayerID(Connection* c)
{
  return c->playerID;
}
DLLEXPORT int getGameNumber(Connection* c)
{
  return c->gameNumber;
}
DLLEXPORT int getMapWidth(Connection* c)
{
  return c->mapWidth;
}
DLLEXPORT int getMapHeight(Connection* c)
{
  return c->mapHeight;
}
DLLEXPORT int getHealthPerBreed(Connection* c)
{
  return c->healthPerBreed;
}
DLLEXPORT int getHealthPerMove(Connection* c)
{
  return c->healthPerMove;
}
DLLEXPORT int getHealthPerTurn(Connection* c)
{
  return c->healthPerTurn;
}
DLLEXPORT int getBaseHealth(Connection* c)
{
  return c->baseHealth;
}
