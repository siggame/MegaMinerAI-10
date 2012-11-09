#include "AI.h"
#include "util.h"

float dist(const VECTOR2D& a, const VECTOR2D& b)
{
  return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
}

template< class T >
T Clamp(const T& val,const T& a, const T& b)
{
	return val < a ? a : (val > b ? b : val);
}

AI::AI(Connection* conn) : BaseAI(conn), m_currentList(24) {}

const char* AI::username()
{
  return "Mehring";
}

const char* AI::password()
{
  return "DEADC0DE";
}

//This function is run once, before your first turn.
void AI::init(){}

Tile& AI::TileAt(const VECTOR2D& pos)
{
	return m_array[pos.x + pos.y * mapWidth()];
}

void AI::BuildMapArray()
{
	m_array.clear();
	m_array.resize(mapWidth() * mapHeight());

	for(int i=0;i<creatures.size();i++)
	{
		TileAt(VECTOR2D(creatures[i].x(),creatures[i].y())) = Tile(&creatures[i]);
	}

	for(int i=0;i<plants.size();i++)
	{
		TileAt(VECTOR2D(plants[i].x(),plants[i].y())) = Tile(&plants[i]);
	}
}

void AI::UpdateAI()
{

  // todo: state based AI

  //Iterate through every creature
  for(int j=0;j<creatures.size();j++)
  {
	  if (creatures[j].owner() == playerID())
	  {
	      VECTOR2D pos(creatures[j].x(),creatures[j].y());

	      // find near plant to creature, this could be optimized, by searching around the creature pos in the 2d array
	      VECTOR2D plantPos(-1,-1);
	      NearObject(plants,VECTOR2D(creatures[j].x(),creatures[j].y()),plantPos);

		  // If A plant has been found
		  if(plantPos.x != -1)
		  {
			  // find a path
			  std::vector<VECTOR2D> path;
			  unsigned int uiSearchedNodes = FindPath(pos,plantPos,HeuristicManhattanDistance(),creatures[j].movementLeft(),path);

			  cout<<"Nodes proccessed: "<<uiSearchedNodes<<endl;

			  // todo: maybe cache the path, but if we do that, next turn this path may no longer be the best
			  // todo: fix moving more than the creature can

			  // Move
			  for(auto iter = path.rbegin(); iter != path.rend(); ++iter)
			  {
				  Tile& tile = TileAt(*iter);

				  // Eat enemy creatures in the way, or plants
				  if(tile.pObj != nullptr)
				  {
					  Mappable::MappableType type = tile.pObj->GetType();
					  if(type == Mappable::Plant || (type == Mappable::Creature && ((Creature*)tile.pObj)->owner() != playerID()))
					  {
						  creatures[j].eat(iter->x,iter->y);
					  }
				  }

				  creatures[j].move(iter->x,iter->y);
			  }
		  }
	  }
  }
}

//This function is called each time it is your turn.
//Return true to end your turn, return false to ask the server for updated information.
bool AI::run()
{
    BuildMapArray();
    UpdateAI();
    return true;
}

//This function is run once, after your last turn.
void AI::end(){}

/*//check if there is a plant to that creature's left
      int plantIn = getPlantAtLocation(creatures[j].x()+1,creatures[j].y());
      //if there is no plant to my left, or there is a plant of size 0, and there is no creature to my left
      if ((plantIn == -1 || (plantIn !=1 && plants[plantIn].size()==0)) && getCreatureAtLocation(creatures[j].x()+1,creatures[j].y()) == -1)
      { //if x is in the range of the map, and y is in the range of the map
        if(0<=creatures[j].x()+1 && creatures[j].x()+1<mapWidth() && 0<=creatures[j].y() && creatures[j].y()<mapHeight())
        { //if I have ennough health to move, and have movment left
          if (creatures[j].currentHealth()>healthPerMove() && creatures[j].movementLeft()>0)
	 			  { //move creature to the left by incrementing its x cooridinate, and not changing its y
	    creatures[j].move(creatures[j].x()+1,creatures[j].y());
          }
			    }
      }

      //check if there is a plant to my left
      plantIn = getPlantAtLocation(creatures[j].x()+1,creatures[j].y());
      //check if there is a creature to my left
      int creatIn = getCreatureAtLocation(creatures[j].x()+1,creatures[j].y());
      //if there is a plant to my left, and its size is > 0, and this creature has not already eaten this turn
      if (plantIn != -1 && plants[plantIn].size()>0 && creatures[j].canEat()==1)
      {
        //eat the plant, using its x and y
        creatures[j].eat(plants[plantIn].x(),plants[plantIn].y());
      }
      //else if there is a creature to my left, and it is not my creature and this creature has not eaten yet
      else if (creatIn!=-1 && creatures[creatIn].owner()!=playerID() && creatures[j].canEat()==1)
      { //take a bite out of this creature
        creatures[j].eat(creatures[creatIn].x(),creatures[creatIn].y());
      }
      //else if there is a creature to my left, and it is my creature, and neither has bred this turn
      else if (creatIn!=-1 && creatures[creatIn].owner()==playerID() && creatures[j].canBreed()==1 && creatures[creatIn].canBreed()==1)
      { //if both creatures have enough health to breed
        if (creatures[j].currentHealth()>healthPerBreed() && creatures[creatIn].currentHealth()>healthPerBreed())
        { //breed with this creature
          creatures[j].breed(creatures[creatIn]);
        }
      }
    }
  }*/
