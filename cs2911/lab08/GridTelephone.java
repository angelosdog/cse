import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

/**
   Presents a phone GUI for the voice mail system.
*/

public class GridTelephone extends JFrame {
	private JTextArea speakerField;
	private Connection connect;
	/**
    Constructs a telephone with a speaker, keypad,
    and microphone.
	 */
	public GridTelephone() {
		JPanel panel = new JPanel();
		this.getContentPane().add(panel);
		
		JPanel speakerPanel = new JPanel();
		speakerPanel.setLayout(new GridBagLayout());
		speakerPanel.add(new JLabel("Speaker:"));
		speakerField = new JTextArea(10, 25);
		speakerPanel.add(speakerField);
		
		String keyLabels = "123456789*0#";
		JPanel keyPanel = new JPanel();
		keyPanel.setLayout(new GridBagLayout());
	}
	
	public void speak(String output) {
		speakerField.setText(output);
	}
	
	public void run(Connection c) {
		connect = c;
	}
}
