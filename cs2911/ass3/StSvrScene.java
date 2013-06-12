import java.awt.Color;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;

/**
 * The scene for the start server window.
 *
 */

public class StSvrScene extends Scene {
  private static final long serialVersionUID = 2811765674132052524L;
  
  private final static String NAME = "StSvr";
  private final static int WINDOW_WIDTH = 270;
  private final static int WINDOW_HEIGHT = 150;
  private final static int START_X = 110;
  private final static int START_Y = 110;

  private SudokuServer server = null;
  
  public StSvrScene() {
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
    panel.setLayout(new GridBagLayout());
    panel.setBackground(new Color(255, 255, 255));
    
    GridBagConstraints c = new GridBagConstraints();
    c.fill = GridBagConstraints.BOTH;
    c.weightx = 1;
    c.weighty = 1;
    
    c.gridx = 0;
    c.gridy = 0;
    JLabel port = new JLabel("Port: ");
    panel.add(port, c);
    
    c.gridx = 0;
    c.gridy = 1;
    JLabel difficulty = new JLabel("Difficulty: ");
    panel.add(difficulty, c);
    
    c.gridx = 1;
    c.gridy = 0;
    final JTextField portTxt = new JTextField(20);
    panel.add(portTxt, c);
    
    c.gridx = 1;
    c.gridy = 1;
    String[] difficulties = { "Novice", "Easy", "Medium", "Hard", "Evil" };
    final JComboBox difficultyCB = new JComboBox(difficulties);
    panel.add(difficultyCB, c);
    
    c.gridx = 0;
    c.gridy = 2;
    c.gridwidth = 2;
    final JButton start = new JButton("Start");
    panel.add(start, c);
    
    c.gridx = 0;
    c.gridy = 3;
    c.gridwidth = 2;
    final JLabel status = new JLabel("Not started");
    panel.add(status, c);
    
    panel.setBorder(BorderFactory.createTitledBorder("Server Details"));
    
    add(panel);
    
    super.run();
    setResizable(false);
    
    start.addActionListener(new ActionListener() {
      public void actionPerformed(ActionEvent arg0) {
        if (server == null) {
          try {
            int p = Integer.parseInt(portTxt.getText());
            int d = difficultyCB.getSelectedIndex() + 1;
            server = new SudokuServer(p, d);
            new Thread(server).start();
            status.setText("Server started");
            start.setText("Stop");
          }
          catch (NumberFormatException e) {
            status.setText("Invalid input");
          }
        }
        else {
          server.stop();
          status.setText("Server stopped");
          start.setText("Start");
          server = null;
        }
      }
    });

    addWindowListener(new WindowAdapter() {
      public void windowClosing(WindowEvent e) {
        if (server != null)
          server.stop();
      }
    });
    
  }
  
}
