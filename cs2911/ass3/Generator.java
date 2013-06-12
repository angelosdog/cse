import java.util.Random;

public class Generator {

  // difficulties and modes
  public static final int NOVICE = 1;
  public static final int EASY = 2;
  public static final int MEDIUM = 3;
  public static final int HARD = 4;
  public static final int EVIL = 5;
  public static final int CREATIVE = 6;

  // break counter for generating boards (for when we get stuck)
  private static final int BREAK_COUNTER = 100;

  /**
   * Generates a full Sudoku, then "digs" holes to form the Sudoku game.
   * The difficulty is set by the number of givens in total and the rows/columns, 
   * as well as the pattern being used
   * |difficulty  |givens |givens in row/columns  |pattern
   * |1           |>50    |5                      |random
   * |2           |36-49  |4                      |random
   * |3           |32-35  |3                      |row by row
   * |4           |28-31  |2                      |snake
   * |5           |22-27  |1                      |LRTB
   * @param difficulty
   * @return
   */
  
  public Sudoku generateBoard(int difficulty) {
    if (difficulty == CREATIVE) {
      Sudoku empty = emptyBoard();
      empty.setSolution(empty.getBoard());
      return empty;
    }

    Sudoku s = null;
    s = generateUnknownBoard();
    s.setSolution(s.getBoard());

    Random rg = new Random();

    int minGivens = 0;
    int minGivensInRC = 0;
    int[] pattern;

    if (difficulty == NOVICE) {
      minGivens = rg.nextInt(10) + 51;
      minGivensInRC = 5;
      pattern = SudokuPattern.randomPattern();
    } else if (difficulty == EASY) {
      minGivens = rg.nextInt(14) + 36;
      minGivensInRC = 4;
      pattern = SudokuPattern.randomPattern();
    } else if (difficulty == MEDIUM) {
      minGivens = rg.nextInt(4) + 32;
      minGivensInRC = 3;
      pattern = SudokuPattern.rowByRowPattern;
    } else if (difficulty == HARD) {
      minGivens = rg.nextInt(4) + 28;
      minGivensInRC = 2;
      pattern = SudokuPattern.snakePattern;
    } else if (difficulty == EVIL) {
      minGivens = rg.nextInt(6) + 22;
      minGivensInRC = 0;
      pattern = SudokuPattern.LRTBPattern;
    } else {
      minGivens = 0;
      minGivensInRC = 0;
      pattern = SudokuPattern.LRTBPattern;
    }

    for (int i = 0; i < Sudoku.WIDTH * Sudoku.HEIGHT; i++) {
      int x = pattern[i] % Sudoku.WIDTH;
      int y = pattern[i] / Sudoku.HEIGHT;
      if (s.numsInSudoku() > minGivens && s.numsInRow(y) > minGivensInRC
          && s.numsInColumn(x) > minGivensInRC) {
        Sudoku clone = new Sudoku(s);
        if (yieldsUnique(clone, x, y))
          s.performMove(x, y, 0);
      }
    }

    return s;
  }

  /**
   * Checks if removing a cell yields a unique solution.
   * @param s
   * @param dugX
   * @param dugY
   * @return
   */
  
  private boolean yieldsUnique(Sudoku s, int dugX, int dugY) {
    int prevValue = s.getBoard()[dugX][dugY];
    for (int i = 1; i < 10; i++) {
      if (i != prevValue && s.isValid(dugX, dugY, i)) {
        s.performMove(dugX, dugY, i);
        if (new Search(s).getSolution() != null)
          return false;
      }
    }
    return true;
  }

  /**
   * Generates a valid full Sudoku.
   * @return
   */

  private Sudoku generateUnknownBoard() {
    Sudoku s = null;
    Random rg = new Random();

    while (s == null) {
      s = new Sudoku();
      for (int i = 0; i < Sudoku.WIDTH && s != null; i++) {
        boolean[] used = { false, false, false, false, false, false, false,
            false, false };
        for (int j = 0; j < Sudoku.HEIGHT && s != null; j++) {
          boolean valid = false;
          int breakCounter = 0;
          while (!valid && s != null) {
            int randomNumber = rg.nextInt(Sudoku.MAX_MOVES) + 1;
            if (!used[randomNumber - 1] && s.isValid(j, i, randomNumber)) {
              s.performMove(j, i, randomNumber);
              used[randomNumber - 1] = true;
              valid = true;
            }
            if (breakCounter > BREAK_COUNTER) {
              s = null;
            }
            breakCounter++;
          }
        }
      }
    }
    return s;
  }

  private Sudoku emptyBoard() {
    Sudoku sudoku = new Sudoku();
    for (int i = 0; i < Sudoku.WIDTH; i++) {
      for (int j = 0; j < Sudoku.HEIGHT; j++) {
        sudoku.performMove(i, i, 0);
      }
    }
    return sudoku;
  }

}
