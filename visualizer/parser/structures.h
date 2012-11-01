//Copyright (C) 2009 - Missouri S&T ACM AI Team
//Please do not modify this file while building your AI
//See AI.h & AI.cpp for that
#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <iostream>
#include <vector>
#include <map>
#include <string>

#include "smartpointer.h"

namespace parser
{

const int MOVE = 0;
const int PLAYERTALK = 1;
const int DEATH = 2;
const int EAT = 3;
const int BREED = 4;

struct Mappable
{
  int id;
  int x;
  int y;

  friend std::ostream& operator<<(std::ostream& stream, Mappable obj);
};

struct Creature: public Mappable 
{
  int owner;
  int maxHelth;
  int currentHealth;
  int energy;
  int carnivorism;
  int herbivorism;
  int speed;
  int movementLeft;
  int defense;
  int canEat;
  int canBreed;
  int parentID;

  friend std::ostream& operator<<(std::ostream& stream, Creature obj);
};

struct Plant: public Mappable 
{
  int size;
  int growthRate;
  int turnsUntilGrowth;

  friend std::ostream& operator<<(std::ostream& stream, Plant obj);
};

struct Player
{
  int id;
  char* playerName;
  float time;

  friend std::ostream& operator<<(std::ostream& stream, Player obj);
};


struct Animation
{
  int type;
};

struct move : public Animation
{
  int actingID;
  int fromX;
  int fromY;
  int toX;
  int toY;

  friend std::ostream& operator<<(std::ostream& stream, move obj);
};

struct playerTalk : public Animation
{
  int actingID;
  char* message;

  friend std::ostream& operator<<(std::ostream& stream, playerTalk obj);
};

struct death : public Animation
{
  int actingID;

  friend std::ostream& operator<<(std::ostream& stream, death obj);
};

struct eat : public Animation
{
  int actingID;
  int targetID;

  friend std::ostream& operator<<(std::ostream& stream, eat obj);
};

struct breed : public Animation
{
  int actingID;
  int targetID;
  int childID;

  friend std::ostream& operator<<(std::ostream& stream, breed obj);
};


struct AnimOwner: public Animation
{
  int owner;
};

struct GameState
{
  std::map<int,Mappable> mappables;
  std::map<int,Creature> creatures;
  std::map<int,Plant> plants;
  std::map<int,Player> players;

  int turnNumber;
  int playerID;
  int gameNumber;
  int mapWidth;
  int mapHeight;
  int healthPerBreed;
  int healthPerMove;
  int healthPerTurn;
  int baseHealth;

  std::map< int, std::vector< SmartPointer< Animation > > > animations;
  friend std::ostream& operator<<(std::ostream& stream, GameState obj);
};

struct Game
{
  std::vector<GameState> states;
  std::string players[2];
  int winner;
	std::string winReason;

  Game();
};

} // parser

#endif
