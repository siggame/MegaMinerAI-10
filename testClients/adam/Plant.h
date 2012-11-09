// -*-c++-*-

#ifndef PLANT_H
#define PLANT_H

#include <iostream>
#include "structures.h"

#include "Mappable.h"

///A basic plant!
class Plant : public Mappable {
  public:
  Plant(_Plant* ptr = NULL);

  // Accessors
  ///Unique Identifier
  int id();
  ///X position of the object
  int x();
  ///Y position of the object
  int y();
  ///The size of the plant
  int size();
  ///The number of turns it takes this plant to grow in size.
  int growthRate();
  ///The number of turns left until this plant will grow again.
  int turnsUntilGrowth();

  // Actions

  // Properties


  friend std::ostream& operator<<(std::ostream& stream, Plant ob);
};

#endif

