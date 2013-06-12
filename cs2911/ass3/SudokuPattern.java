import java.util.Random;

/**
 * Patterns which control the "digging" of holes out of a full completed Sudoku.
 * 
 */

public class SudokuPattern {

	public static final int[] rowByRowPattern = {
		 0,  1,  2,  3,  4,  5,  6,  7,  8,
		 9, 10, 11, 12, 13, 14, 15, 16, 17,
		18, 19, 20, 21, 22, 23, 24, 25, 26,
		27, 28, 29, 30, 31, 32, 33, 34, 35,
		36, 37, 38, 39, 40, 41, 42, 43, 44,
		45, 46, 47, 48, 49, 50, 51, 52, 53,
		54, 55, 56, 57, 58, 59, 60, 61, 62,
		63, 64, 65, 66, 67, 68, 69, 70, 71,
		72, 73, 74, 75, 76, 77, 78, 79, 80,
	};
	
	public static final int[] snakePattern = {
		 0,  1,  2,  3,  4,  5,  6,  7,  8,
		17, 16, 15, 14, 13, 12, 11, 10,  9,
		18, 19, 20, 21, 22, 23, 24, 25, 26,
		35, 34, 33, 32, 31, 30, 29, 28, 27,
		36, 37, 38, 39, 40, 41, 42, 43, 44,
		53, 52, 51, 50, 49, 48, 47, 46, 45,
		54, 55, 56, 57, 58, 59, 60, 61, 62,
		71, 70, 69, 68, 67, 66, 65, 64, 63,
		72, 73, 74, 75, 76, 77, 78, 79, 80,
	};
	
	public static final int[] LRTBPattern = {
		 0,  1,  2,  3,  4,  5,  6,  7,  8,
		17, 18, 19, 20, 21, 22, 23, 24,  9,
		32, 33, 34, 35, 36, 37, 38, 25, 10,
		45, 46, 47, 48, 49, 50, 39, 26, 11,
		56, 57, 58, 59, 60, 51, 40, 27, 12,
		65, 64, 67, 68, 61, 52, 41, 28, 13,
		72, 73, 74, 69, 62, 53, 42, 29, 14,
		77, 78, 75, 70, 63, 54, 43, 30, 15,
		80, 79, 76, 71, 64, 55, 44, 31, 16,
	};

  public static int[] randomPattern() {
    int totalCells = Sudoku.WIDTH * Sudoku.HEIGHT;
    int[] p = new int[totalCells];
    boolean[] used = new boolean[totalCells];
    Random rg = new Random();
    for (int i = 0; i < totalCells; i++) {
      boolean setCell = false;
      while (!setCell) {
        int cell = rg.nextInt(totalCells);
        if (!used[cell]) {
          p[i] = cell;
          used[cell] = true;
          setCell = true;
        }
      }
    }
    return p;
  }

}
