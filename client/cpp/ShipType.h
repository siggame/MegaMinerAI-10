// -*-c++-*-

#ifndef SHIPTYPE_H
#define SHIPTYPE_H

#include <iostream>
#include "structures.h"


///An available ship type
class ShipType {
  public:
  void* ptr;
  ShipType(_ShipType* ptr = NULL);

  // Accessors
  ///Unique Identifier
  int id();
  ///The ship type
  char* type();
  ///The amount of money required to purchase this type of ship
  int cost();

  // Actions
  ///Sends in a new ship of this type. Ships must be warped in with the radius of the player's warp ship.
  int warpIn(int x, int y);

  // Properties


  friend std::ostream& operator<<(std::ostream& stream, ShipType ob);
};

#endif

