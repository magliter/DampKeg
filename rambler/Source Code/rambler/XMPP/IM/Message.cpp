/**********************************************************************************************************************
 * @file    rambler/XMPP/IM/Client/Message.cpp
 * @date    2014-12-17
 * @brief   <# Brief Description#>
 * @details <#Detailed Description#>
 **********************************************************************************************************************/

#include "rambler/XMPP/IM/Message.hpp"

namespace rambler { namespace XMPP { namespace IM {

    StrongPointer<Message const> Message::createMessage(StrongPointer<JID const> const sender,
                                                        StrongPointer<JID const> const recipient,
                                                        String                   const thread,
                                                        String                   const subject,
                                                        String                   const body,
                                                        String                   const timestamp,
                                                        String                   const uniqueID)
    {
        if (!sender || !recipient || body.empty()) {
            return nullptr;
        }

        return makeStrongPointer<Message const>(sender, recipient, thread, subject, body, timestamp, uniqueID);
    }

    String Message::description() const
    {
        String description = "[ Message ]\n";

        description += "ID: ";
        description += uniqueID;
        description += "\n";

        description += "Timestamp: ";
        description += timestamp;
        description += "\n";

        description += "From: ";
        description += sender->description;
        description += "\n";

        description += "To: ";
        description += recipient->description;
        description += "\n";

        if (!thread.empty()) {
            description += "Thread: ";
            description += thread;
            description += "\n";
        }

        if (!subject.empty()) {
            description += "Subject: ";
            description += subject;
            description += "\n";
        }

        description += "Body: ";
        description += body;
        description += "\n";

        return description;
    }


    Message::Message(StrongPointer<JID const> const sender,
                     StrongPointer<JID const> const recipient,
                     String                   const thread,
                     String                   const subject,
                     String                   const body,
                     String                   const timestamp,
                     String                   const uniqueID)
    : sender(sender), recipient(recipient), thread(thread), subject(subject), body(body), timestamp(timestamp), uniqueID(uniqueID)
    {
        /* Nothing to do here */
    }

}}}