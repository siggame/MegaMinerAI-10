//Copyright (C) 2009 - Missouri S&T ACM AI Team
//Please do not modify this file while building your AI
//See AI.h & AI.cpp for that
#ifndef STRUCTURES_H
#define STRUCTURES_H

struct Connection;
struct _Creature;
struct _Plant;
struct _Player;


struct _Creature
{
  Connection* _c;
  int id;
  int owner;
  int x;
  int y;
  int health;
  int hunger;
  int carnivorism;
  int herbivorism;
  int speed;
  int stamina;
  int defense;
  int age;
};
struct _Plant
{
  Connection* _c;
  int id;
  int x;
  int y;
  int size;
  int age;
};
struct _Player
{
  Connection* _c;
  int id;
  char* playerName;
  float time;
};

#endif
