using System;
using System.Runtime.InteropServices;


///A space ship!
public class Ship
{
  public IntPtr ptr;
  protected int ID;
  protected int iteration;

  public Ship()
  {
  }

  public Ship(IntPtr p)
  {
    ptr = p;
    ID = Client.shipGetId(ptr);
    iteration = BaseAI.iteration;
  }

  public bool validify()
  {
    if(iteration == BaseAI.iteration) return true;
    for(int i = 0; i < BaseAI.ships.Length; i++)
    {
      if(BaseAI.ships[i].ID == ID)
      {
        ptr = BaseAI.ships[i].ptr;
        iteration = BaseAI.iteration;
        return true;
      }
    }
    throw new ExistentialError();
  }

    //commands

  ///Command a ship to move to a specified position. If the position specified by this function is not legal, the position of the ship will be updated, but the movement will be rejected by the server.
  public int move(int x, int y)
  {
    validify();
    return Client.shipMove(ptr, x, y);
  }
  ///Blow yourself up, damage those around you, reduces the ship to 0 health.
  public int selfDestruct()
  {
    validify();
    return Client.shipSelfDestruct(ptr);
  }
  ///Commands your ship to attack a target. Making an attack will reduce the number of attacks available to the ship, even if the attack is rejected by the game server.
  public int attack(Ship target)
  {
    validify();
    target.validify();
    return Client.shipAttack(ptr, target.ptr);
  }

    //getters

  ///Unique Identifier
  public int getId()
  {
    validify();
    int value = Client.shipGetId(ptr);
    return value;
  }
  ///The owner of the ship
  public int getOwner()
  {
    validify();
    int value = Client.shipGetOwner(ptr);
    return value;
  }
  ///X position of the ship
  public int getX()
  {
    validify();
    int value = Client.shipGetX(ptr);
    return value;
  }
  ///Y position of the ship
  public int getY()
  {
    validify();
    int value = Client.shipGetY(ptr);
    return value;
  }
  ///The radius of the ship
  public int getRadius()
  {
    validify();
    int value = Client.shipGetRadius(ptr);
    return value;
  }
  ///The ship type
  public string getType()
  {
    validify();
    IntPtr value = Client.shipGetType(ptr);
    return Marshal.PtrToStringAuto(value);
  }
  ///How many more attacks this ship has
  public int getAttacksLeft()
  {
    validify();
    int value = Client.shipGetAttacksLeft(ptr);
    return value;
  }
  ///How much more movement this ship has
  public int getMovementLeft()
  {
    validify();
    int value = Client.shipGetMovementLeft(ptr);
    return value;
  }
  ///The largest possible movement for this ship
  public int getMaxMovement()
  {
    validify();
    int value = Client.shipGetMaxMovement(ptr);
    return value;
  }
  ///The max number of attacks for this ship
  public int getMaxAttacks()
  {
    validify();
    int value = Client.shipGetMaxAttacks(ptr);
    return value;
  }
  ///The strength of attacks for this ship
  public int getDamage()
  {
    validify();
    int value = Client.shipGetDamage(ptr);
    return value;
  }
  ///The range of attacks for this ship
  public int getRange()
  {
    validify();
    int value = Client.shipGetRange(ptr);
    return value;
  }
  ///The total health of the ship
  public int getHealth()
  {
    validify();
    int value = Client.shipGetHealth(ptr);
    return value;
  }
  ///The max health possible for the ship
  public int getMaxHealth()
  {
    validify();
    int value = Client.shipGetMaxHealth(ptr);
    return value;
  }
  ///The amount of damage done when this ship self destructs
  public int getSelfDestructDamage()
  {
    validify();
    int value = Client.shipGetSelfDestructDamage(ptr);
    return value;
  }

}
