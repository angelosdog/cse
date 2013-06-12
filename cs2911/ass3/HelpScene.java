import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Font;
import java.awt.Insets;

import javax.swing.JPanel;
import javax.swing.JTextArea;

/**
 * Help window.
 * 
 */

public class HelpScene extends Scene {
  private static final long serialVersionUID = -7920702506596720555L;

  private final static String NAME = "Help";
  private final static int WINDOW_WIDTH = 600;
  private final static int WINDOW_HEIGHT = 600;
  private final static int START_X = 120;
  private final static int START_Y = 140;

  private final static String HELP_TEXT = "    Welcome to Sudoku!\n\n"
      + "    Rules of Sudoku\n"
      + "      In Sudoku, you have a 9x9 grid, where each row, column and "
      + " 3x3 sub-grid contains the numbers 1..9.\n"
      + "      Your job is to work out a complete board by filling in "
      + " the empty cells given a partially filled board, making sure"
      + " that it satifies the game rules.\n"
      + "      In this application, we have added a few things to help you"
      + " make your experience less frustrating. Here are a list of things"
      + " you can do:\n"
      + "        1. Automatically prevents you from inserting a number that"
      + " would break the game rules.\n"
      + "        2. Hints and a solver in case you get stuck.\n"
      + "        3. Multiple difficulties to suit your skill.\n"
      + "        4. Novice and easy difficulties tells you if you have inserted "
      + "an incorrect cell.\n\n"
      + "      We also have some various additional features:\n"
      + "        1. Create your own board! If you have a sudoku from the "
      + "newspaper or another source, you can type it in and it will tell "
      + "you the solution\n"
      + "        2. Network play so you can play with multiple friends and "
      + "team up to solve various sudokus!";

  public HelpScene() {
    super();
    title = NAME;
    width = WINDOW_WIDTH;
    height = WINDOW_HEIGHT;
    startX = START_X;
    startY = START_Y;
  }

  @Override
  public void run() {
    JPanel panel = new JPanel();
    panel.setLayout(new BorderLayout());
    panel.setBackground(new Color(255, 255, 255));

    JTextArea textArea = new JTextArea(HELP_TEXT, WINDOW_WIDTH, WINDOW_HEIGHT);
    textArea.setFont(new Font("Arial", Font.PLAIN, 17));
    textArea.setMargin(new Insets(30, 30, 30, 30));
    textArea.setEditable(false);
    textArea.setLineWrap(true);
    textArea.setWrapStyleWord(true);

    panel.add(textArea, BorderLayout.CENTER);
    add(panel);

    super.run();
  }

}
