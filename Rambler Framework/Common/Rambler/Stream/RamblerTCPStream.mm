//
//  TCPStream.m
//  Rambler
//
//  Created by Omar Evans on 2015/01/18.
//  Copyright (c) 2015 DampKeg. All rights reserved.
//

#import "RamblerTCPStream.internal.h"
#include "rambler/DNS/SRVRecordResolver_CFNetwork.h"

@implementation RamblerTCPStream

- (instancetype)initWithDomainName:(NSString *)domainName serviceName:(NSString *)serviceName {
    self = [super init];
    if (!domainName || !serviceName) {
        self = nil;
    }

    if (self) {
        _domainName = domainName;
        _serviceName = serviceName;
        _numberFormatter = [NSNumberFormatter new];
        _numberFormatter.locale = [NSLocale autoupdatingCurrentLocale];
        _numberFormatter.allowsFloats = NO;
        _numberFormatter.numberStyle = NSNumberFormatterDecimalStyle;
        _numberFormatter.minimum = @0x0;
        _numberFormatter.maximum = @0xFFFF;
    }

    return self;
}

- (void)dealloc {
    [self close];
}

- (BOOL)connect {
    self.status = RamblerStreamStatus::RamblerStreamStatusOpening;

    CFStringRef remoteHostName = (__bridge CFStringRef)self.remoteHostName;

    CFReadStreamRef readStream;
    CFWriteStreamRef writeStream;

    CFStreamCreatePairWithSocketToHost(kCFAllocatorDefault,
                                       remoteHostName,
                                       self.remoteHostPort.unsignedShortValue,
                                       &readStream,
                                       &writeStream);

    NSInputStream *inputStream = (NSInputStream *)CFBridgingRelease(readStream);
    NSOutputStream *outputStream = (NSOutputStream *)CFBridgingRelease(writeStream);

    self.inputStream = [[RamblerInputStream alloc] initWithInputStream:inputStream];
    self.outputStream = [[RamblerOutputStream alloc] initWithOutputStream:outputStream];

    self.inputStream.delegate = self;
    self.outputStream.delegate = self;

    [self.inputStream scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    [self.outputStream scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];

    [self.inputStream open];
    [self.outputStream open];

    while (self.inputStream.streamStatus == NSStreamStatusOpening ||
           self.outputStream.streamStatus == NSStreamStatusOpening) {
        [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode beforeDate:[NSDate distantFuture]];
    }

    if (self.inputStream.streamStatus == NSStreamStatusOpen &&
        self.outputStream.streamStatus == NSStreamStatusOpen) {
        self.status = RamblerStreamStatusOpen;
        return YES;
    }

    self.inputStream = nil;
    self.outputStream = nil;
    self.status = RamblerStreamStatusNotOpen;
    return NO;
}

- (void)open {
    if (self.serviceName.length != 0 && [self.serviceName rangeOfCharacterFromSet:[[NSCharacterSet decimalDigitCharacterSet]invertedSet]].location == NSNotFound) {
        self.remoteHostName = self.domainName;
        self.remoteHostPort = [self.numberFormatter numberFromString:self.serviceName];

        if ([self connect]) {
            return [self.delegate streamDidOpen:self];
        }
        //TODO: Supply real error information
        return [self.delegate stream:self didFailToOpenWithError:nil];

    }

    rambler::DNS::SRVRecordResolver resolver(self.serviceName.UTF8String, "tcp", self.domainName.UTF8String);
    resolver.start();
    while (!resolver.isDone()) {
        CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0, false);
    }

    //The SRV lookup has completed. Get the results and try to connect
    auto results = rambler::DNS::SRVRecordResolver::prioritizeResults(resolver.getResults());

    for (auto result : results) {
        self.remoteHostName = [NSString stringWithUTF8String:result.target.c_str()];
        self.remoteHostPort = [NSNumber numberWithUnsignedShort:result.port];

        if ([self connect]) {
            return [self.delegate streamDidOpen:self];
        }
    }

    //TODO: Supply real error information
    return [self.delegate stream:self didFailToOpenWithError:nil];
}

- (void)close {
    if (self.status == RamblerStreamStatusClosed) {
        return;
    }
    self.status = RamblerStreamStatusClosed;

    if (self.tlsContext) {
        SSLClose(self.tlsContext);
    }

    [self.outputStream close];
    [self.inputStream close];

    CFRelease(self.tlsContext);
}

- (void)secure {
    if (self.status != RamblerStreamStatusOpen) {
        return;
    }

    self.status = RamblerStreamStatusOpenAndSecuring;

    OSStatus status = noErr;

    self.tlsContext = SSLCreateContext(kCFAllocatorDefault, kSSLClientSide, kSSLStreamType);

    status = SSLSetConnection(self.tlsContext, (__bridge SSLConnectionRef)(self));
    if (status != noErr) {
        self.error = [[NSError alloc] initWithDomain:NSOSStatusErrorDomain code:status userInfo:nil];
        return [self.delegate stream:self didFailToSecureWithError: self.error];
    }

    status = SSLSetIOFuncs(self.tlsContext,
                           secureTransportReadCallback,
                           secureTransportWriteCallback);
    if (status != noErr) {
        self.error = [[NSError alloc] initWithDomain:NSOSStatusErrorDomain code:status userInfo:nil];
        return [self.delegate stream:self didFailToSecureWithError: self.error];
    }

    status = SSLSetPeerDomainName(self.tlsContext, self.domainName.UTF8String, self.domainName.length);
    if (status != noErr) {
        self.error = [[NSError alloc] initWithDomain:NSOSStatusErrorDomain code:status userInfo:nil];
        return [self.delegate stream:self didFailToSecureWithError: self.error];
    }

    do {
        status = SSLHandshake(self.tlsContext);
    } while (status == errSSLWouldBlock);

    switch (status) {
        default:
            self.status = RamblerStreamStatusError;
            self.error = [[NSError alloc] initWithDomain:NSOSStatusErrorDomain code:status userInfo:nil];
            return [self.delegate stream:self didFailToSecureWithError: self.error];
        case errSSLFatalAlert:
            self.status = RamblerStreamStatusError;
            self.error = [[NSError alloc] initWithDomain:NSOSStatusErrorDomain code:status userInfo:nil];
            return [self.delegate stream:self didFailToSecureWithError: self.error];

        case errSSLUnknownRootCert:
        case errSSLNoRootCert:
        case errSSLCertExpired:
        case errSSLXCertChainInvalid:
            //TODO: The UI (if any) should probably ask what to do instead of simply failing.
            self.status = RamblerStreamStatusError;
            self.error = [[NSError alloc] initWithDomain:NSOSStatusErrorDomain code:status userInfo:nil];
            return [self.delegate stream:self didFailToSecureWithError: self.error];

        case errSSLClientCertRequested:
            //TODO: The App should supply this.  Add an event handler.
            self.status = RamblerStreamStatusError;
            self.error = [[NSError alloc] initWithDomain:NSOSStatusErrorDomain code:status userInfo:nil];
            return [self.delegate stream:self didFailToSecureWithError: self.error];

        case noErr:
            [(RamblerInputStream *)self.inputStream secureWithContext: self.tlsContext];
            [(RamblerOutputStream *)self.outputStream secureWithContext: self.tlsContext];
            self.status = RamblerStreamStatusOpenAndSecured;
            return [self.delegate streamDidSecure:self];
    }
}

- (void)writeData:(NSData *)data {
    [self.outputStream write:(const uint8_t *)data.bytes maxLength:data.length];
}

- (void)stream:(NSStream *)aStream handleEvent:(NSStreamEvent)eventCode {
    switch (eventCode) {
        case NSStreamEventHasBytesAvailable:
            if (self.status != RamblerStreamStatusOpen && self.status != RamblerStreamStatusOpenAndSecured) {
                return;
            }
            if ([aStream isKindOfClass:[NSInputStream class]]) {
                NSInputStream *inputStream = (NSInputStream *)aStream;

                uint8_t buffer[4096] { 0 };
                NSMutableData *data = [[NSMutableData alloc] initWithCapacity:4096];

                while (inputStream.hasBytesAvailable) {
                    NSInteger bytesRead = [inputStream read:buffer maxLength:sizeof(buffer)];
                    if (bytesRead < 0) {
                        NSLog(@"An instance of RamblerTCPStream encountered an error occured while reading from its input stream: (%@, %@)", self, inputStream);
                        break;
                    }
                    [data appendBytes:buffer length:bytesRead];
                }

                [self.delegate stream:self hasData:data];
            }
            break;
        case NSStreamEventErrorOccurred:
            NSLog(@"An instance of RamblerTCPStream encountered an error with a stream (%@, %@)", self, aStream);
            break;
        default:
            break;
    }
}

static OSStatus secureTransportReadCallback(SSLConnectionRef connection, void *data, size_t *dataLength)
{
    auto actualConnection = (__bridge RamblerTCPStream *)(connection);

    if (!actualConnection.inputStream.hasBytesAvailable) {
        *dataLength = 0;
        return errSSLWouldBlock;
    }

    NSInteger bytesRead = [(RamblerInputStream *)actualConnection.inputStream readRaw:(uint8_t *)data maxLength:*dataLength];
    if (bytesRead < *dataLength) {
        *dataLength = bytesRead;
        return errSSLWouldBlock;
    }

    return noErr;
}

static OSStatus secureTransportWriteCallback(SSLConnectionRef connection, const void *data, size_t *dataLength)
{
    auto actualConnection = (__bridge RamblerTCPStream *)(connection);

    if (!actualConnection.outputStream.hasSpaceAvailable) {
        *dataLength = 0;
        return errSSLWouldBlock;
    }

    NSInteger bytesWritten = [(RamblerOutputStream *)actualConnection.outputStream writeRaw:(uint8_t *)data maxLength:*dataLength];

    if (bytesWritten < *dataLength) {
        *dataLength = bytesWritten;
        return errSSLWouldBlock;
    }
    
    return noErr;
}


@end
