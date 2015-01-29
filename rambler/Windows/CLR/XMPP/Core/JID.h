#pragma once


namespace Rambler {
	namespace XMPP {
		namespace Core{
			ref struct JID {
				JID(System::String^ localPart, System::String ^ domainPart, System::String^ resourcePart);

				System::Boolean isBareJID();
				System::Boolean isBareJIDWithLocalPart();
				System::Boolean isFullJID();
				System::Boolean isFullJIDWithLocalPart();
				System::Boolean isDomainJID();

			private:

				

			};
		}
	}
}