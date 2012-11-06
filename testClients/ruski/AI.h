//Russley Shaw 11/4/2012
#ifndef AI_H
#define AI_H

#include "BaseAI.h"
#include "RuskiCre.h"

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

  std::vector<RuskiCre> myCreatures;
  std::vector<Creature *> enemyCreatures;

};

#endif
