// -*-c++-*-

#include "structures.h"

#include <iostream>

namespace parser
{


std::ostream& operator<<(std::ostream& stream, Mappable ob)
{
  stream << "id: " << ob.id  <<'\n';
  stream << "x: " << ob.x  <<'\n';
  stream << "y: " << ob.y  <<'\n';
  return stream;
}


std::ostream& operator<<(std::ostream& stream, Creature ob)
{
  stream << "id: " << ob.id  <<'\n';
  stream << "x: " << ob.x  <<'\n';
  stream << "y: " << ob.y  <<'\n';
  stream << "owner: " << ob.owner  <<'\n';
  stream << "maxHealth: " << ob.maxHealth  <<'\n';
  stream << "currentHealth: " << ob.currentHealth  <<'\n';
  stream << "energy: " << ob.energy  <<'\n';
  stream << "carnivorism: " << ob.carnivorism  <<'\n';
  stream << "herbivorism: " << ob.herbivorism  <<'\n';
  stream << "speed: " << ob.speed  <<'\n';
  stream << "movementLeft: " << ob.movementLeft  <<'\n';
  stream << "defense: " << ob.defense  <<'\n';
  stream << "canEat: " << ob.canEat  <<'\n';
  stream << "canBreed: " << ob.canBreed  <<'\n';
  stream << "parentID: " << ob.parentID  <<'\n';
  return stream;
}


std::ostream& operator<<(std::ostream& stream, Plant ob)
{
  stream << "id: " << ob.id  <<'\n';
  stream << "x: " << ob.x  <<'\n';
  stream << "y: " << ob.y  <<'\n';
  stream << "size: " << ob.size  <<'\n';
  stream << "growthRate: " << ob.growthRate  <<'\n';
  stream << "turnsUntilGrowth: " << ob.turnsUntilGrowth  <<'\n';
  return stream;
}


std::ostream& operator<<(std::ostream& stream, Player ob)
{
  stream << "id: " << ob.id  <<'\n';
  stream << "playerName: " << ob.playerName  <<'\n';
  stream << "time: " << ob.time  <<'\n';
  return stream;
}



std::ostream& operator<<(std::ostream& stream, move ob)
{
  stream << "move" << "\n";
  stream << "actingID: " << ob.actingID  <<'\n';
  stream << "fromX: " << ob.fromX  <<'\n';
  stream << "fromY: " << ob.fromY  <<'\n';
  stream << "toX: " << ob.toX  <<'\n';
  stream << "toY: " << ob.toY  <<'\n';
  return stream;
}


std::ostream& operator<<(std::ostream& stream, playerTalk ob)
{
  stream << "playerTalk" << "\n";
  stream << "actingID: " << ob.actingID  <<'\n';
  stream << "message: " << ob.message  <<'\n';
  return stream;
}


std::ostream& operator<<(std::ostream& stream, death ob)
{
  stream << "death" << "\n";
  stream << "actingID: " << ob.actingID  <<'\n';
  return stream;
}


std::ostream& operator<<(std::ostream& stream, eat ob)
{
  stream << "eat" << "\n";
  stream << "actingID: " << ob.actingID  <<'\n';
  stream << "targetID: " << ob.targetID  <<'\n';
  return stream;
}


std::ostream& operator<<(std::ostream& stream, breed ob)
{
  stream << "breed" << "\n";
  stream << "actingID: " << ob.actingID  <<'\n';
  stream << "targetID: " << ob.targetID  <<'\n';
  stream << "childID: " << ob.childID  <<'\n';
  return stream;
}


std::ostream& operator<<(std::ostream& stream, GameState ob)
{
  stream << "turnNumber: " << ob.turnNumber  <<'\n';
  stream << "playerID: " << ob.playerID  <<'\n';
  stream << "gameNumber: " << ob.gameNumber  <<'\n';
  stream << "mapWidth: " << ob.mapWidth  <<'\n';
  stream << "mapHeight: " << ob.mapHeight  <<'\n';
  stream << "healthPerBreed: " << ob.healthPerBreed  <<'\n';
  stream << "healthPerMove: " << ob.healthPerMove  <<'\n';
  stream << "healthPerTurn: " << ob.healthPerTurn  <<'\n';
  stream << "baseHealth: " << ob.baseHealth  <<'\n';

  stream << "\n\nMappables:\n";
  for(std::map<int,Mappable>::iterator i = ob.mappables.begin(); i != ob.mappables.end(); i++)
    stream << i->second << '\n';
  stream << "\n\nCreatures:\n";
  for(std::map<int,Creature>::iterator i = ob.creatures.begin(); i != ob.creatures.end(); i++)
    stream << i->second << '\n';
  stream << "\n\nPlants:\n";
  for(std::map<int,Plant>::iterator i = ob.plants.begin(); i != ob.plants.end(); i++)
    stream << i->second << '\n';
  stream << "\n\nPlayers:\n";
  for(std::map<int,Player>::iterator i = ob.players.begin(); i != ob.players.end(); i++)
    stream << i->second << '\n';
  stream << "\nAnimation\n";
  for
    (
    std::map< int, std::vector< SmartPointer< Animation > > >::iterator j = ob.animations.begin(); 
    j != ob.animations.end(); 
    j++ 
    )
  {
  for(std::vector< SmartPointer< Animation > >::iterator i = j->second.begin(); i != j->second.end(); i++)
  {
//    if((*(*i)).type == MOVE)
//      stream << *((move*)*i) << "\n";
//    if((*(*i)).type == PLAYERTALK)
//      stream << *((playerTalk*)*i) << "\n";
//    if((*(*i)).type == DEATH)
//      stream << *((death*)*i) << "\n";
//    if((*(*i)).type == EAT)
//      stream << *((eat*)*i) << "\n";
//    if((*(*i)).type == BREED)
//      stream << *((breed*)*i) << "\n";
  }
  

  }
  return stream;
}

Game::Game()
{
  winner = -1;
}

} // parser
