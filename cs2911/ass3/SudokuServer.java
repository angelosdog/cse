import java.net.ServerSocket;
import java.io.IOException;
import java.util.LinkedList;

/**
 * The server class. It is a dedicated thread to listen for 
 * connections. Once it accepts a connection, it will
 * spawn a new Worker thread to handle that client.
 *
 */

public class SudokuServer implements Runnable {
  private Game game;
  private boolean updated;
  private LinkedList<Worker> workers;
  private ServerSocket server;
  private boolean running = true;

  /**
   * Constructor. Tries to listen on specified port
   * @param port
   * @param difficulty
   */

  public SudokuServer(int port, int difficulty) {
    game = new Game(difficulty);
    updated = false;
    workers = new LinkedList<Worker>();

    try {
      server = new ServerSocket(port);
    }
    catch (IOException e) {
      System.out.println("Could not listen on port " + port);
    }
  }

  /**
   * Runs a dedicated thread to listen for incoming connections
   * while the server is running. Start a new worked thread for each
   * client
   */

  public void run() {
    UpdateClients clientUpdater = new UpdateClients(workers, this);
    new Thread(clientUpdater).start();
    while (running) {
      Worker w;
      try {
        w = new Worker(server.accept(), game, this);
        synchronized(w) {
          workers.add(w);
        }
        Thread t = new Thread(w);
        t.start();
      }
      catch (IOException e) {
        System.out.println("Could not connect to client");
      }
    }
  }

  /**
   * Stops the server
   */
  
  public void stop() {
    running = false;
    try {
      server.close();
    }
    catch (IOException e) {
      System.out.println("Could not close the server socket");
    }
  }

  public boolean isRunning() {
    return running;
  }

  public boolean isUpdated() {
    return updated;
  }

  public void setUpdated(boolean b) {
    updated = b;
  }
}
