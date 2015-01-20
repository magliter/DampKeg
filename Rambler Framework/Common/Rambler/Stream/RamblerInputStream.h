//
//  RamblerInputStream.h
//  Rambler
//
//  Created by Omar Evans on 2015/01/19.
//  Copyright (c) 2015 DampKeg. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Security/SecureTransport.h>
#import "RamblerStreamStatus.h"

@interface RamblerInputStream : NSInputStream<NSStreamDelegate>

@property(assign, readonly) BOOL isSecure;
@property(assign, readonly) RamblerStreamStatus status;
@property(strong, readonly) NSError *error;

- (instancetype)initWithInputStream:(NSInputStream *)inputStream;
- (void)secureWithContext:(SSLContextRef)context;

#pragma mark NSStream Methods

#pragma mark Configuring Streams

- (id<NSStreamDelegate>)delegate;
- (void)setDelegate:(id<NSStreamDelegate>)delegate;
- (id)propertyForKey:(NSString *)key;
- (BOOL)setProperty:(id)property forKey:(NSString *)key;

#pragma mark Using Streams

- (void)open;
- (void)close;

#pragma mark Managing Run Loops

- (void)scheduleInRunLoop:(NSRunLoop *)aRunLoop forMode:(NSString *)mode;
- (void)removeFromRunLoop:(NSRunLoop *)aRunLoop forMode:(NSString *)mode;

#pragma mark Getting Stream Information

- (NSStreamStatus)streamStatus;
- (NSError *)streamError;

#pragma mark NSInputStreamMethods

- (NSInteger)read:(uint8_t *)buffer maxLength:(NSUInteger)length;

- (BOOL)getBuffer:(uint8_t **)buffer length:(NSUInteger *)length;

- (BOOL)hasBytesAvailable;

@end
