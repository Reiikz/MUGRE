package tk.reiikz.robotdriver.network;

import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

import tk.reiikz.robotdriver.logging.Logger;

public class DataSender
{
	protected NetworkData network;
	protected RoverListener listener;
	protected RoverListener sync_listener;
	protected Logger logger;
	protected ExecutorService exec;
	protected String lastMessage;
	protected Future<MessageStatus> mest;
	protected int sync_fails;
	
	public DataSender(NetworkData network, RoverListener listener, RoverListener sync_listener, Logger logger)
	{
		this.network = network;
		this.listener = listener;
		this.sync_listener = sync_listener;
		this.logger = logger;
		exec = Executors.newCachedThreadPool();
		lastMessage = "";
	}
	
	public void resend()
	{
		send(lastMessage);
	}
	
	public void send(String command, boolean register, boolean retry)
	{
		exec = Executors.newCachedThreadPool();
		mest = exec.submit(new CallableSender(logger, listener, command, network, false));
		exec.shutdown();
		if(register) lastMessage = command;
	}
	
	public void send(String command)
	{
		send(command, true, true);
	}
	
	public MessageStatus getMessageStatus()
	{
		try {
			return mest.get();
		} catch (InterruptedException | ExecutionException e) {
			e.printStackTrace();
		}
		return new MessageStatus("", false);
	}
	
	public boolean sync()
	{
		int c = 0;
		send(RoverCommands.SYNC, false, false);
		logger.log("Waiting for Sync answer");
		while(!lastMessage.equals(sync_listener.getData()) && c < network.getMaxAttempts())
		{
			try {
				Thread.sleep(network.getDelay());
			}catch(Exception e1) {e1.printStackTrace();}
			c++;
		}
		if(lastMessage.equals(sync_listener.getData()))
		{
			sync_fails = 0;
			return true;
		}
		else
		{
			if(sync_fails > 10)
			{
				logger.log("SYNC FAILED :( !");
				return false;
			}
			resend();
			try {
				mest.get();
			}catch(Exception e) {e.printStackTrace();}
			sync_fails++;
			sync();
			return false;
		}
	}
	
	public boolean isFinished()
	{
		return exec.isTerminated();
	}
}
