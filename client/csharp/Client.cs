using System;
using System.Runtime.InteropServices;

public class Client {
  [DllImport("client", CallingConvention = CallingConvention.Cdecl)]
  public static extern IntPtr createConnection();
  [DllImport("client", CallingConvention = CallingConvention.Cdecl)]
  public static extern int serverConnect(IntPtr connection, string host, string port);

  [DllImport("client", CallingConvention = CallingConvention.Cdecl)]
  public static extern int serverLogin(IntPtr connection, string username, string password);
  [DllImport("client", CallingConvention = CallingConvention.Cdecl)]
  public static extern int createGame(IntPtr connection);
  [DllImport("client", CallingConvention = CallingConvention.Cdecl)]
  public static extern int joinGame(IntPtr connection, int id, string playerType);

  [DllImport("client", CallingConvention = CallingConvention.Cdecl)]
  public static extern void endTurn(IntPtr connection);
  [DllImport("client", CallingConvention = CallingConvention.Cdecl)]
  public static extern void getStatus(IntPtr connection);

  [DllImport("client", CallingConvention = CallingConvention.Cdecl)]
  public static extern int networkLoop(IntPtr connection);


    //commands
  [DllImport("client", CallingConvention = CallingConvention.Cdecl)]
  public static extern int creatureMove(IntPtr self, int x, int y);
  [DllImport("client", CallingConvention = CallingConvention.Cdecl)]
  public static extern int creatureEat(IntPtr self, int x, int y);
  [DllImport("client", CallingConvention = CallingConvention.Cdecl)]
  public static extern int creatureBreed(IntPtr self, IntPtr mate);
  [DllImport("client", CallingConvention = CallingConvention.Cdecl)]
  public static extern int playerTalk(IntPtr self, string message);

    //accessors
  [DllImport("client", CallingConvention = CallingConvention.Cdecl)]
  public static extern int getTurnNumber(IntPtr connection);
  [DllImport("client", CallingConvention = CallingConvention.Cdecl)]
  public static extern int getPlayerID(IntPtr connection);
  [DllImport("client", CallingConvention = CallingConvention.Cdecl)]
  public static extern int getGameNumber(IntPtr connection);
  [DllImport("client", CallingConvention = CallingConvention.Cdecl)]
  public static extern int getMapWidth(IntPtr connection);
  [DllImport("client", CallingConvention = CallingConvention.Cdecl)]
  public static extern int getMapHeight(IntPtr connection);
  [DllImport("client", CallingConvention = CallingConvention.Cdecl)]
  public static extern int getHealthPerBreed(IntPtr connection);
  [DllImport("client", CallingConvention = CallingConvention.Cdecl)]
  public static extern int getHealthPerMove(IntPtr connection);
  [DllImport("client", CallingConvention = CallingConvention.Cdecl)]
  public static extern int getHealthPerTurn(IntPtr connection);
  [DllImport("client", CallingConvention = CallingConvention.Cdecl)]
  public static extern int getBaseHealth(IntPtr connection);

  [DllImport("client", CallingConvention = CallingConvention.Cdecl)]
  public static extern IntPtr getMappable(IntPtr connection, int num);
  [DllImport("client", CallingConvention = CallingConvention.Cdecl)]
  public static extern int getMappableCount(IntPtr connection);
  [DllImport("client", CallingConvention = CallingConvention.Cdecl)]
  public static extern IntPtr getCreature(IntPtr connection, int num);
  [DllImport("client", CallingConvention = CallingConvention.Cdecl)]
  public static extern int getCreatureCount(IntPtr connection);
  [DllImport("client", CallingConvention = CallingConvention.Cdecl)]
  public static extern IntPtr getPlant(IntPtr connection, int num);
  [DllImport("client", CallingConvention = CallingConvention.Cdecl)]
  public static extern int getPlantCount(IntPtr connection);
  [DllImport("client", CallingConvention = CallingConvention.Cdecl)]
  public static extern IntPtr getPlayer(IntPtr connection, int num);
  [DllImport("client", CallingConvention = CallingConvention.Cdecl)]
  public static extern int getPlayerCount(IntPtr connection);


    //getters
  [DllImport("client", CallingConvention = CallingConvention.Cdecl)]
  public static extern int mappableGetId(IntPtr ptr);
  [DllImport("client", CallingConvention = CallingConvention.Cdecl)]
  public static extern int mappableGetX(IntPtr ptr);
  [DllImport("client", CallingConvention = CallingConvention.Cdecl)]
  public static extern int mappableGetY(IntPtr ptr);

  [DllImport("client", CallingConvention = CallingConvention.Cdecl)]
  public static extern int creatureGetId(IntPtr ptr);
  [DllImport("client", CallingConvention = CallingConvention.Cdecl)]
  public static extern int creatureGetX(IntPtr ptr);
  [DllImport("client", CallingConvention = CallingConvention.Cdecl)]
  public static extern int creatureGetY(IntPtr ptr);
  [DllImport("client", CallingConvention = CallingConvention.Cdecl)]
  public static extern int creatureGetOwner(IntPtr ptr);
  [DllImport("client", CallingConvention = CallingConvention.Cdecl)]
  public static extern int creatureGetMaxHelth(IntPtr ptr);
  [DllImport("client", CallingConvention = CallingConvention.Cdecl)]
  public static extern int creatureGetCurrentHealth(IntPtr ptr);
  [DllImport("client", CallingConvention = CallingConvention.Cdecl)]
  public static extern int creatureGetEnergy(IntPtr ptr);
  [DllImport("client", CallingConvention = CallingConvention.Cdecl)]
  public static extern int creatureGetCarnivorism(IntPtr ptr);
  [DllImport("client", CallingConvention = CallingConvention.Cdecl)]
  public static extern int creatureGetHerbivorism(IntPtr ptr);
  [DllImport("client", CallingConvention = CallingConvention.Cdecl)]
  public static extern int creatureGetSpeed(IntPtr ptr);
  [DllImport("client", CallingConvention = CallingConvention.Cdecl)]
  public static extern int creatureGetMovementLeft(IntPtr ptr);
  [DllImport("client", CallingConvention = CallingConvention.Cdecl)]
  public static extern int creatureGetDefense(IntPtr ptr);
  [DllImport("client", CallingConvention = CallingConvention.Cdecl)]
  public static extern int creatureGetCanEat(IntPtr ptr);
  [DllImport("client", CallingConvention = CallingConvention.Cdecl)]
  public static extern int creatureGetCanBreed(IntPtr ptr);
  [DllImport("client", CallingConvention = CallingConvention.Cdecl)]
  public static extern int creatureGetParentID(IntPtr ptr);

  [DllImport("client", CallingConvention = CallingConvention.Cdecl)]
  public static extern int plantGetId(IntPtr ptr);
  [DllImport("client", CallingConvention = CallingConvention.Cdecl)]
  public static extern int plantGetX(IntPtr ptr);
  [DllImport("client", CallingConvention = CallingConvention.Cdecl)]
  public static extern int plantGetY(IntPtr ptr);
  [DllImport("client", CallingConvention = CallingConvention.Cdecl)]
  public static extern int plantGetSize(IntPtr ptr);
  [DllImport("client", CallingConvention = CallingConvention.Cdecl)]
  public static extern int plantGetGrowthRate(IntPtr ptr);
  [DllImport("client", CallingConvention = CallingConvention.Cdecl)]
  public static extern int plantGetTurnsUntilGrowth(IntPtr ptr);

  [DllImport("client", CallingConvention = CallingConvention.Cdecl)]
  public static extern int playerGetId(IntPtr ptr);
  [DllImport("client", CallingConvention = CallingConvention.Cdecl)]
  public static extern IntPtr playerGetPlayerName(IntPtr ptr);
  [DllImport("client", CallingConvention = CallingConvention.Cdecl)]
  public static extern float playerGetTime(IntPtr ptr);


    //properties

}
