import javax.swing.JFrame;

/**
 * Scene which extends a JFrame. A Scene is a frame but can have specific
 * properties pertaining to this application (extensibility) All scenes/frames
 * for this application should extend this class.
 */

public abstract class Scene extends JFrame {
  private static final long serialVersionUID = 5316320140917286261L;

  private final static int DEFAULT_WIDTH = 600;
  private final static int DEFAULT_HEIGHT = 600;
  private final static int DEFAULT_X = 100;
  private final static int DEFAULT_Y = 100;

  protected String title;
  protected int width;
  protected int height;
  protected int startX;
  protected int startY;

  public Scene() {
    width = DEFAULT_WIDTH;
    height = DEFAULT_HEIGHT;
    startX = DEFAULT_X;
    startY = DEFAULT_Y;
  }

  public void run() {
    setTitle(title);
    setSize(width, height);
    setLocation(startX, startY);
    setVisible(true);
  }

}
