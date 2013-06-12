import java.io.IOException;
import java.io.Serializable;
import java.util.LinkedList;

/**
 * Sudoku which contains a grid and functionality to manipulate the grid. Also
 * contains further methods pertaining to the game (such as is the game
 * finished).
 * 
 */

public class Sudoku implements Serializable {

  private static final long serialVersionUID = 190219932708126172L;
  private IntegerMatrix board;
  private IntegerMatrix solution = null;

  public static final int WIDTH = 9;
  public static final int HEIGHT = 9;
  public static final int MAX_MOVES = 9;

  public Sudoku() {
    board = new IntegerMatrix(WIDTH, HEIGHT);
  }

  public Sudoku(Sudoku s) {
    this.board = new IntegerMatrix(s.getBoard());
  }

  public Sudoku(int[][] board) {
    this.board = new IntegerMatrix(board);
  }

  public int[][] getBoard() {
    return board.getMatrix();
  }

  public void setBoard(IntegerMatrix im) {
    int[][] boardArray = im.getMatrix();
    for (int i = 0; i < 9; i++) {
      for (int j = 0; j < 9; j++) {
        board.set(i, j, boardArray[i][j]);
      }
    }

  }

  public boolean isComplete() {
    return board.isComplete();
  }

  /**
   * Check if a move is valid.
   * 
   * @param x
   * @param y
   * @param value
   * @return
   */

  public boolean isValid(int x, int y, int value) {
    if (value == 0 && x < WIDTH && y < HEIGHT)
      return true;
    return !board.existsInRow(y, value) && !board.existsInColumn(x, value)
        && !board.existsInSquare(x / 3, y / 3, value);
  }

  public void performMove(int x, int y, int value) {
    board.set(x, y, value);
  }

  public void setSolution(int[][] solution) {
    this.solution = new IntegerMatrix(solution);
  }

  /**
   * Simply used to run JUnit tests.
   * 
   * @return
   */

  public LinkedList<int[]> getEmptyCells() {
    LinkedList<int[]> l = new LinkedList<int[]>();

    for (int i = 0; i < WIDTH; i++) {
      for (int j = 0; j < HEIGHT; j++) {
        if (board.getMatrix()[i][j] == 0) {
          int[] ia = { i, j };
          l.add(ia);
        }
      }
    }

    return l;
  }

  /**
   * Compares the board/grid of each Sudoku.
   */

  @Override
  public boolean equals(Object obj) {
    if (obj == null)
      return false;
    if (getClass() == obj.getClass()) {
      Sudoku other = (Sudoku) obj;
      return other.board.equals(other.board);
    }
    return false;
  }

  public int numsInRow(int row) {
    int counter = 0;
    for (int i = 0; i < WIDTH; i++) {
      if (board.get(i, row) != 0)
        counter++;
    }
    return counter;
  }

  public int numsInColumn(int column) {
    int counter = 0;
    for (int i = 0; i < HEIGHT; i++) {
      if (board.get(column, i) != 0)
        counter++;
    }
    return counter;
  }

  public int numsInSudoku() {
    int counter = 0;
    for (int i = 0; i < HEIGHT * WIDTH; i++) {
      if (board.get(i % 9, i / 9) != 0)
        counter++;
    }
    return counter;
  }

  public int getCell(int x, int y) {
    return board.get(x, y);
  }

  /**
   * Returns a number of a cell of the correct Sudoku solution of a given Sudoku
   * game.
   * 
   * @param x
   * @param y
   * @return
   */

  public int getSolutionCell(int x, int y) {
    if (solution == null) {
      Search s = new Search(this);
      Sudoku solution = s.getSolution().getSudoku();
      setSolution(solution.getBoard());
    }
    return solution.get(x, y);
  }

  /**
   * Get a hint based on the next state of the search which finds the solution
   * given an incomplete board.
   * 
   * @return
   */

  public int[] getHint() {
    Search search = new Search(this);
    SearchSudoku solution = search.getSolution();
    Sudoku nextState;
    if (solution != null) {
      if (solution.getPath().size() > 1) {
        nextState = solution.getPath().get(1);
      } else {
        nextState = solution.getSudoku();
      }
      for (int x = 0; x < 9; x++) {
        for (int y = 0; y < 9; y++) {
          if (this.getCell(x, y) != nextState.getCell(x, y)) {
            int[] ret = {x, y, nextState.getCell(x, y)};
            return ret;
          }
        }
      }
    }
    return null;
  }

  private void writeObject(java.io.ObjectOutputStream out) throws IOException {
    out.defaultWriteObject();
  }

  private void readObject(java.io.ObjectInputStream in) throws IOException,
      ClassNotFoundException {
    in.defaultReadObject();
  }
}
