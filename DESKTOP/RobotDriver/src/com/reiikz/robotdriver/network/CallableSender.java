package com.reiikz.robotdriver.network;

import java.io.IOException;
import java.io.OutputStream;
import java.net.Socket;
import java.util.concurrent.Callable;

import com.reiikz.robotdriver.logging.Logger;

public class CallableSender implements Callable<MessageStatus>
{
	private String IP;
	private int PORT;
	private Logger logger;
	private RoverListener rl;
	private int delay;
	private RoverCommands rcs;
	private int max_attempts;
	private String message_to_send;
	private boolean message_succeded;
	private boolean retry;
	
	public CallableSender(Logger logger, RoverListener rl, String message_to_send, NetworkData networkData, boolean retry)
	{
		this.IP = networkData.getIP();
		this.PORT =networkData.getPORT();
		this.logger = logger;
		this.rl = rl;
		this.delay = networkData.getDelay();
		this.rcs = new RoverCommands();
		this.max_attempts = networkData.getMaxAttempts();
		this.message_to_send = message_to_send;
		this.retry = retry;
	}
	
	public MessageStatus call()
	{
		if(rl.isAlive())
			Send(message_to_send);
		else
			logger.log("Listener is not running");
		return new MessageStatus(message_to_send, message_succeded);
	}
	
	/*
	 * If the busy flag is not true tries to send the message up to 50 times
	 * After eather fail, the message to send changes or a successful attempt, it turns the busy flag to false and
	 * returns if the message was successfully sended
	 */
	private boolean Send(String message)
	{
		int c = 1;
		{
			do
			{
				logger.log("SENDING MESSAGE: " + rcs.getCommandName(message));
				try {
					Socket s = new Socket(IP, PORT);
					OutputStream output = s.getOutputStream();
					output.write(message.getBytes());
					s.close();
				} catch (IOException e1) {
					e1.printStackTrace();
					c = 50;
				}
				
				try {
					logger.logn("waiting...");
					Thread.sleep(delay);
				}catch(Exception e1) {e1.printStackTrace();}
				c++;
			}
			while(! rl.getData().equals(message) && c < max_attempts && retry);
			if(c >= 50)
			{
				logger.log("Communication FAIL :( !");
			}
		}
		message_succeded = c < max_attempts;
		return message_succeded;
	}
}
