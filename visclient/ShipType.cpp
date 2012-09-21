// -*-c++-*-

#include "ShipType.h"
#include "game.h"


namespace client
{

ShipType::ShipType(_ShipType* pointer)
{
    ptr = (void*) pointer;
}

int ShipType::id()
{
  return ((_ShipType*)ptr)->id;
}

char* ShipType::type()
{
  return ((_ShipType*)ptr)->type;
}

int ShipType::cost()
{
  return ((_ShipType*)ptr)->cost;
}


int ShipType::warpIn(int x, int y)
{
  return shipTypeWarpIn( (_ShipType*)ptr, x, y);
}



std::ostream& operator<<(std::ostream& stream,ShipType ob)
{
  stream << "id: " << ((_ShipType*)ob.ptr)->id  <<'\n';
  stream << "type: " << ((_ShipType*)ob.ptr)->type  <<'\n';
  stream << "cost: " << ((_ShipType*)ob.ptr)->cost  <<'\n';
  return stream;
}

}
