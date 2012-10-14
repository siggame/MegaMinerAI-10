using System;
using System.Runtime.InteropServices;


///A basic creature!
public class Creature
{
  public IntPtr ptr;
  protected int ID;
  protected int iteration;

  public Creature()
  {
  }

  public Creature(IntPtr p)
  {
    ptr = p;
    ID = Client.creatureGetId(ptr);
    iteration = BaseAI.iteration;
  }

  public bool validify()
  {
    if(iteration == BaseAI.iteration) return true;
    for(int i = 0; i < BaseAI.creatures.Length; i++)
    {
      if(BaseAI.creatures[i].ID == ID)
      {
        ptr = BaseAI.creatures[i].ptr;
        iteration = BaseAI.iteration;
        return true;
      }
    }
    throw new ExistentialError();
  }

    //commands

  ///Command a creature to move to a specified position
  public int move(int x, int y)
  {
    validify();
    return Client.creatureMove(ptr, x, y);
  }
  ///Eat plant or creature at input location
  public int eat(int x, int y)
  {
    validify();
    return Client.creatureEat(ptr, x, y);
  }
  ///Breed with target adjacent creature. Spawn new creature at input location
  public int breed(Creature mate)
  {
    validify();
    mate.validify();
    return Client.creatureBreed(ptr, mate.ptr);
  }

    //getters

  ///Unique Identifier
  public int getId()
  {
    validify();
    int value = Client.creatureGetId(ptr);
    return value;
  }
  ///The owner of the creature
  public int getOwner()
  {
    validify();
    int value = Client.creatureGetOwner(ptr);
    return value;
  }
  ///X position of the creature
  public int getX()
  {
    validify();
    int value = Client.creatureGetX(ptr);
    return value;
  }
  ///Y position of the creature
  public int getY()
  {
    validify();
    int value = Client.creatureGetY(ptr);
    return value;
  }
  ///The maximum amount of energy this creature can have
  public int getMaxEnergy()
  {
    validify();
    int value = Client.creatureGetMaxEnergy(ptr);
    return value;
  }
  ///The current amount of energy this creature has.
  public int getEnergyLeft()
  {
    validify();
    int value = Client.creatureGetEnergyLeft(ptr);
    return value;
  }
  ///The carnivore level of the creature
  public int getCarnivorism()
  {
    validify();
    int value = Client.creatureGetCarnivorism(ptr);
    return value;
  }
  ///The herbivore level of the creature
  public int getHerbivorism()
  {
    validify();
    int value = Client.creatureGetHerbivorism(ptr);
    return value;
  }
  ///The speed of the creature
  public int getSpeed()
  {
    validify();
    int value = Client.creatureGetSpeed(ptr);
    return value;
  }
  ///The amount of moves this creature has left this turn
  public int getMovementLeft()
  {
    validify();
    int value = Client.creatureGetMovementLeft(ptr);
    return value;
  }
  ///The defense of the creature
  public int getDefense()
  {
    validify();
    int value = Client.creatureGetDefense(ptr);
    return value;
  }
  ///Indicated whether or not this creature can attack this turn.
  public int getCanAttack()
  {
    validify();
    int value = Client.creatureGetCanAttack(ptr);
    return value;
  }
  ///Indicated whether or not this creature can breed this turn.
  public int getCanBreed()
  {
    validify();
    int value = Client.creatureGetCanBreed(ptr);
    return value;
  }
  ///ID of the creature that gave birth to this one.
  public int getParentID()
  {
    validify();
    int value = Client.creatureGetParentID(ptr);
    return value;
  }

}
