import java.io.IOException;
import java.io.Serializable;

/**
 * The class which controls a Sudoku game. The front-end will store this class
 * and can manipulate it via the methods provided. This class can be serialized.
 * 
 */

public class Game implements Serializable {
  private static final long serialVersionUID = 59208897689304191L;
  private static final int DEFAULT_DIFFICULTY = 3;

  private Sudoku sudoku;
  private IntegerMatrix initialState = null;
  private int difficulty;
  private int time;

  public Game() {
    this(DEFAULT_DIFFICULTY);
  }

  public Game(int difficulty) {
    this.difficulty = difficulty;
    if (difficulty != 6) {
      Generator g = new Generator();
      sudoku = g.generateBoard(difficulty);
    } else {
      sudoku = new Sudoku();
    }
    initialState = new IntegerMatrix(sudoku.getBoard());
  }

  // Wrappers for sudoku and initialState

  public int getTime() {
    return time;
  }

  public void setTime(int time) {
    this.time = time;
  }

  public IntegerMatrix getInitialBoard() {
    return initialState;
  }

  public int getCell(int col, int row) {
    return sudoku.getCell(col, row);
  }

  public int getSolutionCell(int col, int row) {
    return sudoku.getSolutionCell(col, row);
  }

  public boolean isValid(int col, int row, int value) {
    return sudoku.isValid(col, row, value);
  }

  public void performMove(int col, int row, int value) {
    sudoku.performMove(col, row, value);
  }

  public int getDifficulty() {
    return difficulty;
  }

  public boolean isComplete() {
    return sudoku.isComplete();
  }

  /**
   * Returns a cell with the correct value.
   * @return an array as follows {x, y, value}
   */
  
  public int[] getHint() {
    return sudoku.getHint();
  }

  public void reset() {
    if (initialState != null) {
      sudoku.setBoard(initialState);
    }
  }

  private void writeObject(java.io.ObjectOutputStream out) throws IOException {
    out.defaultWriteObject();
  }

  private void readObject(java.io.ObjectInputStream in) throws IOException,
      ClassNotFoundException {
    in.defaultReadObject();
  }
}
