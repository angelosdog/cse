import java.awt.Color;
import java.awt.Font;
import java.awt.Graphics2D;
import java.awt.Point;

/**
 * Number Shape which 
 * Simply draws text onto a given area.
 *
 */

public class Number extends Shape {

  private final static Color DEFAULT_COLOR = new Color(0, 0, 0);
  private final static int THICKNESS = 2;
  private int number; // number which will be drawn
  
  public Number(Point p, int width, int height, int number) {
    super(p, width, height, THICKNESS);
    this.number = number;
  }

  public void setNumber(int number) {
    this.number = number;
  }
  
  public void drawSelected(Graphics2D g) {
    draw(g);
  }

  @Override
  public void draw(Graphics2D g) {
    g.setColor(DEFAULT_COLOR);
    
    Font textFont = new Font("Arial", Font.BOLD, 16);  

    int fontSizeToUse = Math.min((int)(textFont.getSize() * width), height);
    textFont = new Font("Arial", Font.BOLD, fontSizeToUse);  
    
    g.setFont(textFont);
    if (number == 0) g.drawString("", leftCorner.x + width/3, leftCorner.y + height);
    else g.drawString(number + "", leftCorner.x + width/3, leftCorner.y + height);
  }

}
