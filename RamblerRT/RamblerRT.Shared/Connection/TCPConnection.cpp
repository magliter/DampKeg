#include "TCPConnection.hpp"
#include "State.hpp"
namespace Rambler{ namespace Connection{

	TCPConnection::TCPConnection(){
		theConnection = new rambler::Connection::WindowsRuntimeBasedConnection();
	}

	TCPConnection::TCPConnection(Platform::String ^ host, Platform::String ^ service){

		std::string stringHost, stringService;
	
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		stringHost = converter.to_bytes(host->Data());
		stringService = converter.to_bytes(service->Data());
		theConnection = new
			rambler::Connection::WindowsRuntimeBasedConnection(stringHost, stringService);
	}
	TCPConnection::~TCPConnection(){
		//IDK WHAT GOES HERE
	}
	Platform::Boolean TCPConnection::open(){
		if (theConnection->open()){
			return true;
		}
		else{
			return false;
		}
	}

	void TCPConnection::close(){
		theConnection->close();
	}


	void TCPConnection::sendData(Platform::String ^ data){
		std::string toSend;

		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		toSend  = converter.to_bytes(data->Data());		
		theConnection->sendData(toSend);
	}



	Stream::State TCPConnection::getState() {
			
		switch (theConnection->getState()){
			case rambler::Connection::State::NotConnected:
				return Stream::State::Closed;
			case rambler::Connection::State::Connecting:
				return Stream::State::Opening;
			case rambler::Connection::State::Connected:
					return Stream::State::Open;
			case rambler::Connection::State::SecurelyConnected:
				return Stream::State::OpenAndSecured;
			}
	}

	Platform::String ^ TCPConnection::getConnectedHost() {
		std::string connectedHost = theConnection->getConnectedHost();
		
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring title_UTF16 = converter.from_bytes(connectedHost.c_str());
		String^ pString = ref new String(title_UTF16.c_str());


		return pString;

	}
	
	uint16 TCPConnection::getConnectedPort() {
		return theConnection->getConnectedPort();
	}
	
	


}}