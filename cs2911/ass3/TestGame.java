import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.JUnit4;

public final class TestGame {
  /**
   * Test suite for IntegerMatrix class
   */
  @RunWith(JUnit4.class)
  public class IntegerMatrixTest {

    @Test
    public void testCreation() {
      for(int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
          IntegerMatrix IM = new IntegerMatrix(i, j);
          assert(IM.getLength() == i && IM.getHeight() == j);
        }
      }
      for(int i = 1; i < 10; i++) {
        for(int j = 1; j < 10; j++) {
          int[][] matrix = new int[i][j];
          IntegerMatrix IM = new IntegerMatrix(matrix);
          assert IM.getLength() == i && IM.getHeight() == j;
          assert matrix != IM.getMatrix(); 
        }
      }
    }
    
    @Test
    public void testSetValue() {
     IntegerMatrix IM = new IntegerMatrix(9, 9);
     IM.set(0, 0, 1);
     assert IM.get(1, 1) == 1;
     assert IM.existsInColumn(0, 1);
     assert IM.existsInRow(0, 1);
     for(int i = 0; i < 3; i++) {
       for(int j = 0; j < 3; j++) {
         assert IM.existsInSquare(i, j, 1);
       }
     }
     IM.set(7, 5, 6);
     assert IM.get(7, 5) == 6;
     assert IM.existsInColumn(5, 6);
     assert IM.existsInRow(7, 6);
     for(int i = 6; i < 9; i++) {
       for(int j = 3; j < 6; j++) {
         assert IM.existsInSquare(i, j, 6);
       }
     }
     for(int i = 0; i < 9; i++) {
       for(int j = 0; j < 9; j++) {
         IM.set(i, j, 1);
       }
     }
     assert IM.isComplete();
     IM.set(0, 0, 0);
     assert IM.isComplete();
    }
  }
  /**
   * Test suite for Sudoku class.
   */
  @RunWith(JUnit4.class)
  public class SudokuTest {
    @Test
    public void testCreation() {
      Sudoku s = new Sudoku();
      assert s.getBoard().length == 9;
      Sudoku s2 = new Sudoku(s);
      assert s != s2;
      assert s.equals(s);
      assert s.getEmptyCells().size() == 81;
      
      for(int i = 1; i < 10; i++) {
        for(int j = 1; j < 10; j++) {
          int[][] matrix = new int[i][j];
          s = new Sudoku(matrix);
          assert s.getBoard().length == i; // may not be a valid test
          assert matrix != s.getBoard();
          assert s.getEmptyCells().size() == i*j;
        }
      }
    }
    
    @Test
    public void testMoves() {
      Sudoku s = new Sudoku();
      assert !s.isComplete();
      s.performMove(0, 0, 1);
      assert s.getCell(0, 0) == 1;
      assert s.numsInSudoku() == 1;
      assert s.numsInColumn(0) == 1;
      assert s.numsInRow(0) == 1;
      assert s.getEmptyCells().size() == 80;
      for(int i = 1; i < 9; i++) {
        assert !s.isValid(i, 0, 1);
        assert !s.isValid(0, i, 1);
      }
      for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
          assert !s.isValid(i, j, 1);
        }
      }
      
      s.performMove(7, 5, 6);
      assert s.getCell(7, 5) == 6;
      assert s.numsInSudoku() == 2;
      assert s.numsInColumn(5) == 1;
      assert s.numsInRow(7) == 1;
      assert s.getEmptyCells().size() == 79;
      for(int i = 1; i < 9; i++) {
        assert !s.isValid(i, 5, 6);
        assert !s.isValid(7, i, 6);
      }
      for(int i = 6; i < 9; i++) {
        for(int j = 3; j < 6; j++) {
          assert !s.isValid(i, j, 6);
        }
      }
    }
  }
}
