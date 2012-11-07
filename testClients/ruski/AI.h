//Russley Shaw 11/4/2012
#ifndef AI_H
#define AI_H

#include "BaseAI.h"
#include "RuskiCre.h"

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

  void update();
  void detType();
  void herbact(int i);
  void carnact(int i);
  void omniact(int i);

  float dist(int x1, int y1, int x2, int y2);

  Point2D closestPlant(int x, int y);
  Point2D closestEnemy(int x, int y);

  std::vector<RuskiCre> myCreatures;
  std::vector<Creature *> enemyCreatures;

};

#endif
