/**********************************************************************************************************************
 * @file    TCPStream_NSStream.hpp
 * @date    2014-12-17
 * @brief   <# Brief Description#>
 * @details <#Detailed Description#>
 **********************************************************************************************************************/

#pragma once

#include "rambler/Connection/TCPConnection.hpp"

#ifdef __OBJC__
#define OBJC_CLASS(name) @class name
#else
#define OBJC_CLASS(name) typedef struct objc_object name
#endif

OBJC_CLASS(RamblerTCPStream);

namespace rambler { namespace Connection {

    /**
     * A CFNetwork based implementation of TCP connection
     * @author Omar Stefan Evans
     * @date   2014-07-04
     */
    class TCPStream_NSStream : public TCPConnection {
    public:

        /**
         * Constructs an object representing a TCP connection to a host at the given domain for a particular service.
         * @author  Omar Stefan Evans
         * @date    2014-06-14
         * @param   domainName the domain name
         * @param   serviceName either a service name for a SRV lookup or a port number
         */
        TCPStream_NSStream(String domainName, String serviceName);

        /**
         * Destructor
         * @author  Omar Stefan Evans
         * @date    2014-06-14
         * @details closes this connection if it is not already closed
         */
        virtual ~TCPStream_NSStream();

        /**
         * Opens this connection.
         * @author  Omar Stefan Evans
         * @date    2014-06-14
         * @return  true on success, false on failure
         */
        virtual void open() override;

        /**
         * Secures this connection.
         * @author  Omar Stefan Evans
         * @date    2014-07-03
         * @return  true on success, false on failure
         */
        virtual void secure() override;

        /**
         * Closes this connection.
         * @author  Omar Stefan Evans
         * @date    2014-07-03
         */
        virtual void close() override;

        /**
         * Sends data over this connection
         * @author  Omar Stefan Evans
         * @date    2014-07-03
         * @param   data the data to send
         */
        virtual void sendData(std::vector<UInt8> const & data) override;

        virtual Stream::State getState();

        /**
         * Gets the domain name used to initialize this TCP connection.
         * @author  Omar Stefan Evans
         * @date    2014-07-02
         */
        virtual String getDomainName() const override;

        /**
         * Gets the service name used to initialize this TCP connection.
         * @author  Omar Stefan Evans
         * @date    2014-07-02
         */
        virtual String getServiceName() const override;

        /**
         * The remote host's actual name. This may be the same as the domain name if this connection was initialized
         * with a port number instead of a service name.
         * @author  Omar Stefan Evans
         * @date    2014-07-02
         */
        virtual String getRemoteHostName() const override;

        /**
         * The port of the remote host for this connection.
         * @author  Omar Stefan Evans
         * @date    2014-07-02
         */
        virtual UInt16 getRemotePortNumber() const override;

    private:

        /* Objective-C stream object */
        RamblerTCPStream * nativeStream;
    };
    
}}