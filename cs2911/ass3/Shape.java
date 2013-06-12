import java.awt.Graphics2D;
import java.awt.Point;
import java.awt.geom.Point2D;

/**
 * Abstract Shape class which all shapes should extend. Contains properties that
 * each shape should have.
 * 
 */

public abstract class Shape {

  protected Point leftCorner;
  protected int width;
  protected int height;
  protected int thickness;

  public Shape(Point p, int width, int height, int thickness) {
    leftCorner = new Point(p.x, p.y);
    this.width = width;
    this.height = height;
    this.thickness = thickness;
  }

  public void setLeftCorner(Point leftCorner) {
    this.leftCorner = new Point(leftCorner.x, leftCorner.y);
  }

  public void setWidth(int width) {
    this.width = width;
  }

  public void setHeight(int height) {
    this.height = height;
  }

  public boolean contains(Point2D p) {
    return (p.getX() >= leftCorner.x && p.getX() <= leftCorner.x + width)
        && (p.getY() >= leftCorner.y && p.getY() <= leftCorner.y + height);
  }

  public abstract void draw(Graphics2D g);

}
