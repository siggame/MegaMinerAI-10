using System;
using System.Runtime.InteropServices;

///The class implementing gameplay logic.
class AI : BaseAI
{
  public override string username()
  {
    return "Shell AI";
  }
  public override string password()
  {
    return "password";
  }

  //This function is called each time it is your turn
  //Return true to end your turn, return false to ask the server for updated information
  public override bool run()
  {
    //Iterate through every creature
    for(int ii=0;ii<creatures.Length;ii++)
    {//if I own the creature
      if (creatures[ii].Owner == playerID())
      { //check if there is a plant to that creature's left
        int plantIn = getPlantAtLocation(creatures[ii].X+1,creatures[ii].Y);
        //if there is no plant to my left, or there is a plant of size 0, and there is no creature to my left
        if ((plantIn == -1 || (plantIn !=1 && plants[plantIn].Size==0)) && getCreatureAtLocation(creatures[ii].X+1,creatures[ii].Y) == -1)
        {//if x is in the range of the map, and y is in the range of the map
          if(0<=creatures[ii].X+1 && creatures[ii].X+1<mapWidth() && 0<=creatures[ii].Y && creatures[ii].Y<mapHeight())
          {//if I have ennough health to move, and have movment left
            if (creatures[ii].CurrentHealth>healthPerMove() && creatures[ii].MovementLeft>0)
            {//move creature to the left by incrementing its x cooridinate, and not changing its y
              creatures[ii].move(creatures[ii].X+1,creatures[ii].Y);
            }
          }
        }      

        //check if there is a plant to my left
        plantIn = getPlantAtLocation(creatures[ii].X+1,creatures[ii].Y);
        //check if there is a creature to my left
        int creatIn = getCreatureAtLocation(creatures[ii].X+1,creatures[ii].Y);
       //if there is a plant to my left, and its size is > 0, and this creature has not already eaten this turn       
       if (plantIn != -1 && plants[plantIn].Size> 0 && creatures[ii].CanEat == 1)
        {//eat the plant, using its x and y
          creatures[ii].eat(plants[plantIn].X,plants[plantIn].Y);
        }
        //else if there is a creature to my left, and it is not my creature and this creature has not eaten yet        
        else if (creatIn!=-1 && creatures[creatIn].Owner!=playerID() && creatures[ii].CanEat == 1)
        {//take a bite out of this creature
          creatures[ii].eat(creatures[creatIn].X,creatures[creatIn].Y);
        }
        //else if there is a creature to my left, and it is my creature, and neither has bred this turn        
        else if (creatIn!=-1 && creatures[creatIn].Owner==playerID() && creatures[ii].CanBreed == 1 && creatures[creatIn].CanBreed == 1)
        {//if both creatures have enough health to breed
          if (creatures[ii].CurrentHealth>healthPerBreed() && creatures[creatIn].CurrentHealth>healthPerBreed())
          {//breed with this creature
            creatures[ii].breed(creatures[creatIn]);
          }
        }
      }
    }
    return true;
  }

  //This function is called once, before your first turn
  public override void init() {}

  //This function is called once, after your last turn
  public override void end() {}
  
  
  public AI(IntPtr c) : base(c)
  {}
}
