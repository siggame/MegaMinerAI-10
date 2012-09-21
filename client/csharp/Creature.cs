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
  ///Command a creature to eat a specified plant
  public int eat(Plant plant)
  {
    validify();
    plant.validify();
    return Client.creatureEat(ptr, plant.ptr);
  }
  ///Breed with target adjacent creature. Spawn new creature at input location
  public int breed(Creature mate, int x, int y)
  {
    validify();
    mate.validify();
    return Client.creatureBreed(ptr, mate.ptr, x, y);
  }
  ///Attack and try to eat target creature
  public int eat(Creature target)
  {
    validify();
    target.validify();
    return Client.creatureEat(ptr, target.ptr);
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
  ///The health of the creature
  public int getHealth()
  {
    validify();
    int value = Client.creatureGetHealth(ptr);
    return value;
  }
  ///The current hunger of the creature
  public int getHunger()
  {
    validify();
    int value = Client.creatureGetHunger(ptr);
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
  ///The spped of the creature
  public int getSpeed()
  {
    validify();
    int value = Client.creatureGetSpeed(ptr);
    return value;
  }
  ///The stamina of the creature
  public int getStamina()
  {
    validify();
    int value = Client.creatureGetStamina(ptr);
    return value;
  }
  ///The defense of the creature
  public int getDefense()
  {
    validify();
    int value = Client.creatureGetDefense(ptr);
    return value;
  }
  ///The age of the creature
  public int getAge()
  {
    validify();
    int value = Client.creatureGetAge(ptr);
    return value;
  }

}
