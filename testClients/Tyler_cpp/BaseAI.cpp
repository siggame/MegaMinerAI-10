//Copyright (C) 2009 - Missouri S&T ACM AI Team
//Please do not modify this file while building your AI
//See AI.h & AI.cpp for that

#include "BaseAI.h"
#include "game.h"

int BaseAI::turnNumber()
{
  return getTurnNumber(c);
}
int BaseAI::playerID()
{
  return getPlayerID(c);
}
int BaseAI::gameNumber()
{
  return getGameNumber(c);
}
int BaseAI::mapWidth()
{
  return getMapWidth(c);
}
int BaseAI::mapHeight()
{
  return getMapHeight(c);
}
int BaseAI::healthPerBreed()
{
  return getHealthPerBreed(c);
}
int BaseAI::healthPerMove()
{
  return getHealthPerMove(c);
}
int BaseAI::healthPerTurn()
{
  return getHealthPerTurn(c);
}
int BaseAI::baseHealth()
{
  return getBaseHealth(c);
}

int BaseAI::getPlantAtLocation(int x, int y)
{
  for(int ii=0; ii<plants.size();ii++)
  {
    if (plants[ii].x() == x && plants[ii].y() == y)
    {		
    	return ii;
    }
  }
  return -1;
}

int BaseAI::getCreatureAtLocation(int x, int y)
{
  for(int ii=0; ii<creatures.size();ii++)
  {
    if (creatures[ii].x() == x && creatures[ii].y() == y)
    {
      return ii;
    }
  }
  return -1;
}

bool BaseAI::startTurn()
{
  static bool initialized = false;
  int count = 0;
  count = getMappableCount(c);
  mappables.clear();
  mappables.resize(count);
  for(int i = 0; i < count; i++)
  {
    mappables[i] = Mappable(getMappable(c, i));
  }

  count = getCreatureCount(c);
  creatures.clear();
  creatures.resize(count);
  for(int i = 0; i < count; i++)
  {
    creatures[i] = Creature(getCreature(c, i));
  }

  count = getPlantCount(c);
  plants.clear();
  plants.resize(count);
  for(int i = 0; i < count; i++)
  {
    plants[i] = Plant(getPlant(c, i));
  }

  count = getPlayerCount(c);
  players.clear();
  players.resize(count);
  for(int i = 0; i < count; i++)
  {
    players[i] = Player(getPlayer(c, i));
  }

  if(!initialized)
  {
    initialized = true;
    init();
  }
  return run();
}

BaseAI::BaseAI(Connection* conn) : c(conn) {}
BaseAI::~BaseAI() {}
