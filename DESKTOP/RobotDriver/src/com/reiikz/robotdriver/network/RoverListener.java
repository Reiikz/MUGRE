package com.reiikz.robotdriver.network;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Scanner;

public class RoverListener extends Thread
{
	
	private int PORT;
	private String data;
	
	public RoverListener()
	{
		super();
		data = "";
	}
	
	public void set(int P)
	{
		PORT = P;
	}
	
	public String getData()
	{
		return data;
	}
	
	public void clean()
	{
		data = "";
	}
	
	public void run()
	{
		try (ServerSocket listener = new ServerSocket(PORT)) {
            System.out.println("Started listener at port: " + PORT);
            while (true) {
                try (Socket sock = listener.accept()) {
                	@SuppressWarnings("resource")
					Scanner in = new Scanner(sock.getInputStream());
			        data = in.nextLine();
			        System.out.println("ReceivedData on socket: " + PORT + ": " + data);
                }
            }
        } catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
