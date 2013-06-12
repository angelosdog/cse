import java.io.IOException;
import java.io.Serializable;

/**
 * An integer matrix class represented by a 2-dimensional array.
 * 
 */

public class IntegerMatrix implements Serializable {

  private static final long serialVersionUID = 5882250953181202821L;

  private int[][] matrix;
  private int length;
  private int height;

  public IntegerMatrix(int length, int height) {
    matrix = new int[length][height];
    this.length = length;
    this.height = height;
  }

  public IntegerMatrix(int[][] matrix) {
    this.matrix = matrix;
    this.length = matrix.length;
    if (matrix.length > 0) {
      this.height = matrix[0].length;
    } else {
      this.height = 0;
    }
  }

  public int getLength() {
    return length;
  }

  public int getHeight() {
    return height;
  }

  /**
   * Creates a deep copy of the matrix.
   * 
   * @return
   */

  public int[][] getMatrix() {
    // create a deep copy
    int[][] copy = new int[length][height];
    for (int i = 0; i < length; i++) {
      for (int j = 0; j < height; j++) {
        copy[i][j] = matrix[i][j];
      }
    }
    return copy;
  }

  public int get(int x, int y) {
    return matrix[x][y];
  }

  public void set(int x, int y, int value) {
    matrix[x][y] = value;
  }

  /**
   * Checks if a number already exists in a row.
   * 
   * @param row
   * @param value
   * @return
   */

  public boolean existsInRow(int row, int value) {
    for (int i = 0; i < length; i++) {
      if (matrix[i][row] == value)
        return true;
    }
    return false;
  }

  /**
   * Checks if a number already exists in a column.
   * 
   * @param row
   * @param value
   * @return
   */

  public boolean existsInColumn(int column, int value) {
    for (int i = 0; i < height; i++) {
      if (matrix[column][i] == value)
        return true;
    }
    return false;
  }

  /**
   * Checks if a number already exists in a 3x3 square.
   * 
   * @param row
   * @param value
   * @return
   */

  public boolean existsInSquare(int x, int y, int value) {
    x *= 3;
    y *= 3;
    for (int i = x; i < x + 3; i++) {
      for (int j = y; j < y + 3; j++) {
        if (matrix[i][j] == value)
          return true;
      }
    }
    return false;
  }

  /**
   * If the entire matrix is set with numbers besides zero.
   * 
   * @return
   */

  public boolean isComplete() {
    for (int i = 0; i < length; i++) {
      for (int j = 0; j < height; j++) {
        if (matrix[i][j] == 0) {
          return false;
        }
      }
    }

    return true;
  }

  /**
   * Compares IntegerMatrix by checking the width, height and contents in each
   * cell.
   */

  @Override
  public boolean equals(Object obj) {
    if (obj == null)
      return false;
    if (getClass() == obj.getClass()) {
      IntegerMatrix other = (IntegerMatrix) obj;

      if (length != other.length || height != other.height)
        return false;

      for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
          if (matrix[i][j] != other.matrix[i][j])
            return false;
        }
      }
      return true;
    }
    return false;
  }

  private void writeObject(java.io.ObjectOutputStream out) throws IOException {
    out.defaultWriteObject();
  }

  private void readObject(java.io.ObjectInputStream in) throws IOException,
      ClassNotFoundException {
    in.defaultReadObject();
    // validateState(); - not sure if necessary?
  }

}
