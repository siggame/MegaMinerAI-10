//Russley Shaw 11/4/2012
#ifndef RUSKI_CREATURE
#define RUSKI_CREATURE

#include "Creature.h"
#include <string>

class RuskiCre
{
public:
  int targetX;
  int targetY;

  std::string type; //herbivore, omnivore, carnivore

  Creature * cre;

  RuskiCre(Creature* c):cre(c){}

};

#endif