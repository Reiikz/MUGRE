package tk.reiikz.robotdriver.network;

public class NetworkData {
	
	private String IP;
	private int PORT;
	private int delay;
	private int max_attempts;
		
	public NetworkData(String IP, int PORT, int delay, int max_attempts)
	{
		this.IP = IP;
		this.PORT = PORT;
		this.delay = delay;
		this.max_attempts = max_attempts;
	}
	
	public String getIP()
	{
		return IP;
	}
	
	public int getPORT()
	{
		return PORT;
	}
	
	public int getDelay()
	{
		return delay;
	}
	
	public int getMaxAttempts()
	{
		return max_attempts;
	}
}
