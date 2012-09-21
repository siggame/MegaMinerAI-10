import com.sun.jna.Pointer;

///An available ship type
class ShipType
{
  Pointer ptr;
  int ID;
  int iteration;
  public ShipType(Pointer p)
  {
    ptr = p;
    ID = Client.INSTANCE.shipTypeGetId(ptr);
    iteration = BaseAI.iteration;
  }
  boolean validify()
  {
    if(iteration == BaseAI.iteration) return true;
    for(int i = 0; i < BaseAI.shipTypes.length; i++)
    {
      if(BaseAI.shipTypes[i].ID == ID)
      {
        ptr = BaseAI.shipTypes[i].ptr;
        iteration = BaseAI.iteration;
        return true;
      }
    }
    throw new ExistentialError();
  }

    //commands

  ///Sends in a new ship of this type. Ships must be warped in with the radius of the player's warp ship.
  int warpIn(int x, int y)
  {
    validify();
    return Client.INSTANCE.shipTypeWarpIn(ptr, x, y);
  }

    //getters

  ///Unique Identifier
  public int getId()
  {
    validify();
    return Client.INSTANCE.shipTypeGetId(ptr);
  }
  ///The ship type
  public String getType()
  {
    validify();
    return Client.INSTANCE.shipTypeGetType(ptr);
  }
  ///The amount of money required to purchase this type of ship
  public int getCost()
  {
    validify();
    return Client.INSTANCE.shipTypeGetCost(ptr);
  }

}
