import java.io.ObjectInputStream;
import java.io.IOException;
import java.net.Socket;

/**
 * A class to update the client board.
 * Dedicated thread to listen for new boards from the server
 */

public class UpdateBoard implements Runnable {
  private Socket socket;
  private SudokuClientScene client;
  private boolean initial = true;

  public UpdateBoard(Socket socket, SudokuClientScene client) {
    this.socket = socket;
    this.client = client;
  }

  /**
   * Listen for incoming data to update the board
   */

  public void run() {
    while(true) {
      try {
        ObjectInputStream ois = new ObjectInputStream(socket.getInputStream());
        client.g = (Game) ois.readObject();
        client.board.setGame(client.g);
        if (initial) {
          client.board.setEditable();
          initial = false;
        }
        client.board.repaint();
      } catch (IOException e) {
      } catch (ClassNotFoundException e) {
      }
    }
  }
}
