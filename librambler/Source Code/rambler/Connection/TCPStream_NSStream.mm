/**********************************************************************************************************************
 * @file    TCPStream_NSStream.hpp
 * @date    2014-12-17
 * @brief   <# Brief Description#>
 * @details <#Detailed Description#>
 **********************************************************************************************************************/

#include "rambler/Connection/TCPStream_NSStream.hpp"
#include "RamblerTCPStream.h"

namespace rambler { namespace Connection {

    StrongPointer<TCPConnection> TCPConnection::nativeTCPConnection(String domainName, String serviceName)
    {
        return memory::makeStrongPointer<TCPStream_NSStream>(domainName, serviceName);
    }

    TCPStream_NSStream::TCPStream_NSStream(String domainName, String serviceName)
    {
        auto delegate = [RamblerTCPStreamDelegate new];

        nativeStream = [[RamblerTCPStream alloc] initWithDomainName:[NSString stringWithUTF8String:domainName.c_str()]
                                                     serviceName:[NSString stringWithUTF8String:serviceName.c_str()]];

        delegate.openedEventHandler = [this](RamblerTCPStream *){
            this->handleOpenedEvent();
        };

        delegate.securedEventHandler = [this](RamblerTCPStream *) {
            this->handleSecuredEvent();
        };

        delegate.closedEventHandler = [this](RamblerTCPStream *) {
            this->handleClosedEvent();
        };

        delegate.hasDataEventHandler = [this](RamblerTCPStream *, NSData * data) {
            this->handleHasDataEvent(std::vector<UInt8>((const UInt8 *)data.bytes, (const UInt8 *)data.bytes + data.length));
        };

        delegate.failedToOpenEventHandler = [this](RamblerTCPStream *, NSError * error) {
            this->handleOpeningFailedEvent();
        };

        delegate.failedToSecureEventHandler = [this](RamblerTCPStream *, NSError * error) {
            this->handleSecuringFailedEvent();
        };

        nativeStream.delegate = delegate;
    }

    TCPStream_NSStream::~TCPStream_NSStream() {
        [nativeStream.delegate release];
        [nativeStream release];
    }

    Stream::State TCPStream_NSStream::getState() {
        return (Stream::State)nativeStream.status;
    }

    void TCPStream_NSStream::open()
    {
        return [nativeStream open];
    }

    void TCPStream_NSStream::secure() {
        return [nativeStream secure];
    }

    void TCPStream_NSStream::close()
    {
        [nativeStream close];
    }

    void TCPStream_NSStream::sendData(std::vector<UInt8> const & data)
    {
        [nativeStream writeData:[NSData dataWithBytes:data.data() length:data.size()]];
    }

    String TCPStream_NSStream::getDomainName() const {
        return String(nativeStream.domainName.UTF8String);
    }

    String TCPStream_NSStream::getServiceName() const {
        return String(nativeStream.serviceName.UTF8String);
    }

    String TCPStream_NSStream::getRemoteHostName() const {
        return String(nativeStream.remoteHostName.UTF8String);
    }

    UInt16 TCPStream_NSStream::getRemotePortNumber() const {
        return [nativeStream.remoteHostPort unsignedShortValue];
    }

}}