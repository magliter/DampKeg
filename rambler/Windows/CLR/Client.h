//2015-01-28

#pragma once

#include "rambler\XMPP\IM\Client\Client.hpp"

using namespace System;

namespace Rambler {
	namespace XMPP {
		namespace IM {
			namespace Client {
				public ref class Client
				{
				public:
					Client(System::String^ username);
					~Client();

					void start();

				private:
					rambler::XMPP::IM::Client::Client *client;
				};
			}
		}
	}

}
