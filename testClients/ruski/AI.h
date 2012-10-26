#ifndef AI_H
#define AI_H

#include "BaseAI.h"
#include "Creature.h"
#include <vector>
#include <cmath>

struct Point2D
{
  int x;
  int y;
};

///The class implementing gameplay logic.
class AI: public BaseAI
{
public:
  AI(Connection* c);
  virtual const char* username();
  virtual const char* password();
  virtual void init();
  virtual bool run();
  virtual void end();
  
  Point2D findValid(Point2D loc, Point2D death);
  
  float distCC(Creature& c1, Creature& c2);
  float distCP(Creature& c1, Plant& p1);
  float distPP(Point2D& p1, Point2D& p2);
  
  
  std::vector<Creature *> myCreatures;
  std::vector<Creature *> enemyCreatures;

};

#endif
