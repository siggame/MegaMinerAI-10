//Copyright (C) 2009 - Missouri S&T ACM AI Team
//Please do not modify this file while building your AI
//See AI.h & AI.cpp for that
#ifndef STRUCTURES_H
#define STRUCTURES_H

struct Connection;
struct _Mappable;
struct _Creature;
struct _Plant;
struct _Player;


struct _Mappable
{
  Connection* _c;
  int id;
  int x;
  int y;
};
struct _Creature
{
  Connection* _c;
  int id;
  int x;
  int y;
  int owner;
  int maxHealth;
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
};
struct _Plant
{
  Connection* _c;
  int id;
  int x;
  int y;
  int size;
  int growthRate;
  int turnsUntilGrowth;
};
struct _Player
{
  Connection* _c;
  int id;
  char* playerName;
  float time;
};

#endif
