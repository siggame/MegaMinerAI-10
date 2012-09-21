using System;
using System.Runtime.InteropServices;


///A basic plant!
public class Plant
{
  public IntPtr ptr;
  protected int ID;
  protected int iteration;

  public Plant()
  {
  }

  public Plant(IntPtr p)
  {
    ptr = p;
    ID = Client.plantGetId(ptr);
    iteration = BaseAI.iteration;
  }

  public bool validify()
  {
    if(iteration == BaseAI.iteration) return true;
    for(int i = 0; i < BaseAI.plants.Length; i++)
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
    int value = Client.plantGetId(ptr);
    return value;
  }
  ///X position of the plant
  public int getX()
  {
    validify();
    int value = Client.plantGetX(ptr);
    return value;
  }
  ///Y position of the plant
  public int getY()
  {
    validify();
    int value = Client.plantGetY(ptr);
    return value;
  }
  ///The size of the plant
  public int getSize()
  {
    validify();
    int value = Client.plantGetSize(ptr);
    return value;
  }
  ///The age of the plant
  public int getAge()
  {
    validify();
    int value = Client.plantGetAge(ptr);
    return value;
  }

}
