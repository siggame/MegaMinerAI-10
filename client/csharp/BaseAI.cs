using System;
using System.Runtime.InteropServices;

/// \brief A basic AI interface.

///This class implements most the code an AI would need to interface with the lower-level game code.
///AIs should extend this class to get a lot of builer-plate code out of the way
///The provided AI class does just that.
public abstract class BaseAI
{
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
  ///X and Y size of the map
  public int mapSize()
  {
    int value = Client.getMapSize(connection);
    return value;
  }
}
