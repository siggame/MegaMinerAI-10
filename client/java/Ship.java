import com.sun.jna.Pointer;

///A space ship!
class Ship
{
  Pointer ptr;
  int ID;
  int iteration;
  public Ship(Pointer p)
  {
    ptr = p;
    ID = Client.INSTANCE.shipGetId(ptr);
    iteration = BaseAI.iteration;
  }
  boolean validify()
  {
    if(iteration == BaseAI.iteration) return true;
    for(int i = 0; i < BaseAI.ships.length; i++)
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
  int move(int x, int y)
  {
    validify();
    return Client.INSTANCE.shipMove(ptr, x, y);
  }
  ///Blow yourself up, damage those around you, reduces the ship to 0 health.
  int selfDestruct()
  {
    validify();
    return Client.INSTANCE.shipSelfDestruct(ptr);
  }
  ///Commands your ship to attack a target. Making an attack will reduce the number of attacks available to the ship, even if the attack is rejected by the game server.
  int attack(Ship target)
  {
    validify();
    target.validify();
    return Client.INSTANCE.shipAttack(ptr, target.ptr);
  }

    //getters

  ///Unique Identifier
  public int getId()
  {
    validify();
    return Client.INSTANCE.shipGetId(ptr);
  }
  ///The owner of the ship
  public int getOwner()
  {
    validify();
    return Client.INSTANCE.shipGetOwner(ptr);
  }
  ///X position of the ship
  public int getX()
  {
    validify();
    return Client.INSTANCE.shipGetX(ptr);
  }
  ///Y position of the ship
  public int getY()
  {
    validify();
    return Client.INSTANCE.shipGetY(ptr);
  }
  ///The radius of the ship
  public int getRadius()
  {
    validify();
    return Client.INSTANCE.shipGetRadius(ptr);
  }
  ///The ship type
  public String getType()
  {
    validify();
    return Client.INSTANCE.shipGetType(ptr);
  }
  ///How many more attacks this ship has
  public int getAttacksLeft()
  {
    validify();
    return Client.INSTANCE.shipGetAttacksLeft(ptr);
  }
  ///How much more movement this ship has
  public int getMovementLeft()
  {
    validify();
    return Client.INSTANCE.shipGetMovementLeft(ptr);
  }
  ///The largest possible movement for this ship
  public int getMaxMovement()
  {
    validify();
    return Client.INSTANCE.shipGetMaxMovement(ptr);
  }
  ///The max number of attacks for this ship
  public int getMaxAttacks()
  {
    validify();
    return Client.INSTANCE.shipGetMaxAttacks(ptr);
  }
  ///The strength of attacks for this ship
  public int getDamage()
  {
    validify();
    return Client.INSTANCE.shipGetDamage(ptr);
  }
  ///The range of attacks for this ship
  public int getRange()
  {
    validify();
    return Client.INSTANCE.shipGetRange(ptr);
  }
  ///The total health of the ship
  public int getHealth()
  {
    validify();
    return Client.INSTANCE.shipGetHealth(ptr);
  }
  ///The max health possible for the ship
  public int getMaxHealth()
  {
    validify();
    return Client.INSTANCE.shipGetMaxHealth(ptr);
  }
  ///The amount of damage done when this ship self destructs
  public int getSelfDestructDamage()
  {
    validify();
    return Client.INSTANCE.shipGetSelfDestructDamage(ptr);
  }

}
