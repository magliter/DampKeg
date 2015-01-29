// This is the main DLL file.

#include "stdafx.h"

#include "utilities.h"
#include "Client.h"

namespace Rambler { namespace XMPP { namespace IM { namespace Client {

	Client::Client(System::String^ username) {
		client = new rambler::XMPP::IM::Client::Client(ramblerString(username));
	}

	Client::~Client() {
		delete client;
	}

	void Client::start() {
		client->start();
	}

}}}}