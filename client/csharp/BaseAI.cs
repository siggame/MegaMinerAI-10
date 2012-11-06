using System;
using System.Runtime.InteropServices;

/// \brief A basic AI interface.

///This class implements most the code an AI would need to interface with the lower-level game code.
///AIs should extend this class to get a lot of builer-plate code out of the way
///The provided AI class does just that.
public abstract class BaseAI
{
  public static Mappable[] mappables;
  public static Creature[] creatures;
  public static Plant[] plants;
  public static Player[] players;
  IntPtr connection;
  public static int iteration;
  bool initialized;

  public BaseAI(IntPtr c)
  {
    connection = c;
  }

  ///
  ///Make this your username, which should be provided.
  public abstract String username();
  ///
  ///Make this your password, which should be provided.
  public abstract String password();
  ///
  ///This is run on turn 1 before run
  public abstract void init();
  ///
  ///This is run every turn . Return true to end the turn, return false
  ///to request a status update from the server and then immediately rerun this function with the
  ///latest game status.
  public abstract bool run();

  ///
  ///This is run on after your last turn.
  public abstract void end();


  public bool startTurn()
  {
    iteration++;
    int count = 0;
    count = Client.getMappableCount(connection);
    mappables = new Mappable[count];
    for(int i = 0; i < count; i++)
    {
      mappables[i] = new Mappable(Client.getMappable(connection, i));
    }
    count = Client.getCreatureCount(connection);
    creatures = new Creature[count];
    for(int i = 0; i < count; i++)
    {
      creatures[i] = new Creature(Client.getCreature(connection, i));
    }
    count = Client.getPlantCount(connection);
    plants = new Plant[count];
    for(int i = 0; i < count; i++)
    {
      plants[i] = new Plant(Client.getPlant(connection, i));
    }
    count = Client.getPlayerCount(connection);
    players = new Player[count];
    for(int i = 0; i < count; i++)
    {
      players[i] = new Player(Client.getPlayer(connection, i));
    }

    if(!initialized)
    {
      initialized = true;
      init();
    }
    return run();
  }


  ///How many turns it has been since the beginning of the round
  public int turnNumber()
  {
    int value = Client.getTurnNumber(connection);
    return value;
  }
  ///Player Number; either 0 or 1
  public int playerID()
  {
    int value = Client.getPlayerID(connection);
    return value;
  }
  ///What number game this is for the server
  public int gameNumber()
  {
    int value = Client.getGameNumber(connection);
    return value;
  }
  ///The width of the map
  public int mapWidth()
  {
    int value = Client.getMapWidth(connection);
    return value;
  }
  ///The height of the map
  public int mapHeight()
  {
    int value = Client.getMapHeight(connection);
    return value;
  }
  ///The amount of energy required from each creature in order to breed.
  public int healthPerBreed()
  {
    int value = Client.getHealthPerBreed(connection);
    return value;
  }
  ///The amount of energy required to try to move.
  public int healthPerMove()
  {
    int value = Client.getHealthPerMove(connection);
    return value;
  }
  ///The amount of energy lost from the passage of time.
  public int healthPerTurn()
  {
    int value = Client.getHealthPerTurn(connection);
    return value;
  }
  ///The base amount of health that each creature starts with
  public int baseHealth()
  {
    int value = Client.getBaseHealth(connection);
    return value;
  }
  
  public int getCreatureAtLocation(int x, int y)
  {
    for(int ii=0;ii<creatures.Length;ii++)
    {
      if (creatures[ii].X==x && creatures[ii].Y==y)
      {
        return ii;
      }
    }
    return -1;
  }
  
  public int getPlantAtLocation(int x, int y)
  {
    for(int ii=0;ii<plants.Length;ii++)
    {
      if (plants[ii].X==x && plants[ii].Y==y)
      {
        return ii;
      }
    }
    return -1;
  }
}
