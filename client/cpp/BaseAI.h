//Copyright (C) 2009 - Missouri S&T ACM AI Team
//Please do not modify this file while building your AI
//See AI.h & AI.cpp for that
#ifndef BASEAI_H
#define BASEAI_H

#include <vector>
#include <ctime>
#include "game.h"

#include "Mappable.h"
#include "Creature.h"
#include "Plant.h"
#include "Player.h"

/// \brief A basic AI interface.

///This class implements most the code an AI would need to interface with the lower-level game code.
///AIs should extend this class to get a lot of boiler-plate code out of the way
///The provided AI class does just that.
class BaseAI
{
protected:
  Connection* c;
  std::vector<Mappable> mappables;
  std::vector<Creature> creatures;
  std::vector<Plant> plants;
  std::vector<Player> players;
public:
  ///How many turns it has been since the beginning of the round
  int turnNumber();
  ///Player Number; either 0 or 1
  int playerID();
  ///What number game this is for the server
  int gameNumber();
  ///The width of the map
  int mapWidth();
  ///The height of the map
  int mapHeight();
  ///The amount of energy required from each creature in order to breed.
  int healthPerBreed();
  ///The amount of energy required to try to move.
  int healthPerMove();
  ///The amount of energy lost from the passage of time.
  int energyPerTurn();
  ///If there is a plant at location (x,y), returns the index of that plant.
  ///If there is no plant, returns -1.
  int getPlantAtLocation(int x,int y);
  ///If there is a creature at location (x,y), returns the index of that creature.
  ///If there is no creature, returns -1.
  int getCreatureAtLocation(int x,int y);
  /// the health a creature loses each turn
  int healthPerTurn();
  ///The base amount of health that each creature starts with
  int baseHealth();

  BaseAI(Connection* c);
  virtual ~BaseAI();
  ///
  ///Make this your username, which should be provided.
  virtual const char* username() = 0;
  ///
  ///Make this your password, which should be provided.
  virtual const char* password() = 0;
  ///
  ///This function is run once, before your first turn.
  virtual void init() = 0;
  ///
  ///This function is called each time it is your turn
  ///Return true to end your turn, return false to ask the server for updated information
  virtual bool run() = 0;
  ///
  ///This function is called after the last turn.
  virtual void end() = 0;


  bool startTurn();
};

#endif
