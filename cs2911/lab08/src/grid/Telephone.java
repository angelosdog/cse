package grid;
import java.awt.*;
import java.awt.event.*;

import javax.swing.*;

/**
   Presents a phone GUI for the voice mail system.
*/

public class Telephone {
	private JTextArea speakerField;
	private Connection connect;
	/**
    Constructs a telephone with a speaker, keypad,
    and microphone.
	 */
	public Telephone() {
		JPanel phone = new JPanel();
		phone.setLayout(new GridBagLayout());
		
		GridBagConstraints gbc = new GridBagConstraints();
		
		JPanel speakerPanel = new JPanel();
		speakerPanel.setLayout(new GridBagLayout());
		gbc.gridx = 0;
		gbc.gridy = 0;
		gbc.gridwidth = 10;
		gbc.gridheight = 1;
		speakerPanel.add(new JLabel("Speaker:"),gbc);
		speakerPanel.setLayout(new GridBagLayout());
		gbc.gridx = 0;
		gbc.gridy = 1;
		gbc.gridwidth = 10;
		gbc.gridheight = 25;
		speakerField = new JTextArea(10, 25);
		speakerPanel.add(speakerField, gbc);
		phone.add(speakerPanel,gbc);
		
		String keyLabels = "123456789*0#";
		JPanel keyPanel = new JPanel();
		keyPanel.setLayout(new GridBagLayout());
		gbc.weightx = 0.3;
		gbc.weighty = 0.3;
		for (int i = 0; i < keyLabels.length(); i++) {
			final String label = keyLabels.substring(i, i+1);
			JButton keyButton = new JButton(label);
			gbc.gridx = i%3;
			gbc.gridy = i/3;
			gbc.gridwidth = 3;
			gbc.gridheight = 3;
			keyPanel.add(keyButton, gbc);
			keyButton.addActionListener(new
					ActionListener()
			{
				public void actionPerformed(ActionEvent event)
				{
					connect.dial(label);
				}
			});
		}
		gbc.gridx = 0;
		gbc.gridy = 26;
		gbc.gridwidth = 30;
		gbc.gridheight = 9;
		phone.add(keyPanel, gbc);

		final JTextArea microphoneField = new JTextArea(10,25);
		JPanel buttons = new JPanel();
		buttons.setLayout(new GridBagLayout());

		JButton speechButton = new JButton("Send speech");
		speechButton.addActionListener(new
				ActionListener()
		{
			public void actionPerformed(ActionEvent event)
			{
				connect.record(microphoneField.getText());
				microphoneField.setText("");
			}
		});

		JButton hangupButton = new JButton("Hangup");
		hangupButton.addActionListener(new
				ActionListener()
		{
			public void actionPerformed(ActionEvent event)
			{
				connect.hangup();
			}
		});

		gbc.gridx = 1;
		gbc.gridy = 1;
		gbc.gridwidth = 5;
		gbc.gridheight = 2;
		buttons.add(speechButton, gbc);
		
		gbc.gridx = 7;
		gbc.gridy = 1;
		gbc.gridwidth = 5;
		gbc.gridheight = 2;
		buttons.add(hangupButton, gbc);
		
		JPanel microphonePanel = new JPanel();
		microphonePanel.setLayout(new GridBagLayout());
		gbc.gridx = 0;
		gbc.gridy = 0;
		gbc.gridwidth = 10;
		gbc.gridheight = 1;
		microphonePanel.add(new JLabel("Microphone	:"), gbc);
		gbc.gridx = 0;
		gbc.gridy = 1;
		gbc.gridwidth = 10;
		gbc.gridheight = 25;
		microphonePanel.add(microphoneField, gbc);
		gbc.gridx = 0;
		gbc.gridy = 26;
		gbc.gridwidth = 10;
		gbc.gridheight = 4;
		microphonePanel.add(buttons, gbc);
		
		gbc.gridx = 0;
		gbc.gridy = 37;
		gbc.gridwidth = 20;
		gbc.gridheight = 26;

		phone.add(microphonePanel,gbc);


		JFrame frame = new JFrame();
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.getContentPane().add(phone);

		frame.pack();
		frame.setVisible(true);
	}
	
	public void speak(String output) {
		speakerField.setText(output);
	}
	
	public void run(Connection c) {
		connect = c;
	}
}
