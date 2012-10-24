// -*-c++-*-

#include "Plant.h"
#include "game.h"


Plant::Plant(_Plant* pointer)
{
    ptr = (void*) pointer;
}

int Plant::id()
{
  return ((_Plant*)ptr)->id;
}

int Plant::x()
{
  return ((_Plant*)ptr)->x;
}

int Plant::y()
{
  return ((_Plant*)ptr)->y;
}

int Plant::size()
{
  return ((_Plant*)ptr)->size;
}

int Plant::growthRate()
{
  return ((_Plant*)ptr)->growthRate;
}

int Plant::turnsUntilGrowth()
{
  return ((_Plant*)ptr)->turnsUntilGrowth;
}




std::ostream& operator<<(std::ostream& stream,Plant ob)
{
  stream << "id: " << ((_Plant*)ob.ptr)->id  <<'\n';
  stream << "x: " << ((_Plant*)ob.ptr)->x  <<'\n';
  stream << "y: " << ((_Plant*)ob.ptr)->y  <<'\n';
  stream << "size: " << ((_Plant*)ob.ptr)->size  <<'\n';
  stream << "growthRate: " << ((_Plant*)ob.ptr)->growthRate  <<'\n';
  stream << "turnsUntilGrowth: " << ((_Plant*)ob.ptr)->turnsUntilGrowth  <<'\n';
  return stream;
}
