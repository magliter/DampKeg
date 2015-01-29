#pragma once

#include "rambler\Stream\TCPStream.hpp"

namespace Rambler{
	namespace Stream{
		
		ref class TCPStream
		{
		public:
			TCPStream(System::String^ domainName, System::String^ serviceName);
			~TCPStream();
			System::String^ getDomainName();
			System::String^ getServiceName();
			System::String^ getRemoteHostName();
			System::UInt16  getRemotePortNumber();

		private:
			rambler::Stream::TCPStream * stream;
		};
	}
}