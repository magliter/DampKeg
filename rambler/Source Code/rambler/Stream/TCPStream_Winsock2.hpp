/**********************************************************************************************************************
* @file    rambler/Connection/TCPStream_Winsock2.hpp
* @date    2014-07-18
* @brief   <# Brief Description #>
* @details <# Detailed Description #>
**********************************************************************************************************************/

#pragma once

#include "rambler/Stream/TCPStream.hpp"

#include <thread>

#include <winsock2.h>
#include <ws2tcpip.h>

#define SECURITY_WIN32
#include <security.h>
#include <schnlsp.h>

namespace rambler { namespace Stream {

	/**
	* A Winsock2 based implementation of TCP connection
	* @author Omar Stefan Evans
	* @date   2014-07-18
	*/
	class TCPStream_Winsock2 : public TCPStream {
	public:

		/**
		* Constructs an object representing a TCP connection to a host at the given domain for a particular service.
		* @author  Omar Stefan Evans
		* @date    2014-07-18
		* @param   domainName the domain name
		* @param   serviceName either a service name for a SRV lookup or a port number
		*/
		TCPStream_Winsock2(String domainName, String serviceName);

		/**
		* Destructor
		* @author  Omar Stefan Evans
		* @date    2014-07-18
		* @details closes this connection if it is not already closed
		*/
		virtual ~TCPStream_Winsock2();

		virtual State getState() override;

		/**
		* Opens this connection.
		* @author  Omar Stefan Evans
		* @date    2014-07-18
		* @return  true on success, false on failure
		*/
		virtual void open() override;

		/**
		* Secures this connection.
		* @author  Omar Stefan Evans
		* @date    2014-07-18
		* @return  true on success, false on failure
		*/
		virtual void secure() override;

		/**
		* Closes this connection.
		* @author  Omar Stefan Evans
		* @date    2014-07-18
		*/
		virtual void close() override;

		/**
		* Sends data over this connection
		* @author  Omar Stefan Evans
		* @date    2014-07-18
		* @param   data the data to send
		*/
		virtual void sendData(std::vector<UInt8> const & data) override;

		/**
		* Gets the domain name used to initialize this TCP stream.
		* @author  Omar Stefan Evans
		* @date    2015-01-27
		*/
		virtual String getDomainName() const override;

		/**
		* Gets the service name used to initialize this TCP stream.
		* @author  Omar Stefan Evans
		* @date    2015-01-27
		*/
		virtual String getServiceName() const override;

		/**
		* The remote host's actual name. This may be the same as the domain name if this TCP stream
		* was initialized with a port number instead of a service name.
		* @author  Omar Stefan Evans
		* @date    2015-01-27
		*/
		virtual String getRemoteHostName() const override;

		/**
		* The port number on the remote host for this TCP stream.
		* @author  Omar Stefan Evans
		* @date    2015-01-27
		*/
		virtual UInt16 getRemotePortNumber() const override;

	private:
		String domainName;
		String serviceName;
		String remoteHostName;
		UInt16 remotePortNumber;

		void readLoopFunction();

		WSADATA wsaData;
		SOCKET theSocket { INVALID_SOCKET };
		PSecurityFunctionTable securityFunctionTable;
		CtxtHandle securityContextHandle;

		std::thread readLoopThread;
	};

}}