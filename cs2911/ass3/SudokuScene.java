import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectOutputStream;

import javax.swing.*;

/**
 * Sudoku Scene which is the scene where the users play the game.
 *
 */

public class SudokuScene extends Scene implements KeyListener {
  private static final long serialVersionUID = 2748814766843005110L;

  private final static String NAME = "Sudoku Scene";
  private final static int WINDOW_WIDTH = 600;
  private final static int WINDOW_HEIGHT = 600;
  private final static int START_X = 110;
  private final static int START_Y = 120;

  public Game g;
  public Board board;
  private JLabel labelTime;
  private Timer t;

  private boolean finished;

  public SudokuScene() {
    this(3);
  }

  public SudokuScene(int difficulty) {
    super();
    title = NAME;
    width = WINDOW_WIDTH;
    height = WINDOW_HEIGHT;
    startX = START_X;
    startY = START_Y;

    newSudoku(difficulty); // change difficulty
    board = new Board(g);
    setTimer();
  }

  private void setTimer() {
    t = new Timer(1000, new ActionListener() {
      @Override
      public void actionPerformed(ActionEvent e) {
        if (isVisible()) {
          int time = g.getTime() + 1;
          g.setTime(time);
          labelTime.setText("Time: " + time / 3600 + "h" + time / 60 + "m"
              + time % 60 + "s");
        }
      }
    });
    t.start();
  }

  public SudokuScene(Game g) {
    this.g = g;
    board = new Board(g);
  }

  public void newSudoku(int difficulty) {
    g = new Game(difficulty);
  }

  public void run() {
    // setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

    JPanel leftPanel = new JPanel();
    leftPanel.setLayout(new GridBagLayout());
    leftPanel.setBackground(new Color(255, 255, 255));
    leftPanel.addKeyListener(this);

    GridBagConstraints c = new GridBagConstraints();
    c.fill = GridBagConstraints.BOTH;
    c.weightx = 1;
    c.weighty = 20;
    c.gridx = 0;
    c.gridy = 0;

    labelTime = new JLabel("Time: 00h00m00s");
    final Dimension size = labelTime.getPreferredSize();
    labelTime.setText("Time: 0h0m0s");
    leftPanel.setPreferredSize(size);
    leftPanel.add(labelTime, c);

    add(leftPanel, BorderLayout.WEST);

    for (int i = 0; i < 10; i++) {
      c.gridx = 0;
      c.gridy = i + 2;
      final JButton b = new JButton(i + "");
      if (i == 0)
        b.setText("clr");
      b.addKeyListener(this);
      b.addActionListener(new ActionListener() {
        @Override
        public void actionPerformed(ActionEvent e) {
          int[] selectedID = board.getSelectedID();
          if (selectedID != null) {
            if (!b.getText().equals("clr"))
              performMove(Integer.parseInt(b.getText()), selectedID[0],
                  selectedID[1]);
            else {
              if (board.getSelected().getGiven() != false) {
                performMove(0, selectedID[0], selectedID[1]);
              }
            }
          }
        }
      });
      leftPanel.add(b, c);
    }

    c.gridx = 0;
    c.gridy = 12;
    final JButton reset = new JButton("Reset");
    leftPanel.add(reset, c);
    reset.addKeyListener(this);
    reset.addActionListener(new ActionListener() {
      @Override
      public void actionPerformed(ActionEvent e) {
        g.reset();
        board.setGame(g);
        board.repaint();
      }
    });

    board.addKeyListener(this);
    add(board, BorderLayout.CENTER);

    JMenuBar menuBar = new JMenuBar();
    JMenu menu = new JMenu("File");
    JMenuItem save = new JMenuItem("Save");
    JMenuItem mm = new JMenuItem("Main Menu");
    JMenuItem pause = new JMenuItem("Pause");
    JMenuItem help = new JMenuItem("Help");
    JMenuItem quit = new JMenuItem("Quit");
    
    JMenu assist = new JMenu("Assistance");
    JMenuItem hint = new JMenuItem("Hint");
    JMenuItem solve = new JMenuItem("Solve");

    // ///////////////////////
    /* EVENT HANDLING HERE */
    // ///////////////////////

    save.addActionListener(new ActionListener() {
      @Override
      public void actionPerformed(ActionEvent e) {
        try {
          JFileChooser fileChooser = new JFileChooser();
          fileChooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
          int choice = fileChooser.showSaveDialog(fileChooser);
          if (choice == JFileChooser.APPROVE_OPTION) {
            File file = fileChooser.getSelectedFile();
            FileOutputStream fileOut = new FileOutputStream(file);
            ObjectOutputStream objectOut = new ObjectOutputStream(fileOut);
            objectOut.writeObject(g);
            objectOut.close();
          }
        } catch (IOException e1) {
        }
      }
    });
    mm.addActionListener(new ActionListener() {
      @Override
      public void actionPerformed(ActionEvent e) {
        GUI.getInstance().switchScene(GUI.MENU, null);
      }
    });
    pause.addActionListener(new ActionListener() {
      @Override
      public void actionPerformed(ActionEvent e) {

      }
    });
    help.addActionListener(new ActionListener() {
      @Override
      public void actionPerformed(ActionEvent e) {
        HelpScene hs = new HelpScene();
        hs.run();
      }
    });
    quit.addActionListener(new ActionListener() {
      @Override
      public void actionPerformed(ActionEvent e) {
        System.exit(0);
      }
    });

    hint.addActionListener(new ActionListener() {
      @Override
      public void actionPerformed(ActionEvent e) {
        if (!finished) {
          int[] m = g.getHint();
          if (m != null) {
            int[] temp = { m[0], m[1] };
            performMove(m[2], m[0], m[1]);
            board.getCell(temp).setGiven();
            if (g.isComplete()) {
              finished = true;
              JOptionPane.showMessageDialog(null, "Well done! You win!",
                  "Finished!", JOptionPane.INFORMATION_MESSAGE);
            }
          } else {
            JOptionPane.showMessageDialog(null,
                "You have inserted a number in the wrong position.");
          }
        }
      }
    });
    solve.addActionListener(new ActionListener() {
      @Override
      public void actionPerformed(ActionEvent e) {
        for (int i = 0; i < Sudoku.WIDTH; i++) {
          for (int j = 0; j < Sudoku.HEIGHT; j++) {
            int[] temp = { i, j };
            if (board.getNumber(temp) == 0) {
              board.setNumber(temp, g.getSolutionCell(i, j));
              board.getCell(temp).setGiven();
            } 
          }
        }
        finished = true;
        JOptionPane.showMessageDialog(null, "Well done! You win!", "Finished!",
            JOptionPane.INFORMATION_MESSAGE);
      }
    });

    menu.add(save);
    menu.add(mm);
    //menu.add(pause);
    menu.add(help);
    menu.add(quit);
    assist.add(hint);
    assist.add(solve);
    menuBar.add(menu);
    menuBar.add(assist);
    setJMenuBar(menuBar);

    super.run();
  }

  public void performMove(int num, int x, int y) {
    if (!finished) {
      int[] cellID = { x, y };
      int old = g.getCell(x, y);
      if (num != old) {
        g.performMove(x, y, 0);
        if (g.isValid(x, y, num) && board.isEditable(cellID)) {
          g.performMove(x, y, num);
          board.setNumber(cellID, num);
        } else {
          g.performMove(x, y, old);
          board.setNumber(cellID, old);
        }
      }
      
      if (g.isComplete()) {
        finished = true;
        JOptionPane.showMessageDialog(null, "Well done! You win!", "Finished!",
            JOptionPane.INFORMATION_MESSAGE);
      }
    }
  }

  private boolean isNumber(int c) {
    return c >= 48 && c <= 57;
  }

  private boolean isNumpad(int c) {
    return c >= 96 && c <= 105;
  }

  @Override
  public void keyPressed(KeyEvent e) {
    int keyID = e.getID();
    if (keyID == KeyEvent.KEY_PRESSED) {
      int key = e.getKeyCode();
      int[] selectedID = board.getSelectedID();
      if (selectedID != null) {
        if (isNumber(key)) {
          performMove(key - 48, selectedID[0], selectedID[1]);
        } 
        else if (isNumpad(key)) {
          performMove(key - 96, selectedID[0], selectedID[1]);
        } 
        else if (key == KeyEvent.VK_UP) {
          int[] cellID = board.getSelectedID();
          cellID[1]--;
          if (cellID[1] < 0) {
            cellID[0]--;
            cellID[1] = Sudoku.HEIGHT - 1;
          }
          if (cellID[0] >= 0)
            board.setSelected(cellID);
        } 
        else if (key == KeyEvent.VK_DOWN) {
          int cellID[] = board.getSelectedID();
          cellID[1]++;
          if (cellID[1] >= Sudoku.HEIGHT) {
            cellID[0]++;
            cellID[1] = 0;
          }
          if (cellID[0] < Sudoku.WIDTH)
            board.setSelected(cellID);
        } 
        else if (key == KeyEvent.VK_LEFT) {
          int cellID[] = board.getSelectedID();
          cellID[0]--;
          if (cellID[0] < 0) {
            cellID[0] = Sudoku.WIDTH - 1;
            cellID[1]--;
          }
          if (cellID[1] >= 0)
            board.setSelected(cellID);
        } 
        else if (key == KeyEvent.VK_RIGHT) {
          int cellID[] = board.getSelectedID();
          cellID[0]++;
          if (cellID[0] >= Sudoku.WIDTH) {
            cellID[0] = 0;
            cellID[1]++;
          }
          if (cellID[1] < Sudoku.HEIGHT)
            board.setSelected(cellID);
        }
      }
    }
  }

  @Override
  public void keyReleased(KeyEvent arg0) {

  }

  @Override
  public void keyTyped(KeyEvent arg0) {

  }

}
