// -*-c++-*-

#ifndef PLANT_H
#define PLANT_H

#include <iostream>
#include "vc_structures.h"


namespace client
{


///A basic plant!
class Plant {
  public:
  void* ptr;
  Plant(_Plant* ptr = NULL);

  // Accessors
  ///Unique Identifier
  int id();
  ///X position of the plant
  int x();
  ///Y position of the plant
  int y();
  ///The size of the plant
  int size();
  ///The age of the plant
  int age();

  // Actions

  // Properties


  friend std::ostream& operator<<(std::ostream& stream, Plant ob);
};

}

#endif

