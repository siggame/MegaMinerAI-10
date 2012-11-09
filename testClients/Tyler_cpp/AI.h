#ifndef AI_H
#define AI_H

#include "BaseAI.h"

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
  bool isValid(int x, int y);
  Creature* findAdjacentEnemy(Creature* & c);
  Creature* findAdjacentFriend(Creature* & c);
  Plant* findAdjacentPlant(Creature* & c);
  void stupidMove(Creature* & c);
  bool goLeft(Creature* & c);
  bool goRight(Creature* & c);
  bool goUp(Creature* & c);
  bool goDown(Creature* & c);
};

#endif
