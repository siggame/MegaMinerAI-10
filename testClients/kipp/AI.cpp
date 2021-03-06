#include "AI.h"
#include "util.h"

AI::AI(Connection* conn) : BaseAI(conn) {}

const char* AI::username()
{
  return "Shell AI";
}

const char* AI::password()
{
  return "password";
}

//This function is run once, before your first turn.
void AI::init(){}

//This function is called each time it is your turn.
//Return true to end your turn, return false to ask the server for updated information.
bool AI::run()
{
  //Iterate through every creature
  for(int ii = 0; ii < creatures.size(); ii++)
  { 
	//if I own the creature
	if (creatures[ii].owner() == playerID())
	{
		/*
		roles
		0 = herbivore
		1 = carnivore
		2 = breeder
		*/
		int role;

		// if herbivorism > carnivorism and has not much energy
		if (creatures[ii].herbivorism() > creatures[ii].carnivorism())
		{
			if (creatures[ii].energy() < energyPerTurn() * 5)
			{
				// if the creature has enough energy to survive
				role = 2;
				// time to get kinky
			}
			else
			{
				// if the creature doesn't have much energy
				role = 0;
				// must get noms
			}
		}
		else if (creatures[ii].herbivorism() < creatures[ii].carnivorism())
		{
			if (creatures[ii].energy() < energyPerTurn() * 5)
			{
				// if the creature has enough energy to survive
				role = 2;
				// time to get kinky
			}
			else
			{
				// if the creature doesn't have much energy
				role = 1;
				// must get noms
			}
		}

		//if (role == 0)
		{
			int plantLeft = getPlantAtLocation(creatures[ii].x() + 1, creatures[ii].y());
			int plantTop = getPlantAtLocation(creatures[ii].x(), creatures[ii].y() - 1);
			int plantRight = getPlantAtLocation(creatures[ii].x() - 1, creatures[ii].y());
			int plantDown = getPlantAtLocation(creatures[ii].x(), creatures[ii].y() + 1);

			if (plantLeft == 1 && creatures[ii].energy() < (5 * energyPerTurn()) && creatures[ii].canEat() == 1)
			{
				creatures[ii].eat(creatures[ii].x() + 1, creatures[ii].y());
			}
			else if (plantTop == 1 && creatures[ii].energy() < (5 * energyPerTurn()) && creatures[ii].canEat() == 1)
			{
				creatures[ii].eat(creatures[ii].x(), creatures[ii].y() - 1);
			}
			else if (plantRight == 1 && creatures[ii].energy() < (5 * energyPerTurn()) && creatures[ii].canEat() == 1)
			{
				creatures[ii].eat(creatures[ii].x() - 1, creatures[ii].y());
			}
			else if (plantDown == 1 && creatures[ii].energy() < (5 * energyPerTurn()) && creatures[ii].canEat() == 1)
			{
				creatures[ii].eat(creatures[ii].x(), creatures[ii].y() + 1);
			}
			else
			{
				switch((rand() % 3) + 1)
				{
					case 0: // left
						creatures[ii].move(creatures[ii].x() + 2, creatures[ii].y());
						break;
					case 1: // up
						creatures[ii].move(creatures[ii].x(), creatures[ii].y() - 2);
						break;
					case 2: // right
						creatures[ii].move(creatures[ii].x() - 2, creatures[ii].y());
						break;
					case 3: // down
						creatures[ii].move(creatures[ii].x(), creatures[ii].y() + 2);
						break;
				}
			}
		}


		if (creatures[ii].movementLeft() > 2)
		{
			return false;
		}








		/*
	  //check if there is a plant to that creature's left
	  int plantIn = getPlantAtLocation(creatures[ii].x() + 1, creatures[ii].y());

	  //if there is no plant to my left, or there is a plant of size 0, and there is no creature to my left
	  if ((plantIn == -1 || (plantIn != 1 && plants[plantIn].size() == 0)) && getCreatureAtLocation(creatures[ii].x() + 1, creatures[ii].y()) == -1)
	  { 
		//if x is in the range of the map, and y is in the range of the map
		if(0 <= creatures[ii].x() + 1 && creatures[ii].x() + 1<mapWidth() && 0 <= creatures[ii].y() && creatures[ii].y() < mapHeight())
		{ 
			//if I have ennough health to move, and have movment left
			if (creatures[ii].currentHealth() > healthPerMove() && creatures[ii].movementLeft() > 0)
			{ 
				//move creature to the left by incrementing its x cooridinate, and not changing its y
				creatures[ii].move(creatures[ii].x() + 1,creatures[ii].y());
				
			}
		}
	  }      

	  //check if there is a plant to my left
	  plantIn = getPlantAtLocation(creatures[ii].x()+1,creatures[ii].y());*/

		/*
	  //check if there is a creature to my left
	  int creatIn = getCreatureAtLocation(creatures[ii].x()+1,creatures[ii].y());
	  //if there is a plant to my left, and its size is > 0, and this creature has not already eaten this turn
	  if (plantIn != -1 && plants[plantIn].size()>0 && creatures[ii].canEat()==1)
	  {
		//eat the plant, using its x and y
		creatures[ii].eat(plants[plantIn].x(),plants[plantIn].y());
	  }
	  //else if there is a creature to my left, and it is not my creature and this creature has not eaten yet
	  else if (creatIn!=-1 && creatures[creatIn].owner()!=playerID() && creatures[ii].canEat()==1)
	  { //take a bite out of this creature
		creatures[ii].eat(creatures[creatIn].x(),creatures[creatIn].y());
	  }
	  //else if there is a creature to my left, and it is my creature, and neither has bred this turn
	  else if (creatIn!=-1 && creatures[creatIn].owner()==playerID() && creatures[ii].canBreed()==1 && creatures[creatIn].canBreed()==1)
	  { //if both creatures have enough health to breed
		if (creatures[ii].currentHealth()>healthPerBreed() && creatures[creatIn].currentHealth()>healthPerBreed())
		{ //breed with this creature
		  creatures[ii].breed(creatures[creatIn]);
		}
	  }*/
	}
  }
  return true;
}

//This function is run once, after your last turn.
void AI::end(){}
