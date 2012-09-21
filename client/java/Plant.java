import com.sun.jna.Pointer;

///A basic plant!
class Plant
{
  Pointer ptr;
  int ID;
  int iteration;
  public Plant(Pointer p)
  {
    ptr = p;
    ID = Client.INSTANCE.plantGetId(ptr);
    iteration = BaseAI.iteration;
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
  ///X position of the plant
  public int getX()
  {
    validify();
    return Client.INSTANCE.plantGetX(ptr);
  }
  ///Y position of the plant
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
  ///The age of the plant
  public int getAge()
  {
    validify();
    return Client.INSTANCE.plantGetAge(ptr);
  }

}
