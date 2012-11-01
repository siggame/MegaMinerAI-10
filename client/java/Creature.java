import com.sun.jna.Pointer;

///A basic creature!
class Creature extends Mappable
{
  public Creature(Pointer p)
  {
    super(p);
  }
  boolean validify()
  {
    if(iteration == BaseAI.iteration) return true;
    for(int i = 0; i < BaseAI.creatures.length; i++)
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
  int move(int x, int y)
  {
    validify();
    return Client.INSTANCE.creatureMove(ptr, x, y);
  }
  ///Eat plant or creature at input location
  int eat(int x, int y)
  {
    validify();
    return Client.INSTANCE.creatureEat(ptr, x, y);
  }
  ///Breed with target adjacent creature. Spawn new creature at input location
  int breed(Creature mate)
  {
    validify();
    mate.validify();
    return Client.INSTANCE.creatureBreed(ptr, mate.ptr);
  }

    //getters

  ///Unique Identifier
  public int getId()
  {
    validify();
    return Client.INSTANCE.creatureGetId(ptr);
  }
  ///X position of the object
  public int getX()
  {
    validify();
    return Client.INSTANCE.creatureGetX(ptr);
  }
  ///Y position of the object
  public int getY()
  {
    validify();
    return Client.INSTANCE.creatureGetY(ptr);
  }
  ///The owner of the creature
  public int getOwner()
  {
    validify();
    return Client.INSTANCE.creatureGetOwner(ptr);
  }
  ///The maximum amount of health this creature can have
  public int getMaxHelth()
  {
    validify();
    return Client.INSTANCE.creatureGetMaxHelth(ptr);
  }
  ///The current amount of health that this creature has.
  public int getCurrentHealth()
  {
    validify();
    return Client.INSTANCE.creatureGetCurrentHealth(ptr);
  }
  ///The energy level of the creature. This helps your max health
  public int getEnergy()
  {
    validify();
    return Client.INSTANCE.creatureGetEnergy(ptr);
  }
  ///The carnivore level of the creature
  public int getCarnivorism()
  {
    validify();
    return Client.INSTANCE.creatureGetCarnivorism(ptr);
  }
  ///The herbivore level of the creature
  public int getHerbivorism()
  {
    validify();
    return Client.INSTANCE.creatureGetHerbivorism(ptr);
  }
  ///The speed of the creature
  public int getSpeed()
  {
    validify();
    return Client.INSTANCE.creatureGetSpeed(ptr);
  }
  ///The amount of moves this creature has left this turn
  public int getMovementLeft()
  {
    validify();
    return Client.INSTANCE.creatureGetMovementLeft(ptr);
  }
  ///The defense of the creature
  public int getDefense()
  {
    validify();
    return Client.INSTANCE.creatureGetDefense(ptr);
  }
  ///Indicated whether or not this creature can eat this turn.
  public int getCanEat()
  {
    validify();
    return Client.INSTANCE.creatureGetCanEat(ptr);
  }
  ///Indicated whether or not this creature can breed this turn.
  public int getCanBreed()
  {
    validify();
    return Client.INSTANCE.creatureGetCanBreed(ptr);
  }
  ///ID of the creature that gave birth to this one.
  public int getParentID()
  {
    validify();
    return Client.INSTANCE.creatureGetParentID(ptr);
  }

}
