// -*-c++-*-

#ifndef CREATURE_H
#define CREATURE_H

#include <iostream>
#include "vc_structures.h"


namespace client
{

class Plant;
class Creature;

///A basic creature!
class Creature {
  public:
  void* ptr;
  Creature(_Creature* ptr = NULL);

  // Accessors
  ///Unique Identifier
  int id();
  ///The owner of the creature
  int owner();
  ///X position of the creature
  int x();
  ///Y position of the creature
  int y();
  ///The health of the creature
  int health();
  ///The current hunger of the creature
  int hunger();
  ///The carnivore level of the creature
  int carnivorism();
  ///The herbivore level of the creature
  int herbivorism();
  ///The spped of the creature
  int speed();
  ///The stamina of the creature
  int stamina();
  ///The defense of the creature
  int defense();
  ///The age of the creature
  int age();

  // Actions
  ///Command a creature to move to a specified position
  int move(int x, int y);
  ///Command a creature to eat a specified plant
  int eat(Plant& plant);
  ///Breed with target adjacent creature. Spawn new creature at input location
  int breed(Creature& mate, int x, int y);
  ///Attack and try to eat target creature
  int eat(Creature& target);

  // Properties


  friend std::ostream& operator<<(std::ostream& stream, Creature ob);
};

}

#endif

