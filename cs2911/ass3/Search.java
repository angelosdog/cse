import java.util.LinkedList;
import java.util.Stack;

/**
 * This is the search class used by solve and hint functionality
 * It searches for a solution to the startState using DFS
 */

public class Search {
  // Stack of states which need to be explored
  private Stack<SearchSudoku> fringe;

  public Search(Sudoku startState) {
    fringe = new Stack<SearchSudoku>();
    fringe.add(new SearchSudoku(startState, new LinkedList<Sudoku>()));
  }

  /**
   * Searches possible states for the goal state
   * @return the goal state
   */

  public SearchSudoku getSolution() {
    while (fringe.size() > 0) {
      SearchSudoku currState = fringe.pop();
      // if the current state is the goal state then we return it
      if (currState.getSudoku().isComplete()) {
        return currState;
      }

      int[][] board = currState.getSudoku().getBoard();
      int x = 0; int y = 0;
      int minx = 0; int miny = 0; int minFree = 10;
      // To minimise the number of states we have to search what we
      // do is we find the cell with the least number of possibilities
      // and we enqueue all of those successor states. This minimises
      // the branching factor at each stage of the search

      // find cell with the smallest number of possibilities
      for (x = 0; x < board.length; x++) {
        for (y = 0; y < board[0].length; y++) {
          if (board[x][y] == 0) {
            int freeRow = 0;
            int freeCol = 0;
            int freeSquare = 0;
            for (int i = 0; i < 9; i++) {
              if (board[x][i] == 0) 
                freeRow++;
            }
            for (int i = 0; i < 9; i++) {
              if (board[i][y] == 0) 
                freeCol++;
            }
            for (int i = x / 3; i < (x / 3) + 3; i++) {
              for (int j = y / 3; j < (y / 3) + 3; j++) {
                if (board[i][j] == 0) 
                  freeSquare++;
              }
            }
            int free = Math.min(Math.min(freeRow, freeCol), freeSquare);
            if (free < minFree) {
              minFree = free;
              minx = x;
              miny = y;
            }
          }
        }
      }

      // enqueue all valid moves for that cell
      for (int i = 1; i < 10; i++) {
        board = currState.getSudoku().getBoard();

        if (currState.getSudoku().isValid(minx, miny, i)) {
          board[minx][miny] = i;
          SearchSudoku ss = new SearchSudoku(new Sudoku(board), new LinkedList<Sudoku>(currState.getPath()));
          ss.getPath().add(currState.getSudoku());
          fringe.add(ss);
        }
      }
    }
    return null;
  }
}
