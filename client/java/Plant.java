import com.sun.jna.Pointer;

///A basic plant!
class Plant extends Mappable
{
  public Plant(Pointer p)
  {
    super(p);
  }
  boolean validify()
  {
    if(iteration == BaseAI.iteration) return true;
    for(int i = 0; i < BaseAI.plants.length; i++)
    {
      if(BaseAI.plants[i].ID == ID)
      {
        ptr = BaseAI.plants[i].ptr;
        iteration = BaseAI.iteration;
        return true;
      }
    }
    throw new ExistentialError();
  }

    //commands


    //getters

  ///Unique Identifier
  public int getId()
  {
    validify();
    return Client.INSTANCE.plantGetId(ptr);
  }
  ///X position of the object
  public int getX()
  {
    validify();
    return Client.INSTANCE.plantGetX(ptr);
  }
  ///Y position of the object
  public int getY()
  {
    validify();
    return Client.INSTANCE.plantGetY(ptr);
  }
  ///The size of the plant
  public int getSize()
  {
    validify();
    return Client.INSTANCE.plantGetSize(ptr);
  }
  ///The number of turns it takes this plant to grow in size.
  public int getGrowthRate()
  {
    validify();
    return Client.INSTANCE.plantGetGrowthRate(ptr);
  }
  ///The number of turns left until this plant will grow again.
  public int getTurnsUntilGrowth()
  {
    validify();
    return Client.INSTANCE.plantGetTurnsUntilGrowth(ptr);
  }

}
