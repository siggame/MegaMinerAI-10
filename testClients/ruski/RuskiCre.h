//Russley Shaw 11/4/2012
#ifndef RUSKI_CREATURE
#define RUSKI_CREATURE

#include "Creature.h"
#include <string>

struct Point2D
{
  int x;
  int y;
};

class RuskiCre
{
public:
  Point2D tgt;

  std::string type; //herbivore, omnivore, carnivore

  Creature * cre;

  RuskiCre(Creature* c):cre(c){}

};

#endif