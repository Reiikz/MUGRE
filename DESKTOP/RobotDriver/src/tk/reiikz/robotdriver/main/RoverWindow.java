
package tk.reiikz.robotdriver.main;

import java.awt.EventQueue;

import javax.swing.JFrame;
import javax.swing.JTextField;

import tk.reiikz.robotdriver.logging.Logger;
import tk.reiikz.robotdriver.network.DataSenderManager;
import tk.reiikz.robotdriver.network.NetworkData;
import tk.reiikz.robotdriver.network.RoverCommands;
import tk.reiikz.robotdriver.network.RoverListener;

import javax.swing.JLabel;
import javax.swing.JButton;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.util.Timer;
import java.util.TimerTask;

public class RoverWindow {

	private JFrame frmMugresBrain;
	private JTextField txtIP;
	private JTextField txtOutPort;

	/**
	 * Launch the application.
	 */
	public static void main(String[] args) {
		EventQueue.invokeLater(new Runnable() {
			public void run() {
				try {
					RoverWindow window = new RoverWindow();
					window.frmMugresBrain.setVisible(true);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}

	/**
	 * Create the application.
	 */
	public RoverWindow() {
		initialize();
	}

	/**
	 * Initialize the contents of the frame.
	 */
	int SPORT = 0, CPORT = 0;
	String IP;
	RoverListener listener = new RoverListener();
	RoverListener sync_listener = new RoverListener();
	Logger logger;
	DataSenderManager dsm;
	boolean w, a, s, d, stop;
	String direction, lastDirection;
	RoverCommands commands =  new RoverCommands();
	
	private JTextField txtInPort;
	private void initialize() {
		
		
		frmMugresBrain = new JFrame();
		frmMugresBrain.setTitle("MUGRE's Brain");
		
		frmMugresBrain.setBounds(100, 100, 600, 400);
		frmMugresBrain.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frmMugresBrain.getContentPane().setLayout(null);
		
		txtIP = new JTextField();
		txtIP.setText("192.168.0.106");
		txtIP.setBounds(66, 11, 160, 20);
		frmMugresBrain.getContentPane().add(txtIP);
		txtIP.setColumns(10);
		
		txtOutPort = new JTextField();
		txtOutPort.setText("5500");
		txtOutPort.setColumns(10);
		txtOutPort.setBounds(313, 11, 160, 20);
		frmMugresBrain.getContentPane().add(txtOutPort);
		
		txtInPort = new JTextField();
		txtInPort.setText("5505");
		txtInPort.setColumns(10);
		txtInPort.setBounds(313, 39, 160, 20);
		frmMugresBrain.getContentPane().add(txtInPort);
		
		JLabel lblNewLabel = new JLabel("IP:");
		lblNewLabel.setBounds(10, 14, 46, 14);
		frmMugresBrain.getContentPane().add(lblNewLabel);
		
		JLabel lblNewLabel_1 = new JLabel("Send PORT:");
		lblNewLabel_1.setBounds(236, 14, 67, 14);
		frmMugresBrain.getContentPane().add(lblNewLabel_1);
		
		JLabel lblNewLabel_2 = new JLabel("Status:");
		lblNewLabel_2.setBounds(10, 67, 46, 14);
		frmMugresBrain.getContentPane().add(lblNewLabel_2);
		
		JLabel lblStatus = new JLabel("Not connected.");
		lblStatus.setBounds(66, 67, 407, 14);
		frmMugresBrain.getContentPane().add(lblStatus);
		logger = new Logger(lblStatus);
		
		JButton btnReady = new JButton("Ready");
		btnReady.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				
				SPORT = Integer.parseInt(txtInPort.getText());
				CPORT = Integer.parseInt(txtOutPort.getText());
				IP = txtIP.getText();
				
				listener.set(SPORT);
				if(!listener.isAlive())
					listener.start();
				sync_listener.set(SPORT+1);
				if(!sync_listener.isAlive())
					sync_listener.start();
				
				dsm = new DataSenderManager(new NetworkData(IP, CPORT, 200, 1), listener, sync_listener, logger);
				dsm.sendNockNock();
				
				Thread thread = new Thread(){
				    public void run(){
				    	while(!dsm.isFinished())
						{
							try {
								Thread.sleep(200);
							}catch(Exception e1) {e1.printStackTrace();}
						}
						if(dsm.sync())
						{
							logger.log("WHO IS THERE? :D");
							btnReady.setEnabled(false);
							txtIP.setEnabled(false);
							txtInPort.setEnabled(false);
							txtOutPort.setEnabled(false);
						}
				    }
				  };
			  thread.start();
			  
			}
		});
		btnReady.setBounds(483, 10, 89, 23);
		frmMugresBrain.getContentPane().add(btnReady);
		
		JLabel lblRecvPort = new JLabel("Recv PORT:");
		lblRecvPort.setBounds(236, 42, 67, 14);
		frmMugresBrain.getContentPane().add(lblRecvPort);
		
		JButton btnStop = new JButton("??");
		btnStop.addKeyListener(new KeyAdapter() {
			@Override
			public void keyPressed(KeyEvent arg0) {
				switch(Character.toString(arg0.getKeyChar()).toLowerCase())
				{
					case "w":
					{
						w = true;
						break;
					}
					case "s":
					{
						s = true;
						break;
					}
					case "a":
					{
						a = true;
						break;
					}
					case "d":
					{
						d = true;
						break;
					}
					default:
					{
						stop = true;
						break;
					}
				}
				updateDirection();
			}
			@Override
			public void keyReleased(KeyEvent e) {
				switch(Character.toString(e.getKeyChar()).toLowerCase())
				{
					case "w":
					{
						w = false;
						break;
					}
					case "s":
					{
						s = false;
						break;
					}
					case "a":
					{
						a = false;
						break;
					}
					case "d":
					{
						d = false;
						break;
					}
					default:
					{
						stop = false;
						break;
					}
				}
				updateDirection();
			}
		});
		btnStop.setBounds(222, 178, 102, 23);
		frmMugresBrain.getContentPane().add(btnStop);
	}
	
	private void updateDirection()
	{
		String detected_keys = "";
		if(w && !s)
			detected_keys += "w";
		if(s && !w)
			detected_keys += "s";
		if(a && !d)
			detected_keys += "a";
		if(d && !a)
			detected_keys += "d";
		if(stop || (!w && !a && !s && !d))
			detected_keys = "";
		
		switch(detected_keys)
		{
			case "w":
			{
				direction = RoverCommands.FORWARD;
				break;
			}
			case "s":
			{
				direction = RoverCommands.BACKWARDS;
				break;
			}
			case "wa":
			{
				direction = RoverCommands.FORWARD_LEFT;
				break;
			}
			case "wd":
			{
				direction = RoverCommands.FORWARD_RIGHT;
				break;
			}
			case "sa":
			{
				direction = RoverCommands.BACKWARDS_LEFT;
				break;
			}
			case "sd":
			{
				direction = RoverCommands.BACKWARDS_RIGHT;
				break;
			}
			default:
			{
				direction = RoverCommands.STOP;
				break;
			}
		}
		
		if(!direction.equals(lastDirection))
		{
			dsm.send(direction);
			Thread thread = new Thread(){
			    public void run(){
			    	while(!dsm.isFinished())
					{
						try {
							Thread.sleep(200);
						}catch(Exception e1) {e1.printStackTrace();}
					}
					if(dsm.sync())
					{
						try {
							Thread.sleep(1000);
						}catch(Exception e1) {e1.printStackTrace();}
						if(!direction.equals(RoverCommands.STOP))
							logger.log("Moving:   " + commands.getCommandName(direction));
						else
							logger.log("STOPPED");
					}
			    }
			  };
			  thread.start();
			  lastDirection = direction;
		}
	}
}
