import com.sun.jna.Pointer;

///
class Player
{
  Pointer ptr;
  int ID;
  int iteration;
  public Player(Pointer p)
  {
    ptr = p;
    ID = Client.INSTANCE.playerGetId(ptr);
    iteration = BaseAI.iteration;
  }
  boolean validify()
  {
    if(iteration == BaseAI.iteration) return true;
    for(int i = 0; i < BaseAI.players.length; i++)
    {
      if(BaseAI.players[i].ID == ID)
      {
        ptr = BaseAI.players[i].ptr;
        iteration = BaseAI.iteration;
        return true;
      }
    }
    throw new ExistentialError();
  }

    //commands

  ///Allows a player to display messages on the screen
  int talk(String message)
  {
    validify();
    return Client.INSTANCE.playerTalk(ptr, message);
  }

    //getters

  ///Unique Identifier
  public int getId()
  {
    validify();
    return Client.INSTANCE.playerGetId(ptr);
  }
  ///Player's Name
  public String getPlayerName()
  {
    validify();
    return Client.INSTANCE.playerGetPlayerName(ptr);
  }
  ///Time remaining, updated at start of turn
  public float getTime()
  {
    validify();
    return Client.INSTANCE.playerGetTime(ptr);
  }

}
