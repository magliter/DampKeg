/**********************************************************************************************************************
 * @file    TCPStream_CFNetwork.hpp
 * @date    2014-07-04
 * @brief   <# Brief Description#>
 * @details <#Detailed Description#>
 **********************************************************************************************************************/

#include "rambler/Stream/TCPStream_CFNetwork.hpp"
#include "rambler/DNS/SRVRecordResolver_CFNetwork.h"

namespace rambler { namespace Stream {

    StrongPointer<TCPStream> TCPStream::nativeTCPStream(String domainName, String serviceName)
    {
        return std::make_shared<TCPStream_CFNetwork>(domainName, serviceName);
    }

    TCPStream_CFNetwork::TCPStream_CFNetwork(String domainName, String serviceName) : domainName(domainName), serviceName(serviceName)
    {
        /* Nothing to do here */
    }

    TCPStream_CFNetwork::~TCPStream_CFNetwork() {
        close();
    }

    bool TCPStream_CFNetwork::connect() {
        state = Stream::State::Opening;

        CFStringRef host_cfstring = CFStringCreateWithCString(kCFAllocatorDefault,
                                                              remoteHostName.c_str(),
                                                              kCFStringEncodingUTF8);

        CFStreamCreatePairWithSocketToHost(kCFAllocatorDefault,
                                           host_cfstring,
                                           remotePortNumber,
                                           &inputStream,
                                           &outputStream);
        CFRelease(host_cfstring);

        CFStreamClientContext inputStreamContext;
        memset(&inputStreamContext, 0, sizeof(inputStreamContext));
        inputStreamContext.info = reinterpret_cast<void *>(this);

        CFReadStreamSetClient(inputStream,
                              kCFStreamEventHasBytesAvailable |
                              kCFStreamEventEndEncountered |
                              kCFStreamEventErrorOccurred,
                              inputStreamCallback,
                              &inputStreamContext);

        CFReadStreamScheduleWithRunLoop(inputStream, CFRunLoopGetMain(), kCFRunLoopDefaultMode);

        if (CFReadStreamOpen(inputStream) && CFWriteStreamOpen(outputStream)) {
            state = Stream::State::Open;
            return true;
        } else {
            CFRelease(outputStream);
            CFRelease(inputStream);
            outputStream = nullptr;
            inputStream = nullptr;

            state = Stream::State::NotOpen;
            return false;
        }
    }

    State TCPStream_CFNetwork::getState() {
        return state;
    }

    void TCPStream_CFNetwork::open()
    {
        if (!serviceName.empty() && serviceName.find_first_not_of("0123456789") == String::npos) {
            remoteHostName = domainName;
            remotePortNumber = atoi(serviceName.c_str());

            if (connect()) {
                handleOpenedEvent();
            }
            handleOpeningFailedEvent();
        }

        DNS::SRVRecordResolver resolver(serviceName, "tcp", domainName);
        resolver.start();
        while (!resolver.isDone()) {
            CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0, false);
        }

        //The SRV lookup has completed. Get the results and try to connect
        auto results = DNS::SRVRecordResolver::prioritizeResults(resolver.getResults());

        for (auto result : results) {
            remoteHostName = result.target;
            remotePortNumber = result.port;
            if (connect()) {
                return handleOpenedEvent();
            }
        }

        return handleOpeningFailedEvent();
    }

    void TCPStream_CFNetwork::secure() {
        if (state != Stream::State::Open) {
            return;
        }

        state = Stream::State::OpenAndSecuring;

        OSStatus status = noErr;

        secureLayerContext = SSLCreateContext(kCFAllocatorDefault, kSSLClientSide, kSSLStreamType);

        status = SSLSetConnection(secureLayerContext, reinterpret_cast<SSLConnectionRef>(this));
        if (status != noErr) {
            handleSecuringFailedEvent();
            return;
        }

        status = SSLSetIOFuncs(secureLayerContext,
                                        TCPStream_CFNetwork::secureTransportReadCallback,
                                        TCPStream_CFNetwork::secureTransportWriteCallback);
        if (status != noErr) {
            handleSecuringFailedEvent();
            return;
        }

        status = SSLSetPeerDomainName(secureLayerContext, domainName.c_str(), domainName.size());
        if (status != noErr) {
            handleSecuringFailedEvent();
            return;
        }

        do {
            status = SSLHandshake(secureLayerContext);
        } while (status == errSSLWouldBlock);

        switch (status) {
            default:
                //TODO: Log this!
                handleSecuringFailedEvent();
                return;
            case errSSLFatalAlert:
                handleSecuringFailedEvent();
                return;
            case errSSLUnknownRootCert:
            case errSSLNoRootCert:
            case errSSLCertExpired:
            case errSSLXCertChainInvalid:
                //TODO: The UI (if any) should probably ask what to do instead of simply failing.
                handleSecuringFailedEvent();
                return;
            case errSSLClientCertRequested:
                //TODO: The App should supply this.  Add an event handler.
                handleSecuringFailedEvent();
                return;
            case noErr:
                state = Stream::State::OpenAndSecured;
                handleSecuredEvent();
                return;
        }
    }

    void TCPStream_CFNetwork::close()
    {
        if (state == Stream::State::Closed) {
            return;
        }

        state = Stream::State::Closed;

        if (secureLayerContext != nullptr) {
            SSLClose(secureLayerContext);
        }

        if (inputStream != nullptr) {
            CFReadStreamUnscheduleFromRunLoop(inputStream, CFRunLoopGetMain(), kCFRunLoopDefaultMode);
            CFReadStreamClose(inputStream);
            CFRelease(inputStream);
            inputStream = nullptr;
        }

        if (outputStream != nullptr) {
            CFWriteStreamClose(outputStream);
            CFRelease(outputStream);
            outputStream= nullptr;
        }

        if (secureLayerContext != nullptr) {
            CFRelease(secureLayerContext);
            secureLayerContext = nullptr;
        }
        handleClosedEvent();
    }

    void TCPStream_CFNetwork::sendData(std::vector<UInt8> const & data)
    {
        switch (state) {
            case Stream::State::Open:
                writeData(data);
                break;
            case Stream::State::OpenAndSecured:
                writeDataSecurely(data);
                break;
            default:
                break;
        }
    }

    String TCPStream_CFNetwork::getDomainName() const {
        return domainName;
    }

    String TCPStream_CFNetwork::getServiceName() const {
        return serviceName;
    }

    String TCPStream_CFNetwork::getRemoteHostName() const {
        return remoteHostName;
    }

    UInt16 TCPStream_CFNetwork::getRemotePortNumber() const {
        return remotePortNumber;
    }

#pragma mark Private Member Functions

    std::vector<UInt8> TCPStream_CFNetwork::readData()
    {
        UInt8 buffer[1024];
        std::vector<UInt8> data;

        while (CFReadStreamHasBytesAvailable(inputStream)) {
            CFIndex bytesRead = CFReadStreamRead(inputStream, buffer, sizeof(buffer) - 1);
            if (bytesRead == -1) {
                break;
            }

            data.reserve((data.size() + bytesRead));
            data.insert(data.end(), buffer, buffer + bytesRead);
        }

        return data;
    }

    std::vector<UInt8> TCPStream_CFNetwork::readDataSecurely()
    {
        UInt8 buffer[1024];
        std::vector<UInt8> data;

        while (CFReadStreamHasBytesAvailable(inputStream)) {
            size_t bytesRead = 0;
            OSStatus status = SSLRead(secureLayerContext, buffer, sizeof(buffer), &bytesRead);
            //TODO: Handle any errors here

            data.reserve((data.size() + bytesRead));
            data.insert(data.end(), buffer, buffer + bytesRead);
        }

        return data;
    }

    void TCPStream_CFNetwork::writeData(const std::vector<UInt8> &data)
    {
        CFWriteStreamWrite(outputStream, data.data(), data.size());
    }

    void TCPStream_CFNetwork::writeDataSecurely(const std::vector<UInt8> &data)
    {
        UInt8 const *buffer = data.data();
        size_t bytesToWrite = data.size();

        while (bytesToWrite != 0) {
            size_t bytesWritten = 0;
            OSStatus status = SSLWrite(secureLayerContext, buffer, bytesToWrite, &bytesWritten);
            //TODO: Handle any errors here

            buffer += bytesWritten;
            bytesToWrite -= bytesWritten;
        }
    }

    /* Static functions */

    void TCPStream_CFNetwork::inputStreamCallback(CFReadStreamRef inputStream,
                                                       CFStreamEventType eventType,
                                                       void *clientCallBackInfo)
    {
        TCPStream_CFNetwork *theStream = reinterpret_cast<TCPStream_CFNetwork *>(clientCallBackInfo);

        auto attemptToReadData = [theStream]() -> std::vector<UInt8> {

            switch (theStream->state) {
                case Stream::State::Open:
                    return theStream->readData();
                case Stream::State::OpenAndSecured:
                    return theStream->readDataSecurely();
                default:
                    return {};
            }

        };


        std::vector<UInt8> data;

        switch (eventType) {
            case kCFStreamEventHasBytesAvailable:
                data = attemptToReadData();
                if (data.size() > 0) {
                    theStream->handleHasDataEvent(data);
                }
                break;
            case kCFStreamEventEndEncountered:
                theStream->close();
                break;
            case kCFStreamEventErrorOccurred:
                theStream->close();
                break;
        }
    }

    OSStatus TCPStream_CFNetwork::secureTransportReadCallback (SSLConnectionRef connection,
                                                 void *data,
                                                 size_t *dataLength)
    {
        auto actualStream = reinterpret_cast<TCPStream_CFNetwork const *>(connection);

        if (!CFReadStreamHasBytesAvailable(actualStream->inputStream)) {
            *dataLength = 0;
            return errSSLWouldBlock;
        }

        auto bytesRead = CFReadStreamRead(actualStream->inputStream, reinterpret_cast<UInt8 *>(data), *dataLength);

        if (bytesRead < *dataLength) {
            *dataLength = bytesRead;
            return errSSLWouldBlock;
        }

        return noErr;
    }

    OSStatus TCPStream_CFNetwork::secureTransportWriteCallback (SSLConnectionRef connection,
                                                  const void *data,
                                                  size_t *dataLength)
    {
        auto actualStream = reinterpret_cast<TCPStream_CFNetwork const *>(connection);

        if (!CFWriteStreamCanAcceptBytes(actualStream->outputStream)) {
            *dataLength = 0;
            return errSSLWouldBlock;
        }

        auto bytesWritten = CFWriteStreamWrite(actualStream->outputStream, reinterpret_cast<UInt8 const *>(data), *dataLength);

        if (bytesWritten < *dataLength) {
            *dataLength = bytesWritten;
            return errSSLWouldBlock;
        }

        return noErr;
    }

}}