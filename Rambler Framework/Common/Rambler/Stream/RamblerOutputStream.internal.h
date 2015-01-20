//
//  RamblerOutputStream.internal.h
//  Rambler
//
//  Created by Omar Evans on 2015/01/19.
//  Copyright (c) 2015 DampKeg. All rights reserved.
//

#import "RamblerOutputStream.h"

@interface RamblerOutputStream ()

@property(strong, readonly)  NSOutputStream * wrappedStream;
@property(assign, readwrite) SSLContextRef tlsContext;
@property(assign, readwrite) RamblerStreamStatus status;
@property(strong, readwrite) NSError *error;

- (NSInteger)writeRaw:(const uint8_t *)buffer maxLength:(NSUInteger)length;

@end