import java.util.LinkedList;

/**
 * A class to push board updates to all clients
 * Dedicated thread to check if the server board has been updated
 * and if so, push the new board to all clients
 */

public class UpdateClients implements Runnable {
  private LinkedList<Worker> workers;
  private SudokuServer server;

  public UpdateClients(LinkedList<Worker> workers, SudokuServer server) {
    this.workers = workers;
    this.server = server;
  }

  /**
   * Check if the server board has been updated and if so write
   * the new board to each client
   */

  public void run() {
    while (server.isRunning()) {
      try {
        Thread.sleep(100);
      }
      catch (InterruptedException e) {

      }
      if (server.isUpdated()) {
        synchronized(workers) {
          for (Worker w : workers) {
            w.writeGame();
          }
        }
        server.setUpdated(false);
      }
    }
  }

}
