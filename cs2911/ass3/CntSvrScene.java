import java.awt.Color;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;

import java.net.Socket;
import java.io.IOException;
import java.net.UnknownHostException;

/**
 * The scene for the client connect window.
 *
 */

public class CntSvrScene extends Scene {
  private static final long serialVersionUID = 3072929963909496338L;
  
  private final static String NAME = "CntSvr";
  private final static int WINDOW_WIDTH = 270;
  private final static int WINDOW_HEIGHT = 150;
  private final static int START_X = 110;
  private final static int START_Y = 110;
  
  public CntSvrScene() {
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
    JLabel addr = new JLabel("Address: ");
    panel.add(addr, c);
    
    c.gridx = 0;
    c.gridy = 1;
    JLabel port = new JLabel("Port: ");
    panel.add(port, c);
    
    c.gridx = 1;
    c.gridy = 0;
    final JTextField addrTxt = new JTextField(20);
    panel.add(addrTxt, c);
    
    c.gridx = 1;
    c.gridy = 1;
    final JTextField portTxt = new JTextField(20);
    panel.add(portTxt, c);
    
    c.gridx = 0;
    c.gridy = 2;
    c.gridwidth = 2;
    JButton connect = new JButton("Connect");
    panel.add(connect, c);
    
    c.gridx = 0;
    c.gridy = 3;
    c.gridwidth = 2;
    final JLabel status = new JLabel("Not connected");
    panel.add(status, c);
    
    panel.setBorder(BorderFactory.createTitledBorder("Server Details"));
    
    add(panel);
    
    super.run();
    setResizable(false);

    connect.addActionListener(new ActionListener() {
      public void actionPerformed(ActionEvent arg0) {
        try {
          int p = Integer.parseInt(portTxt.getText());
          String s = addrTxt.getText();
          // check if it is possible to connect
          Socket socket = new Socket(s, p);
          socket.close();
          // Start new scene
          SudokuClientScene client = new SudokuClientScene(s, p);
          client.run();
          status.setText("Connected");
        }
        catch (NumberFormatException e) {
          status.setText("Invalid input");
        }
        catch (UnknownHostException e) {
          status.setText("Could not establish connection");
        }
        catch (IOException e) {
          status.setText("Could not initialise reader or writer");
        }
      }
    });
    
  }

}
