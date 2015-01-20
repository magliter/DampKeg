//
//  RamblerInputStream.m
//  Rambler
//
//  Created by Omar Evans on 2015/01/19.
//  Copyright (c) 2015 DampKeg. All rights reserved.
//

#import "RamblerInputStream.internal.h"

@implementation RamblerInputStream {
    id<NSStreamDelegate> _delegate;
}

- (instancetype)initWithInputStream:(NSInputStream *)inputStream {
    self = [super init];

    if (self) {
        _delegate = self;
        _status = RamblerStreamStatusNotOpen;
        _wrappedStream = inputStream;
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

- (NSInteger)read:(uint8_t *)buffer maxLength:(NSUInteger)length {
    return self.isSecure ? [self readSecurely:buffer maxLength:length] : [self readInsecurely:buffer maxLength:length];
}

- (BOOL)getBuffer:(uint8_t **)buffer length:(NSUInteger *)length {
    return NO;
}

- (BOOL)hasBytesAvailable {
    return self.wrappedStream.hasBytesAvailable;
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

- (NSInteger)readInsecurely:(uint8_t *)buffer maxLength:(NSUInteger)length {

    NSInteger bytesRead;
    RamblerStreamStatus prevStatus = self.status;

    self.status = RamblerStreamStatusReading;
    bytesRead = [self readRaw:buffer maxLength:length];
    self.status = prevStatus;

    return bytesRead;
}

- (NSInteger)readSecurely:(uint8_t *)buffer maxLength:(NSUInteger)length {

    NSUInteger bytesRead;
    RamblerStreamStatus prevStatus = self.status;

    self.status = RamblerStreamStatusReadingSecurely;

    OSStatus status = SSLRead(self.tlsContext, buffer, length, &bytesRead);

    if (status == noErr) {
        self.status = prevStatus;
    } else {
        self.error = [NSError errorWithDomain:NSOSStatusErrorDomain code:status userInfo:nil];
        self.status = RamblerStreamStatusError;
    }
    
    return bytesRead;
}

- (NSInteger)readRaw:(uint8_t *)buffer maxLength:(NSUInteger)length {
    return [self.wrappedStream read:buffer maxLength:length];
}

@end