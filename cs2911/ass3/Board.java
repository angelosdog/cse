import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Point;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.LinkedList;
import javax.swing.JComponent;

/**
 * This class is a JComponent which uses the Graphics2D library to draw the
 * Sudoku grid. The grid contains the 9x9 cells like a Sudoku game, and contains
 * a corresponding Game which contains all information about the current Sudoku
 * being played.
 * 
 */

public class Board extends JComponent {
  private static final long serialVersionUID = 5159406914015825999L;

  // width and height of the board
  // used to resize shapes when the frame is resized
  public static int width;
  public static int height;

  // cell shapes in the grid
  private SudokuCell[][] shapes;
  private SudokuCell selected;
  
  // a list of cells to highlights when we select a cell
  private LinkedList<SudokuCell> sameSelected;
  
  private Game game;

  /**
   * Sets up the board given a Game, and setups the standard JComponent setup.
   * Adds a mouse listener for selecting cell.
   * @param g
   */
  
  public Board(Game g) {
    shapes = new SudokuCell[Sudoku.WIDTH][Sudoku.HEIGHT];
    selected = null;
    sameSelected = new LinkedList<SudokuCell>();
    game = g;

    width = getWidth();
    height = getHeight();

    addMouseListener(new MouseAdapter() {
      @Override
      public void mousePressed(MouseEvent me) {
        Point pointClicked = me.getPoint();
        for (SudokuCell[] col : shapes) {
          for (SudokuCell s : col) {
            if (s.contains(pointClicked)) {
              if (selected == s) {
                selected = null;
                sameSelected = new LinkedList<SudokuCell>();
              } else {
                setSelected(s.getCellID());
              }
              repaint();
              return;
            }
          }
        }
        repaint();
      }
    });

    int[][] initialBoard = g.getInitialBoard().getMatrix();
    for (int i = 0; i < Sudoku.WIDTH; i++) {
      for (int j = 0; j < Sudoku.HEIGHT; j++) {
        int value = initialBoard[i][j];
        int[] cellID = { i, j };
        if (value == 0)
          shapes[i][j] = new SudokuCell(cellID, value, true);
        else
          shapes[i][j] = new SudokuCell(cellID, value, false);
      }
    }

    setGame(g);

    this.setBackground(new Color(100, 100, 100));
    repaint();
  }
  
  /**
   * Update the board with a new game
   * @param game
   */

  public void setGame(Game game) {
    this.game = game;
    for (int i = 0; i < Sudoku.WIDTH; i++) {
      for (int j = 0; j < Sudoku.HEIGHT; j++) {
        int value = game.getCell(i, j);
        shapes[i][j].setNumber(value);
      }
    }
  }

  public boolean isEditable(int[] cellID) {
    return shapes[cellID[0]][cellID[1]].isEditable();
  }

  public int getNumber(int[] cellID) {
    return shapes[cellID[0]][cellID[1]].getNumber();
  }

  public SudokuCell getCell(int[] cellID) {
    return shapes[cellID[0]][cellID[1]];
  }

  public void setNumber(int[] cellID, int number) {
    shapes[cellID[0]][cellID[1]].setNumber(number);
    repaint();
  }

  public int[] getSelectedID() {
    if (selected == null)
      return null;
    return selected.getCellID();
  }

  public SudokuCell getSelected() {
    return selected;
  }

  /**
   * Set the current numbers on the board to not editable.
   */
  
  public void setEditable() {
    int[][] board = game.getInitialBoard().getMatrix();
    for (int i = 0; i < Sudoku.WIDTH; i++) {
      for (int j = 0; j < Sudoku.HEIGHT; j++) {
        if (board[i][j] != 0)
          shapes[i][j].setEditable(false);
      }
    }

  }

  /**
   * Set the selected cell, then highlight the corresponding cells.
   * @param cellID
   */
  
  public void setSelected(int[] cellID) {
    selected = shapes[cellID[0]][cellID[1]];
    sameSelected = new LinkedList<SudokuCell>();
    if (selected == null)
      return;
    else {
      int x = cellID[0];
      int y = cellID[1];
      for (int i = 1; i <= x; i++) {
        int[] temp = cellID.clone();
        temp[0] -= i;
        sameSelected.add(getCell(temp));
      }
      for (int i = 1; i < Sudoku.WIDTH - x; i++) {
        int[] temp = cellID.clone();
        temp[0] += i;
        sameSelected.add(getCell(temp));
      }
      for (int i = 1; i <= y; i++) {
        int[] temp = cellID.clone();
        temp[1] -= i;
        sameSelected.add(getCell(temp));
      }
      for (int i = 1; i < Sudoku.HEIGHT - y; i++) {
        int[] temp = cellID.clone();
        temp[1] += i;
        sameSelected.add(getCell(temp));
      }

      // get 3x3 grid
      x -= (x % (Sudoku.WIDTH / 3));
      y -= (y % (Sudoku.HEIGHT / 3));
      for (int i = 0; i < Sudoku.WIDTH / 3; i++) {
        for (int j = 0; j < Sudoku.HEIGHT / 3; j++) {
          int[] temp = { x, y };
          temp[0] += i;
          temp[1] += j;
          SudokuCell c = getCell(temp);
          if (selected != c)
            sameSelected.add(c);
        }
      }
    }
    repaint();
  }
  
  /**
   * Override paintComponent to paint each cell
   * @param graphics
   */

  @Override
  public void paintComponent(Graphics g) {
    super.paintComponent(g);
    for (SudokuCell[] col : shapes) {
      for (SudokuCell s : col) {
        if (s.isEditable()) {
          int[] coords = s.getCellID();
          int x = coords[0];
          int y = coords[1];
          int n = s.getNumber();
          if (n == 0)
            s.draw((Graphics2D) g);
          else if (game.getDifficulty() == 6 && !s.isGiven())
            s.drawFixed((Graphics2D) g);
          else if (game.getDifficulty() < 4 && game.getSolutionCell(x, y) == n
              && !s.isGiven())
            s.drawCorrect((Graphics2D) g);
          else if (game.getDifficulty() < 4 && !s.isGiven())
            s.drawIncorrect((Graphics2D) g);
          else
            s.draw((Graphics2D) g);
        } else
          s.drawFixed((Graphics2D) g);
      }
    }
    if (selected != null) {
      selected.drawSelected((Graphics2D) g);
      for (SudokuCell sc : sameSelected) {
        sc.drawSameSelected((Graphics2D) g);
      }
    }
    width = getWidth();
    height = getHeight();
  }
}
