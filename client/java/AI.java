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
    for(int ii=0;ii<creatures.length;ii++)
	{
  	  if (creatures[ii].getOwner() == playerID())
      {
        if (getPlantAtLocation(creatures[ii].getX()+1,creatures[ii].getY()) == null && getCreatureAtLocation(creatures[ii].getX()+1,creatures[ii].getY()) == null)
        {
          if(0<=creatures[ii].getX()+1 && creatures[ii].getX()+1<mapWidth() && 0<=creatures[ii].getY() && creatures[ii].getY()<mapHeight())
		  {
            if (creatures[ii].getEnergyLeft()>energyPerAction() && creatures[ii].getMovementLeft()>0)
			{
			  creatures[ii].move(creatures[ii].getX()+1,creatures[ii].getY());
			}
		  }
        }      
		Plant plant = getPlantAtLocation(creatures[ii].getX()+1,creatures[ii].getY());
		Creature creat = getCreatureAtLocation(creatures[ii].getX()+1,creatures[ii].getY());
		if (plant != null && plant.getSize()>0/* && creatures[ii].getCanEat()==0*/)
		{
			creatures[ii].eat(creatures[ii].getX()+1,creatures[ii].getY());
		}
		else if (creat!=null && creat.getOwner()!=playerID() /*&& creatures[ii].getCanEat()!=1*/)
		{
			creatures[ii].eat(creatures[ii].getX()+1,creatures[ii].getY());
		}
		else if (creat!=null && creat.getOwner()==playerID() /*&& creatures[ii].getCanBreed()!=1 && creat.getCanBreed()!=1*/)
		{
			if (creatures[ii].getEnergyLeft()>=energyPerBreed() && creat.getEnergyLeft()>=energyPerBreed())
			{
				creatures[ii].breed(creat);
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
