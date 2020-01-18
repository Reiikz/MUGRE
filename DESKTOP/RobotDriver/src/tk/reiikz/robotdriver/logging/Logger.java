package tk.reiikz.robotdriver.logging;

import javax.swing.JLabel;

public class Logger
{
	JLabel lbl;
	
	public Logger(JLabel plbl)
	{
		lbl = plbl;
	}
	
	public void log(String message)
	{
		System.out.println(message);
		lbl.setText(message);
	}
	
	public void logn(String message)
	{
		System.out.println(message);
		lbl.setText(lbl.getText() + " " + message);
	}
}
