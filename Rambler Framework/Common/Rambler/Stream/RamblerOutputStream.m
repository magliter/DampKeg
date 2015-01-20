//
//  RamblerOutputStream.m
//  Rambler
//
//  Created by Omar Evans on 2015/01/19.
//  Copyright (c) 2015 DampKeg. All rights reserved.
//

#import "RamblerOutputStream.internal.h"

@implementation RamblerOutputStream {
    id<NSStreamDelegate> _delegate;
}

- (instancetype)initWithOutputStream:(NSOutputStream *)outputStream {
    self = [super init];

    if (self) {
        _delegate = self;
        _status = RamblerStreamStatusNotOpen;
        _wrappedStream = outputStream;
        _wrappedStream.delegate = self;
    }

    return self;
}

- (void)secureWithContext:(SSLContextRef)context {
    if (self.isSecure) {
        return;
    }

    self.tlsContext = context;

    if (self.isSecure) {
        self.status = RamblerStreamStatusOpenAndSecured;
    }
}

- (BOOL)isSecure {
    return self.tlsContext != nil;
}

#pragma mark NSStream Methods

#pragma mark Configuring Streams

- (id<NSStreamDelegate>)delegate {
    return _delegate;
}

- (void)setDelegate:(id<NSStreamDelegate>)delegate {
    _delegate = delegate == nil ? self : delegate;
}

- (id)propertyForKey:(NSString *)key {
    return [self.wrappedStream propertyForKey:key];
}

- (BOOL)setProperty:(id)property forKey:(NSString *)key {
    return [self.wrappedStream setProperty:property forKey:key];
}

#pragma mark Using Streams

- (void)open {
    self.status = RamblerStreamStatusOpening;
    [self.wrappedStream open];
}

- (void)close {
    self.status = RamblerStreamStatusClosed;
    [self.wrappedStream close];
}

#pragma mark Managing Run Loops

- (void)scheduleInRunLoop:(NSRunLoop *)aRunLoop forMode:(NSString *)mode {
    [self.wrappedStream scheduleInRunLoop:aRunLoop forMode:mode];
}

- (void)removeFromRunLoop:(NSRunLoop *)aRunLoop forMode:(NSString *)mode {
    [self.wrappedStream removeFromRunLoop:aRunLoop forMode:mode];
}

#pragma mark Getting Stream Information

- (NSStreamStatus)streamStatus {
    return self.status & 0x0F;
}

- (NSError *)streamError {
    return self.error == nil ? self.wrappedStream.streamError : self.error;
}

#pragma mark NSInputStream Methods

- (NSInteger)write:(const uint8_t *)buffer maxLength:(NSUInteger)length {
    return self.isSecure ? [self writeSecurely:buffer maxLength:length] : [self writeInsecurely:buffer maxLength:length];
}

- (BOOL)hasSpaceAvailable {
    return self.wrappedStream.hasSpaceAvailable;
}

#pragma mark NSStreamDelegate Methods

- (void)stream:(NSStream *)aStream handleEvent:(NSStreamEvent)eventCode {
    if (aStream == self) {
        return;
    }

    switch (eventCode) {
        case NSStreamEventOpenCompleted:
            self.status = RamblerStreamStatusOpen;
            break;
        case NSStreamEventErrorOccurred:
            self.status = RamblerStreamStatusError;
            break;
        case NSStreamEventEndEncountered:
            self.status = RamblerStreamStatusAtEnd;
            break;
        default:
            break;
    }

    [self.delegate stream:self handleEvent:eventCode];
}

#pragma mark RamblerInputStream Internal Methods

- (NSInteger)writeInsecurely:(const uint8_t *)buffer maxLength:(NSUInteger)length {

    NSInteger bytesWritten;
    RamblerStreamStatus prevStatus = self.status;

    self.status = RamblerStreamStatusWriting;
    bytesWritten = [self writeRaw:buffer maxLength:length];
    self.status = prevStatus;

    return bytesWritten;
}

- (NSInteger)writeSecurely:(const uint8_t *)buffer maxLength:(NSUInteger)length {

    NSUInteger bytesWritten;
    RamblerStreamStatus prevStatus = self.status;

    self.status = RamblerStreamStatusWritingSecurely;

    OSStatus status = SSLWrite(self.tlsContext, buffer, length, &bytesWritten);
    
    if (status == noErr) {
        self.status = prevStatus;
    } else {
        self.error = [NSError errorWithDomain:NSOSStatusErrorDomain code:status userInfo:nil];
        self.status = RamblerStreamStatusError;
    }

    return bytesWritten;
}

- (NSInteger)writeRaw:(const uint8_t *)buffer maxLength:(NSUInteger)length {
    return [self.wrappedStream write:buffer maxLength:length];
}

@end
