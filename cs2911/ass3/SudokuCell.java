import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.Point;
import java.awt.geom.Line2D;
import java.awt.geom.Rectangle2D;

/**
 * Sudoku Cell Shape which is drawn onto the board.
 * Every cell contains a number in it (clear cell in a Sudoku is represented by 0).
 */

public class SudokuCell extends Shape {

  // Color schemes
  private final static Color DEFAULT_COLOR = new Color(0, 0, 0);
  private final static Color DEFAULT_COLOR_BG = new Color(255, 255, 255);
  private final static Color SELECTED_COLOR = new Color(16, 16, 16);
  private final static Color SELECTED_COLOR_BG = new Color(160, 160, 160);
  private final static Color SAME_SELECTED_COLOR = new Color(16, 16, 0);
  private final static Color SAME_SELECTED_COLOR_BG = new Color(255, 255, 0);
  private final static Color FIXED_COLOR = new Color(0, 0, 102);
  private final static Color FIXED_COLOR_BG = new Color(0, 153, 204);
  private final static Color CORRECT_COLOR = new Color(0, 51, 0);
  private final static Color CORRECT_COLOR_BG = new Color(0, 255, 66);
  private final static Color INCORRECT_COLOR = new Color(102, 0, 0);
  private final static Color INCORRECT_COLOR_BG = new Color(204, 51, 51);
  private final static Color TENTATIVE_COLOR = new Color(255, 255, 153);
  private final static Color TENTATIVE_COLOR_BG = new Color(255, 255, 102);
  private final static int THICKNESS = 2;

  private final static int GRID_THICKNESS = 2;

  private final int cellID[];
  private boolean isEditable;

  private boolean given = false; // is the cell a given one (generated)
  private boolean tentative = false;
  private int number; // number corresponding to this cell
  private Number numberShape; // number shape for this cell

  public SudokuCell(int cellID[], int number, boolean isEditable) {
    super(new Point(0, 0), 0, 0, THICKNESS);
    setNumber(number);
    this.cellID = new int[2];
    this.cellID[0] = cellID[0];
    this.cellID[1] = cellID[1];
    updateResize();
    this.isEditable = isEditable;
  }

  /**
   * Draws the cell and the number inside it. Sets the colors appropriately.
   * Also adds thick grid lines to the board based on what cell we are drawing.
   * 
   * @param g
   * @param defaultColor
   * @param defaultBGColor
   */

  public void draw(Graphics2D g, Color defaultColor, Color defaultBGColor) {
    updateResize();
    Rectangle2D.Double cell = new Rectangle2D.Double(leftCorner.x,
        leftCorner.y, width, height);
    g.setColor(defaultBGColor);
    g.fillRect(leftCorner.x, leftCorner.y, width, height);
    g.setColor(defaultColor);
    g.setStroke(new BasicStroke(thickness));
    g.draw(cell);

    g.setStroke(new BasicStroke(2 * GRID_THICKNESS + 2 * thickness));
    g.setColor(DEFAULT_COLOR);
    if (cellID[0] % 3 == 0 && cellID[1] == 0 && cellID[0] != 0) {
      int x = cellID[0] * Board.width / 9;
      g.draw(new Line2D.Double(x, leftCorner.y + 3 * GRID_THICKNESS / 2, x,
          Board.height / 2));
    }
    if (cellID[0] % 3 == 0 && cellID[1] == 8 && cellID[0] != 0) {
      int x = cellID[0] * Board.width / 9;
      g.draw(new Line2D.Double(x, Board.height / 2, x, leftCorner.y + height
          - 3 * GRID_THICKNESS / 2));
    }
    if (cellID[1] % 3 == 0 && cellID[0] == 0 && cellID[1] != 0) {
      int y = cellID[1] * Board.height / 9;
      g.draw(new Line2D.Double(leftCorner.x + 3 * GRID_THICKNESS / 2, y,
          Board.width / 2, y));
    }
    if (cellID[1] % 3 == 0 && cellID[0] == 8 && cellID[1] != 0) {
      int y = cellID[1] * Board.height / 9;
      g.draw(new Line2D.Double(Board.width / 2, y, leftCorner.x + width - 3
          * GRID_THICKNESS / 2, y));
    }

    numberShape.draw(g);
  }

  @Override
  public void draw(Graphics2D g) {
    draw(g, DEFAULT_COLOR, DEFAULT_COLOR_BG);
  }

  public void drawSelected(Graphics2D g) {
    draw(g, SELECTED_COLOR, SELECTED_COLOR_BG);
  }

  public void drawFixed(Graphics2D g) {
    draw(g, FIXED_COLOR, FIXED_COLOR_BG);
  }

  public void drawCorrect(Graphics2D g) {
    draw(g, CORRECT_COLOR, CORRECT_COLOR_BG);
  }

  public void drawIncorrect(Graphics2D g) {
    draw(g, INCORRECT_COLOR, INCORRECT_COLOR_BG);
  }

  public void drawSameSelected(Graphics2D g) {
    draw(g, SAME_SELECTED_COLOR, SAME_SELECTED_COLOR_BG);
  }

  public void drawTentative(Graphics2D g) {
    draw(g, TENTATIVE_COLOR, TENTATIVE_COLOR_BG);
  }

  public int[] getCellID() {
    return cellID.clone();
  }

  public int getNumber() {
    return number;
  }

  public boolean toggleTentative() {
    tentative = tentative == true ? false : true;
    return tentative;
  }

  public void setNumber(int number) {
    this.number = number;
    numberShape = new Number(new Point(0, 0), 0, 0, number);
  }

  public void setGiven() {
    given = true;
  }

  public boolean isEditable() {
    return isEditable;
  }

  public void setEditable(boolean editable) {
    isEditable = editable;
  }

  public boolean isGiven() {
    return given;
  }

  public boolean getGiven() {
    return given;
  }

  /**
   * Updates the width/length/size of the shapes based on the Board
   * width/height.
   */

  private void updateResize() {
    width = (Board.width / 3 - 3 * THICKNESS / 2 - GRID_THICKNESS / 2) / 3;
    height = (Board.height / 3 - 3 * THICKNESS / 2 - GRID_THICKNESS / 2) / 3;

    leftCorner.x = (cellID[0] / 3) * (Board.width / 3 + GRID_THICKNESS / 2)
        + (cellID[0] % 3) * width + thickness;
    leftCorner.y = (cellID[1] / 3) * (Board.height / 3 + GRID_THICKNESS / 2)
        + (cellID[1] % 3) * height + thickness;

    numberShape.setLeftCorner(new Point(leftCorner.x + width / 4 + 1,
        leftCorner.y + height / 4));
    numberShape.setHeight(height / 2);
    numberShape.setWidth(width / 2);
  }

}
