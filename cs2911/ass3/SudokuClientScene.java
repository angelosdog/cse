import java.net.Socket;
import java.io.PrintWriter;
import java.io.IOException;
import java.net.UnknownHostException;

/**
 * The scene where the users play a networked game.
 * This class extends SudokuScene to allow networked play.
 * It tries to connect to a server and then runs a new thread
 * to update the board.
 */

public class SudokuClientScene extends SudokuScene {
  private static final long serialVersionUID = 5316389169155937946L;
  private Socket socket;

  /**
   * Constructor. Tries to connect to specified server
   * @param addr
   * @param port
   */

  public SudokuClientScene(String addr, int port) {
    super();
    newSudoku(6);
    board = new Board(g);

    try {
      socket = new Socket(addr, port);
    } catch (UnknownHostException e) {
      System.out.println("Could not establish connection");
      this.dispose();
    } catch (IOException e) {
      System.out.println("Could not initialise reader or writer");
      this.dispose();
    }
  }

  /**
   * Overrides perform move to send the move to the server
   * @param num
   * @param x
   * @param y
   */

  public void performMove(int num, int x, int y) {
    if (socket == null)
      return;
    PrintWriter out = null;
    try {
      out = new PrintWriter(socket.getOutputStream(), true);
    } catch (UnknownHostException e) {
      System.out.println("Could not establish connection");
      this.dispose();
    } catch (IOException e) {
      System.out.println("Could not initialise reader or writer");
      this.dispose();
    }

    String s = "PerformMove " + x + " " + y + " " + num;
    out.println(s);
  }

  /**
   * Runs the Scene then starts a new thread to update the board
   */

  public void run() {
    super.run();
    UpdateBoard ub = new UpdateBoard(socket, this);
    new Thread(ub).start();
  }
  
}
