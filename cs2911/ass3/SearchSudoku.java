import java.util.LinkedList;

/**
 * This class represents a search state, it contains the sudoku
 * class which is represented by the state and also a list of 
 * all previous states from the starting state which lead to this state
 */

public class SearchSudoku {
  private Sudoku sudoku;
  private LinkedList<Sudoku> path;
  
  SearchSudoku(Sudoku sudoku, LinkedList<Sudoku> path) {
    this.sudoku = sudoku;
    this.path = path;
  }
  
  /**
   * Returns the sudoku class represented by this state
   * @return the sudoku class represented by this state
   */

  public Sudoku getSudoku() {
    return sudoku;
  }
  
  /**
   * Returns the path leading up to the current state
   * @return a list containing all the sudoku classes leading
   * up to the current state from the start state
   */

  public LinkedList<Sudoku> getPath() {
    return path;
  }
}
