//Copyright (C) 2009 - Missouri S&T ACM AI Team
//Please do not modify this file while building your AI
//See AI.h & AI.cpp for that
#ifndef GAME_H
#define GAME_H

#include "network.h"
#include "structures.h"

#ifdef _WIN32
#define DLLEXPORT extern "C" __declspec(dllexport)

#ifdef ENABLE_THREADS
#include "pthread.h"
#endif

#else
#define DLLEXPORT

#ifdef ENABLE_THREADS
#include <pthread.h>
#endif

#endif

struct Connection
{
  int socket;
  
  #ifdef ENABLE_THREADS
  pthread_mutex_t mutex;
  #endif
  
  int turnNumber;
  int playerID;
  int gameNumber;
  int mapWidth;
  int mapHeight;
  int healthPerBreed;
  int healthPerMove;
  int healthPerTurn;
  int baseHealth;

  _Mappable* Mappables;
  int MappableCount;
  _Creature* Creatures;
  int CreatureCount;
  _Plant* Plants;
  int PlantCount;
  _Player* Players;
  int PlayerCount;
};

#ifdef __cplusplus
extern "C"
{
#endif
  DLLEXPORT Connection* createConnection();
  DLLEXPORT void destroyConnection(Connection* c);
  DLLEXPORT int serverConnect(Connection* c, const char* host, const char* port);

  DLLEXPORT int serverLogin(Connection* c, const char* username, const char* password);
  DLLEXPORT int createGame(Connection* c);
  DLLEXPORT int joinGame(Connection* c, int id, const char* playerType);

  DLLEXPORT void endTurn(Connection* c);
  DLLEXPORT void getStatus(Connection* c);


//commands

  ///Command a creature to move to a specified position
  DLLEXPORT int creatureMove(_Creature* object, int x, int y);
  ///Eat plant or creature at input location
  DLLEXPORT int creatureEat(_Creature* object, int x, int y);
  ///Breed with target adjacent creature. Spawn new creature at input location
  DLLEXPORT int creatureBreed(_Creature* object, _Creature* mate);
  ///Allows a player to display messages on the screen
  DLLEXPORT int playerTalk(_Player* object, char* message);

//derived properties



//accessors

DLLEXPORT int getTurnNumber(Connection* c);
DLLEXPORT int getPlayerID(Connection* c);
DLLEXPORT int getGameNumber(Connection* c);
DLLEXPORT int getMapWidth(Connection* c);
DLLEXPORT int getMapHeight(Connection* c);
DLLEXPORT int getHealthPerBreed(Connection* c);
DLLEXPORT int getHealthPerMove(Connection* c);
DLLEXPORT int getHealthPerTurn(Connection* c);
DLLEXPORT int getBaseHealth(Connection* c);

DLLEXPORT _Mappable* getMappable(Connection* c, int num);
DLLEXPORT int getMappableCount(Connection* c);

DLLEXPORT _Creature* getCreature(Connection* c, int num);
DLLEXPORT int getCreatureCount(Connection* c);

DLLEXPORT _Plant* getPlant(Connection* c, int num);
DLLEXPORT int getPlantCount(Connection* c);

DLLEXPORT _Player* getPlayer(Connection* c, int num);
DLLEXPORT int getPlayerCount(Connection* c);



  DLLEXPORT int networkLoop(Connection* c);
#ifdef __cplusplus
}
#endif

#endif
