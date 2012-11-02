import com.sun.jna.Pointer;

///The class implementing gameplay logic.
public class AI extends BaseAI
{
  public String username()
  {
    return "Shell AI";
  }
  public String password()
  {
    return "password";
  }

  //This function is called each time it is your turn
  //Return true to end your turn, return false to ask the server for updated information
  public boolean run()
  {
    //Iterate through every creature
    for(int ii=0;ii<creatures.length;ii++)
    {//if I own the creature
      if (creatures[ii].getOwner() == playerID())
      { //check if there is a plant to that creature's left
        int plantIn = getPlantAtLocation(creatures[ii].getX()+1,creatures[ii].getY());
        //if there is no plant to my left, or there is a plant of size 0, and there is no creature to my left
        if ((plantIn == -1 || (plantIn !=1 && plants[plantIn].getSize()==0)) && getCreatureAtLocation(creatures[ii].getX()+1,creatures[ii].getY()) == -1)
        {//if x is in the range of the map, and y is in the range of the map
          if(0<=creatures[ii].getX()+1 && creatures[ii].getX()+1<mapWidth() && 0<=creatures[ii].getY() && creatures[ii].getY()<mapHeight())
          {//if I have ennough health to move, and have movment left
            if (creatures[ii].getCurrentHealth()>healthPerMove() && creatures[ii].getMovementLeft()>0)
            {//move creature to the left by incrementing its x cooridinate, and not changing its y
              creatures[ii].move(creatures[ii].getX()+1,creatures[ii].getY());
            }
          }
        }      

        //check if there is a plant to my left
        plantIn = getPlantAtLocation(creatures[ii].getX()+1,creatures[ii].getY());
        //check if there is a creature to my left
        int creatIn = getCreatureAtLocation(creatures[ii].getX()+1,creatures[ii].getY());
       //if there is a plant to my left, and its size is > 0, and this creature has not already eaten this turn       
       if (plantIn != -1 && plants[plantIn].getSize()>0 && creatures[ii].getCanEat()==1)
        {//eat the plant, using its x and y
          creatures[ii].eat(plants[plantIn].getX(),plants[plantIn].getY());
        }
        //else if there is a creature to my left, and it is not my creature and this creature has not eaten yet        
        else if (creatIn!=-1 && creatures[creatIn].getOwner()!=playerID() && creatures[ii].getCanEat()==1)
        {//take a bite out of this creature
          creatures[ii].eat(creatures[creatIn].getX(),creatures[creatIn].getY());
        }
        //else if there is a creature to my left, and it is my creature, and neither has bred this turn        
        else if (creatIn!=-1 && creatures[creatIn].getOwner()==playerID() && creatures[ii].getCanBreed()==1 && creatures[creatIn].getCanBreed()==1)
        {//if both creatures have enough health to breed
          if (creatures[ii].getCurrentHealth()>healthPerBreed() && creatures[creatIn].getCurrentHealth()>healthPerBreed())
          {//breed with this creature
            creatures[ii].breed(creatures[creatIn]);
          }
        }
      }
    }
    return true;
  }


  //This function is called once, before your first turn
  public void init() {}

  //This function is called once, after your last turn
  public void end() {}
  
  
  public AI(Pointer c)
  {
    super(c);
  }
}
