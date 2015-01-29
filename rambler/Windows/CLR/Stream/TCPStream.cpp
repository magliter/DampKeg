
#include "TCPStream.hpp"
#include "../utilities.h"

namespace Rambler {
	namespace Stream{
		TCPStream::TCPStream(System::String^ domainName, System::String^ serviceName){

			
			auto temp = rambler::Stream::TCPStream::nativeTCPStream(ramblerString(domainName),
				ramblerString(serviceName));

			stream = temp.get();

			rambler::memory::MemoryManager::default_manager()->share_memory(stream);
		}

		TCPStream::~TCPStream(){
			rambler::memory::MemoryManager::default_manager()->release_memory(stream);
		}

		System::String^ TCPStream::getDomainName(){
			return dotNetString(stream->getDomainName());
		}

		System::String^ TCPStream::getServiceName(){
			return dotNetString(stream->getServiceName());
		}

		System::String^ TCPStream::getRemoteHostName(){
			return dotNetString(stream->getRemoteHostName());
		}

		System::UInt16 TCPStream::getRemotePortNumber(){
			return stream->getRemotePortNumber();
		}
	}
}
