package com.reiikz.robotdriver.network;

import com.reiikz.robotdriver.logging.Logger;

public class DataSenderManager extends DataSender
{
	public DataSenderManager(NetworkData network, RoverListener listener, RoverListener sync_listener, Logger logger) {
		super(network, listener, sync_listener, logger);
	}

	public void sendNockNock()
	{
		send(RoverCommands.NOCK_NOCK);
	}
}
