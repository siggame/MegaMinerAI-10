using System;
using System.Runtime.InteropServices;

public class Client {
  [DllImport("client")]
  public static extern IntPtr createConnection();
  [DllImport("client")]
  public static extern int serverConnect(IntPtr connection, string host, string port);

  [DllImport("client")]
  public static extern int serverLogin(IntPtr connection, string username, string password);
  [DllImport("client")]
  public static extern int createGame(IntPtr connection);
  [DllImport("client")]
  public static extern int joinGame(IntPtr connection, int id, string playerType);

  [DllImport("client")]
  public static extern void endTurn(IntPtr connection);
  [DllImport("client")]
  public static extern void getStatus(IntPtr connection);

  [DllImport("client")]
  public static extern int networkLoop(IntPtr connection);


    //commands
  [DllImport("client")]
  public static extern int creatureMove(IntPtr self, int x, int y);
  [DllImport("client")]
  public static extern int creatureEat(IntPtr self, IntPtr plant);
  [DllImport("client")]
  public static extern int creatureBreed(IntPtr self, IntPtr mate, int x, int y);
  [DllImport("client")]
  public static extern int creatureEat(IntPtr self, IntPtr target);
  [DllImport("client")]
  public static extern int playerTalk(IntPtr self, string message);

    //accessors
  [DllImport("client")]
  public static extern int getTurnNumber(IntPtr connection);
  [DllImport("client")]
  public static extern int getPlayerID(IntPtr connection);
  [DllImport("client")]
  public static extern int getMapSize(IntPtr connection);

  [DllImport("client")]
  public static extern IntPtr getCreature(IntPtr connection, int num);
  [DllImport("client")]
  public static extern int getCreatureCount(IntPtr connection);
  [DllImport("client")]
  public static extern IntPtr getPlant(IntPtr connection, int num);
  [DllImport("client")]
  public static extern int getPlantCount(IntPtr connection);
  [DllImport("client")]
  public static extern IntPtr getPlayer(IntPtr connection, int num);
  [DllImport("client")]
  public static extern int getPlayerCount(IntPtr connection);


    //getters
  [DllImport("client")]
  public static extern int creatureGetId(IntPtr ptr);
  [DllImport("client")]
  public static extern int creatureGetOwner(IntPtr ptr);
  [DllImport("client")]
  public static extern int creatureGetX(IntPtr ptr);
  [DllImport("client")]
  public static extern int creatureGetY(IntPtr ptr);
  [DllImport("client")]
  public static extern int creatureGetHealth(IntPtr ptr);
  [DllImport("client")]
  public static extern int creatureGetHunger(IntPtr ptr);
  [DllImport("client")]
  public static extern int creatureGetCarnivorism(IntPtr ptr);
  [DllImport("client")]
  public static extern int creatureGetHerbivorism(IntPtr ptr);
  [DllImport("client")]
  public static extern int creatureGetSpeed(IntPtr ptr);
  [DllImport("client")]
  public static extern int creatureGetMaxStamina(IntPtr ptr);
  [DllImport("client")]
  public static extern int creatureGetDefense(IntPtr ptr);
  [DllImport("client")]
  public static extern int creatureGetAge(IntPtr ptr);

  [DllImport("client")]
  public static extern int plantGetId(IntPtr ptr);
  [DllImport("client")]
  public static extern int plantGetX(IntPtr ptr);
  [DllImport("client")]
  public static extern int plantGetY(IntPtr ptr);
  [DllImport("client")]
  public static extern int plantGetSize(IntPtr ptr);
  [DllImport("client")]
  public static extern int plantGetAge(IntPtr ptr);

  [DllImport("client")]
  public static extern int playerGetId(IntPtr ptr);
  [DllImport("client")]
  public static extern IntPtr playerGetPlayerName(IntPtr ptr);
  [DllImport("client")]
  public static extern float playerGetTime(IntPtr ptr);


    //properties

}
