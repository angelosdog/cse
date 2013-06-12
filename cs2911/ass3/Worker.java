import java.net.Socket;
import java.io.BufferedReader;
import java.io.PrintWriter;
import java.io.InputStreamReader;
import java.io.IOException;
import java.io.ObjectOutputStream;

/**
 * The class used to service clients.
 * For each client which connects to the server, a new Worker
 * thread is spawned to service this client. The worker listens
 * for incoming actions and actuates these actions on the server game.
 * If successful, the worker will tell the server the game has been updated
 * so the server can push the changes to the clients.
 */

public class Worker implements Runnable {
  private Socket client;
  private Game game;
  private SudokuServer server;

  public Worker(Socket client, Game game, SudokuServer server) {
    this.client = client;
    this.game = game;
    this.server = server;
    writeGame();
  }

  /**
   * Listens for incoming messages from the client.
   * If a message is received, check if the move is valid, and
   * if so, perform the move and tell the server the game has been updated.
   */

  public void run() {
    BufferedReader in = null;
    try {
      in = new BufferedReader(new InputStreamReader(client.getInputStream()));
    }
    catch (IOException e) {
      System.out.println("Could not establish reader or writer");
    }

    while (server.isRunning()) {
      try {
        String[] cmd = in.readLine().split(" ");
        if (cmd[0].equals("PerformMove")) {
          if (game.isValid(Integer.parseInt(cmd[1]),
                      Integer.parseInt(cmd[2]),
                      Integer.parseInt(cmd[3]))) {
            synchronized (game) {
              game.performMove(Integer.parseInt(cmd[1]),
                               Integer.parseInt(cmd[2]),
                               Integer.parseInt(cmd[3]));
            }
            server.setUpdated(true);
          }
        }
        writeGame();
      }
      catch (IOException e) {
        System.out.println("Read failed");
      }
      catch (NullPointerException e) {
        return;
      }
    }
  }

  /**
   * Write the current game to the client.
   */

  public void writeGame() {
    ObjectOutputStream oos = null;
    try {
      oos = new ObjectOutputStream(client.getOutputStream());
      oos.writeObject(game);
    }
    catch (IOException e) {

    }
  }
}
