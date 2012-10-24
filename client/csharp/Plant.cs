using System;
using System.Runtime.InteropServices;


///A basic plant!
public class Plant: Mappable
{

  public Plant()
  {
  }

  public Plant(IntPtr p)
  {
    ptr = p;
    ID = Client.plantGetId(ptr);
    iteration = BaseAI.iteration;
  }

  public override bool validify()
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
  public new int Id
  {
    get
    {
      validify();
      int value = Client.plantGetId(ptr);
      return value;
    }
  }

  ///X position of the object
  public new int X
  {
    get
    {
      validify();
      int value = Client.plantGetX(ptr);
      return value;
    }
  }

  ///Y position of the object
  public new int Y
  {
    get
    {
      validify();
      int value = Client.plantGetY(ptr);
      return value;
    }
  }

  ///The size of the plant
  public int Size
  {
    get
    {
      validify();
      int value = Client.plantGetSize(ptr);
      return value;
    }
  }

  ///The number of turns it takes this plant to grow in size.
  public int GrowthRate
  {
    get
    {
      validify();
      int value = Client.plantGetGrowthRate(ptr);
      return value;
    }
  }

  ///The number of turns left until this plant will grow again.
  public int TurnsUntilGrowth
  {
    get
    {
      validify();
      int value = Client.plantGetTurnsUntilGrowth(ptr);
      return value;
    }
  }

}

