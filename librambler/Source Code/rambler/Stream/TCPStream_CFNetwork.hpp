/**************************************************************************************************
 * @file    TCPStream_CFNetwork.hpp
 * @date    2015-01-27
 * @brief   <# Brief Description#>
 * @details <#Detailed Description#>
 **************************************************************************************************/

#pragma once

#include "rambler/Stream/TCPStream.hpp"

#include <CoreFoundation/CoreFoundation.h>
#include <Security/SecureTransport.h>

namespace rambler { namespace Stream {

    /**
     * A CFNetwork based implementation of TCPStream
     * @author Omar Stefan Evans
     * @date   2015-01-27
     */
    class TCPStream_CFNetwork : public TCPStream {
    public:

        /**
         * Constructs an object representing a TCP stream to a host at the given domain for a particular service.
         * @author  Omar Stefan Evans
         * @date    2015-01-27
         * @param   domainName the domain name
         * @param   serviceName either a service name for a SRV lookup or a port number
         */
        TCPStream_CFNetwork(String domainName, String serviceName);

        /**
         * Destructor
         * @author  Omar Stefan Evans
         * @date    2015-01-27
         * @details closes this stream if it is not already closed
         */
        virtual ~TCPStream_CFNetwork();

        virtual State getState() override;

        /**
         * Opens this stream.
         * @author  Omar Stefan Evans
         * @date    2015-01-27
         * @return  true on success, false on failure
         */
        virtual void open() override;

        /**
         * Secures this stream.
         * @author  Omar Stefan Evans
         * @date    2015-01-27
         * @return  true on success, false on failure
         */
        virtual void secure() override;

        /**
         * Closes this stream.
         * @author  Omar Stefan Evans
         * @date    2015-01-27
         */
        virtual void close() override;

        /**
         * Sends data over this stream
         * @author  Omar Stefan Evans
         * @date    2015-01-27
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

        bool connect();

        /**
         * For internal use only!
         * @author  Omar Stefan Evans
         * @date    2015-01-27
         * @details Use this to read data when this stream is open (but not secured).
         * @return  the data that was read
         */
        std::vector<UInt8> readData();

        /**
         * For internal use only!
         * @details Use this to read data when this stream is open and secured.
         * @return  the data that was read
         */
        std::vector<UInt8> readDataSecurely();

        /**
         * For internal use only!
         * @author  Omar Stefan Evans
         * @date    2015-01-27
         * @details Use this to write data when this stream is open (but not secured).
         * @param   data the data to write
         */
        void writeData(std::vector<UInt8> const & data);

        /**
         * For internal use only!
         * @author  Omar Stefan Evans
         * @date    2015-01-27
         * @details Use this to write data when this stream is open and secured.
         * @param   data the data to write
         */
        void writeDataSecurely(std::vector<UInt8> const & data);

        /* CFNetwork Specific Functions */
        static void inputStreamCallback(CFReadStreamRef inputStream,
                                        CFStreamEventType eventType,
                                        void *clientCallBackInfo);


        /* Secure Transport Specific Functions */

        static OSStatus secureTransportReadCallback (SSLConnectionRef connection,
                                                     void *data,
                                                     size_t *dataLength);

        static OSStatus secureTransportWriteCallback (SSLConnectionRef connection,
                                                      const void *data,
                                                      size_t *dataLength);

        /* CFNetwork Specific Data */
        CFReadStreamRef inputStream   { nullptr };
        CFWriteStreamRef outputStream { nullptr };

        /* Secure Transport Specific Data */
        SSLContextRef secureLayerContext { nullptr };


    };
    
}}