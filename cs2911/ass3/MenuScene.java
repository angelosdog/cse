import java.awt.Color;
import java.awt.Font;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import javax.swing.*;

/**
 * Scene for the menu.
 *
 */

public class MenuScene extends Scene {
  private static final long serialVersionUID = -4114805963130374216L;
  
  private final static String NAME = "Menu";
  private final static int WINDOW_WIDTH = 300;
  private final static int WINDOW_HEIGHT = 300;
  private final static int START_X = 100;
  private final static int START_Y = 100;
  
  public MenuScene() {
    super();
    title = NAME;
    width = WINDOW_WIDTH;
    height = WINDOW_HEIGHT;
    startX = START_X;
    startY = START_Y;
  }
  
  @Override
  public void run() {
    setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    JPanel panel = new JPanel();
    panel.setLayout(new GridBagLayout());
    panel.setBackground(new Color(255, 255, 255));
    
    GridBagConstraints c = new GridBagConstraints();
    c.fill = GridBagConstraints.HORIZONTAL;
    c.weightx = 1;
    c.weighty = 1;
    
    c.gridx = 4;
    c.gridy = 1;
    c.gridwidth = 2;
    JLabel title = new JLabel("Sudoku", JLabel.CENTER);
    title.setFont(new Font("Arial", Font.BOLD, 50));
    panel.add(title, c);
    
    c.gridx = 1;
    c.gridy = 2;
    c.gridwidth = 4;
    String[] difficulties = { "Novice", "Easy", "Medium", "Hard", "Evil" };
    final JComboBox difficulty = new JComboBox(difficulties);
    panel.add(difficulty, c);
    
    c.gridx = 5;
    c.gridy = 2;
    c.gridwidth = 4;
    JButton start = new JButton("Start");
    start.addActionListener(new ActionListener() {
      @Override
      public void actionPerformed(ActionEvent e) {
        GUI.getInstance().setDifficulty(difficulty.getSelectedIndex() + 1);
        GUI.getInstance().switchScene(GUI.SUDOKU_SCENE, null);
      }
    });
    panel.add(start, c);
    
    c.gridx = 1;
    c.gridy = 3;
    c.gridwidth = 6;
    JButton help = new JButton("Help");
    help.addActionListener(new ActionListener() {
      @Override
      public void actionPerformed(ActionEvent e) {
        HelpScene hs = new HelpScene();
        hs.run();
      }
    });
    panel.add(help, c);
    
    add(panel);
    
    JMenuBar menuBar = new JMenuBar();
    JMenu menu = new JMenu("File");
    JMenuItem load = new JMenuItem("Load");
    JMenuItem stSvr = new JMenuItem("Start Server");
    JMenuItem cntToSvr = new JMenuItem("Connect to Server");
    JMenuItem createPuz = new JMenuItem("Create Puzzle!");

    load.addActionListener(new ActionListener() {
      @Override
      public void actionPerformed(ActionEvent e) {
        
          JFileChooser fileChooser = new JFileChooser();
          fileChooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
          int choice = fileChooser.showOpenDialog(fileChooser);
          if (choice == JFileChooser.APPROVE_OPTION) {
            File file = fileChooser.getSelectedFile();
            try {
              FileInputStream fileIn = new FileInputStream (file);
              ObjectInputStream objectIn = new ObjectInputStream (fileIn);
              Object o = objectIn.readObject ();
              objectIn.close();
              if (o instanceof Game) {
                Game g = (Game) o;
                GUI.getInstance().switchScene(GUI.SUDOKU_SCENE, g);
              } else {
                JOptionPane.showMessageDialog(fileChooser, "The file you chose was not a valid save file.");
              }
            } catch (IOException e1) {
              JOptionPane.showMessageDialog(fileChooser, "The file you chose was not a valid save file.");
            } catch (ClassNotFoundException e1) {
              JOptionPane.showMessageDialog(fileChooser, "The file you chose was not a valid save file.");
            }

          }
        

      }
    });

    stSvr.addActionListener(new ActionListener() {
      @Override
      public void actionPerformed(ActionEvent e) {
        StSvrScene sss = new StSvrScene();
        sss.run();
      }
    });
    cntToSvr.addActionListener(new ActionListener() {
      @Override
      public void actionPerformed(ActionEvent e) {
        CntSvrScene css = new CntSvrScene();
        css.run();
      }
    });
    createPuz.addActionListener(new ActionListener() {
      @Override
      public void actionPerformed(ActionEvent e) {
        SudokuScene ss = new SudokuScene(Generator.CREATIVE);
        ss.run();
      }
    });
    
    menu.add(load);
    menu.add(stSvr);
    menu.add(cntToSvr);
    menu.add(createPuz);
    menuBar.add(menu);
    setJMenuBar(menuBar);
    
    super.run();
    setResizable(false);
  }

}
