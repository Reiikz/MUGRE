package tk.reiikz.robotdriver.network;

import java.util.Hashtable;

public class RoverCommands
{
	private Hashtable<String, String> commands;
	private Hashtable<String, String> commandName;
	
	public static final String STOP = "stop";
	public static final String FORWARD = "w";
	public static final String BACKWARDS = "s";
	public static final String FORWARD_LEFT = "wa";
	public static final String FORWARD_RIGHT = "wd";
	public static final String BACKWARDS_LEFT = "sa";
	public static final String BACKWARDS_RIGHT = "sd";
	public static final String LIGHTS_OFF = "loff";
	public static final String LIGHTS_ON = "lon";
	public static final String NOCK_NOCK = "nock nock";
	public static final String SYNC = "what ya doin";
	
	public RoverCommands()
	{
		commands = new Hashtable<String, String>();
		commandName = new Hashtable<String, String>();
		addCommand(RoverCommands.STOP, "STOP");
		addCommand(FORWARD, "FORWARD");
		addCommand(BACKWARDS, "BACKWARDS");
		addCommand(FORWARD_LEFT, "FORWARD: LEFT");
		addCommand(FORWARD_RIGHT, "FORWARD: RIGHT");
		addCommand(BACKWARDS_LEFT, "BACKWARDS: LEFT");
		addCommand(BACKWARDS_RIGHT, "BACKWARDS: RIGHT");
		addCommand(LIGHTS_OFF, "LIGHTS OFF");
		addCommand(LIGHTS_ON, "LIGHTS ON");
		addCommand(NOCK_NOCK, "NOCK, NOCK!");
		addCommand(SYNC, "WHAT YA DOIN");
	}
	
	private void addCommand(String command, String command_name)
	{
		commands.put(command_name, command);
		commandName.put(command, command_name);
	}
	
	public String getCommand(String command_name)
	{
		if(commands.containsKey(command_name))
			return commands.get(command_name);
		else
			return command_name;
	}
	
	public String getCommandName(String command)
	{
		if(commandName.containsKey(command))
			return commandName.get(command);
		else
			return command;
	}
}
