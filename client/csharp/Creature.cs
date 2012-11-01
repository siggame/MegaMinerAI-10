using System;
using System.Runtime.InteropServices;


///A basic creature!
public class Creature: Mappable
{

  public Creature()
  {
  }

  public Creature(IntPtr p)
  {
    ptr = p;
    ID = Client.creatureGetId(ptr);
    iteration = BaseAI.iteration;
  }

  public override bool validify()
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
  public new int Id
  {
    get
    {
      validify();
      int value = Client.creatureGetId(ptr);
      return value;
    }
  }

  ///X position of the object
  public new int X
  {
    get
    {
      validify();
      int value = Client.creatureGetX(ptr);
      return value;
    }
  }

  ///Y position of the object
  public new int Y
  {
    get
    {
      validify();
      int value = Client.creatureGetY(ptr);
      return value;
    }
  }

  ///The owner of the creature
  public int Owner
  {
    get
    {
      validify();
      int value = Client.creatureGetOwner(ptr);
      return value;
    }
  }

  ///The maximum amount of health this creature can have
  public int MaxHelth
  {
    get
    {
      validify();
      int value = Client.creatureGetMaxHelth(ptr);
      return value;
    }
  }

  ///The current amount of health that this creature has.
  public int CurrentHealth
  {
    get
    {
      validify();
      int value = Client.creatureGetCurrentHealth(ptr);
      return value;
    }
  }

  ///The energy level of the creature. This helps your max health
  public int Energy
  {
    get
    {
      validify();
      int value = Client.creatureGetEnergy(ptr);
      return value;
    }
  }

  ///The carnivore level of the creature
  public int Carnivorism
  {
    get
    {
      validify();
      int value = Client.creatureGetCarnivorism(ptr);
      return value;
    }
  }

  ///The herbivore level of the creature
  public int Herbivorism
  {
    get
    {
      validify();
      int value = Client.creatureGetHerbivorism(ptr);
      return value;
    }
  }

  ///The speed of the creature
  public int Speed
  {
    get
    {
      validify();
      int value = Client.creatureGetSpeed(ptr);
      return value;
    }
  }

  ///The amount of moves this creature has left this turn
  public int MovementLeft
  {
    get
    {
      validify();
      int value = Client.creatureGetMovementLeft(ptr);
      return value;
    }
  }

  ///The defense of the creature
  public int Defense
  {
    get
    {
      validify();
      int value = Client.creatureGetDefense(ptr);
      return value;
    }
  }

  ///Indicated whether or not this creature can eat this turn.
  public int CanEat
  {
    get
    {
      validify();
      int value = Client.creatureGetCanEat(ptr);
      return value;
    }
  }

  ///Indicated whether or not this creature can breed this turn.
  public int CanBreed
  {
    get
    {
      validify();
      int value = Client.creatureGetCanBreed(ptr);
      return value;
    }
  }

  ///ID of the creature that gave birth to this one.
  public int ParentID
  {
    get
    {
      validify();
      int value = Client.creatureGetParentID(ptr);
      return value;
    }
  }

}

