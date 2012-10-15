// -*-c++-*-

#ifndef CREATURE_H
#define CREATURE_H

#include <iostream>
#include "structures.h"

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
  ///The maximum amount of energy this creature can have
  int maxEnergy();
  ///The current amount of energy this creature has.
  int energyLeft();
  ///The carnivore level of the creature
  int carnivorism();
  ///The herbivore level of the creature
  int herbivorism();
  ///The speed of the creature
  int speed();
  ///The amount of moves this creature has left this turn
  int movementLeft();
  ///The defense of the creature
  int defense();
  ///Indicated whether or not this creature can attack this turn.
  int canAttack();
  ///Indicated whether or not this creature can breed this turn.
  int canBreed();
  ///ID of the creature that gave birth to this one.
  int parentID();

  // Actions
  ///Command a creature to move to a specified position
  int move(int x, int y);
  ///Eat plant or creature at input location
  int eat(int x, int y);
  ///Breed with target adjacent creature. Spawn new creature at input location
  int breed(Creature& mate);

  // Properties


  friend std::ostream& operator<<(std::ostream& stream, Creature ob);
};

#endif

