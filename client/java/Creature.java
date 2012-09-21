import com.sun.jna.Pointer;

///A basic creature!
class Creature
{
  Pointer ptr;
  int ID;
  int iteration;
  public Creature(Pointer p)
  {
    ptr = p;
    ID = Client.INSTANCE.creatureGetId(ptr);
    iteration = BaseAI.iteration;
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
  ///Command a creature to eat a specified plant
  int eat(Plant plant)
  {
    validify();
    plant.validify();
    return Client.INSTANCE.creatureEat(ptr, plant.ptr);
  }
  ///Breed with target adjacent creature. Spawn new creature at input location
  int breed(Creature mate, int x, int y)
  {
    validify();
    mate.validify();
    return Client.INSTANCE.creatureBreed(ptr, mate.ptr, x, y);
  }
  ///Attack and try to eat target creature
  int eat(Creature target)
  {
    validify();
    target.validify();
    return Client.INSTANCE.creatureEat(ptr, target.ptr);
  }

    //getters

  ///Unique Identifier
  public int getId()
  {
    validify();
    return Client.INSTANCE.creatureGetId(ptr);
  }
  ///The owner of the creature
  public int getOwner()
  {
    validify();
    return Client.INSTANCE.creatureGetOwner(ptr);
  }
  ///X position of the creature
  public int getX()
  {
    validify();
    return Client.INSTANCE.creatureGetX(ptr);
  }
  ///Y position of the creature
  public int getY()
  {
    validify();
    return Client.INSTANCE.creatureGetY(ptr);
  }
  ///The health of the creature
  public int getHealth()
  {
    validify();
    return Client.INSTANCE.creatureGetHealth(ptr);
  }
  ///The current hunger of the creature
  public int getHunger()
  {
    validify();
    return Client.INSTANCE.creatureGetHunger(ptr);
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
  ///The max stamina of the creature
  public int getMaxStamina()
  {
    validify();
    return Client.INSTANCE.creatureGetMaxStamina(ptr);
  }
  ///The defense of the creature
  public int getDefense()
  {
    validify();
    return Client.INSTANCE.creatureGetDefense(ptr);
  }
  ///The age of the creature
  public int getAge()
  {
    validify();
    return Client.INSTANCE.creatureGetAge(ptr);
  }

}
