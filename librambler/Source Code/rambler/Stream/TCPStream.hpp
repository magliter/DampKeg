/**************************************************************************************************
 * @file    TCPStream.hpp
 * @date    2015-01-27
 * @brief   <# Brief Description#>
 * @details <#Detailed Description#>
 **************************************************************************************************/

#pragma once

#include "rambler/Stream/BidirectionalStream.hpp"

namespace rambler { namespace Stream {

    /**
     * An abstract object representing a TCP stream
     * @author Omar Stefan Evans
     * @date   2014-07-04
     */
    class TCPStream : public Stream::BidirectionalStream<UInt8> {
    public:

        /**
         * Returns a strong pointer to a native object (i.e., a concrete one instead of an abstract one) representing a
         * TCP stream to a host at the given domain for a particular service.
         * @author  Omar Stefan Evans
         * @date    2015-01-27
         * @param   domainName the domain name
         * @param   serviceName either a service name for a SRV lookup or a port number
         * @return  a strong pointer to a "native" TCP stream object
         */
        static StrongPointer<TCPStream> nativeTCPStream(String domainName, String serviceName);

        /**
         * Destructor
         * @details The destructor in subclasses should close the stream if it is not already closed.
         */
        virtual ~TCPStream() = default;

        /**
         * Called when open() fails.
         * @author  Omar Stefan Evans
         * @date    2014-07-04
         */
        virtual void handleOpeningFailedEvent() override;

        /**
         * Called when secure() fails. This stream will be closed.
         * @author  Omar Stefan Evans
         * @date    2014-07-04
         */
        virtual void handleSecuringFailedEvent() override;

        /**
         * Gets the domain name used to initialize this TCP stream.
         * @author  Omar Stefan Evans
         * @date    2015-01-27
         */
        virtual String getDomainName() const = 0;

        /**
         * Gets the service name used to initialize this TCP stream.
         * @author  Omar Stefan Evans
         * @date    2015-01-27
         */
        virtual String getServiceName() const = 0;

        /**
         * The remote host's actual name. This may be the same as the domain name if this TCP stream
         * was initialized with a port number instead of a service name.
         * @author  Omar Stefan Evans
         * @date    2015-01-27
         */
        virtual String getRemoteHostName() const = 0;

        /**
         * The port of the remote host for this stream.
         * @author  Omar Stefan Evans
         * @date    2015-01-27
         */
        virtual UInt16 getRemotePortNumber() const = 0;
    };

}}