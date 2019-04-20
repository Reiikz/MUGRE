package com.reiikz.robotdriver.network;

public class MessageStatus
{
	private String tryedToSend;
	private boolean success;
	
	public MessageStatus(String tryedToSend, boolean success)
	{
		this.tryedToSend = tryedToSend;
		this.success = success;
	}
	
	public boolean wasSuccessful()
	{
		return success;
	}
	
	public String getMessage()
	{
		return tryedToSend;
	}
}
