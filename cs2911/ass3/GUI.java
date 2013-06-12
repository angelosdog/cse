import javax.swing.JOptionPane;

/**
 * Class which is the main controller for what frames appear. One can think
 * about this like a tree, where this class is the root of the tree controlling
 * its sub-nodes. Whilst this isn't necessary (since main menu can be the root),
 * this is good for extensibility.
 * 
 */

public class GUI {

  // IDs of our sub-nodes (frames)
  public final static int NO_SCENE = -1;
  public final static int MENU = 0;
  public final static int SUDOKU_SCENE = 1;

  private Scene[] frames;
  private int currentFrame;
  private int currentDifficulty;

  private static GUI instance;

  public GUI() {
    frames = new Scene[2];
    for (int i = 0; i < 2; i++) {
      frames[i] = null;
    }
    currentFrame = NO_SCENE;
    instance = this;
    currentDifficulty = 1;
  }

  /**
   * Get the instance of the GUI. Used by the sub-nodes to access the methods in
   * GUI.
   * 
   * @return
   */

  public static GUI getInstance() {
    return instance;
  }

  /**
   * Switches the scene to another scene.
   * 
   * @param id
   *          - ID of the frame
   * @param g
   *          - Game if required to be used.
   */

  public void switchScene(int id, Game g) {
    if (currentFrame != NO_SCENE) {
      if (id == SUDOKU_SCENE && frames[SUDOKU_SCENE] != null) {
        int result = JOptionPane
            .showConfirmDialog(
                null,
                "There is a previous game. Do you want to discard the old game? (clicking no will open the old game)",
                "Warning", JOptionPane.INFORMATION_MESSAGE);
        if (result == JOptionPane.YES_OPTION) {
          frames[SUDOKU_SCENE].setVisible(false);
          frames[SUDOKU_SCENE].dispose();
          frames[SUDOKU_SCENE] = null;
        } else if (result == JOptionPane.NO_OPTION) {
        } else if (result == JOptionPane.CANCEL_OPTION
            || result == JOptionPane.DEFAULT_OPTION) {
          return;
        }
      }
      // frames[currentFrame].setVisible(false);
    }

    Scene s = frames[id];
    if (s != null) {
      frames[id].setVisible(true);
    } else {
      if (id == MENU) {
        s = new MenuScene();
      } else if (id == SUDOKU_SCENE) {
        if (g == null) {
          s = new SudokuScene(currentDifficulty);
        } else {
          s = new SudokuScene(g);
        }
      }
      frames[id] = s;
      s.run();
    }
    currentFrame = id;
  }

  public void setDifficulty(int difficulty) {
    currentDifficulty = difficulty;
  }

}
