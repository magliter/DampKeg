//
//  RamblerInputStream.internal.h
//  Rambler
//
//  Created by Omar Evans on 2015/01/19.
//  Copyright (c) 2015 DampKeg. All rights reserved.
//

#import "RamblerInputStream.h"

@interface RamblerInputStream ()

@property(strong, readonly)  NSInputStream * wrappedStream;
@property(assign, readwrite) SSLContextRef tlsContext;
@property(assign, readwrite) RamblerStreamStatus status;
@property(strong, readwrite) NSError *error;

- (NSInteger)readRaw:(uint8_t *)buffer maxLength:(NSUInteger)length;

@end