/*
 * TCPClient.h
 *
 *  Created on: Dec 19, 2017
 *      Author: Rebecca Bernstein
 */

#ifndef TCPCLIENT_H_
#define TCPCLIENT_H_

#include <string>

class TCPClient {
public:
	TCPClient(char* servername, int port);
	virtual ~TCPClient();

	int produce(void* buf, int size);

	int setupSocket(char* servername, int port);

	int writeBuf(char* buf, long size);

private:
	// TCP connection
	int sock;
};

#endif /* TCPCLIENT_H_ */
