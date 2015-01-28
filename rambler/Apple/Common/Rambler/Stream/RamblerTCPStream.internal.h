//
//  TCPStream.internal.h
//  Rambler
//
//  Created by Omar Evans on 2015/01/18.
//  Copyright (c) 2015 DampKeg. All rights reserved.
//

#import "RamblerTCPStream.h"
#import "RamblerInputStream.internal.h"
#import "RamblerOutputStream.internal.h"

@interface RamblerTCPStream ()

@property (strong, readwrite) NSString * remoteHostName;
@property (strong, readwrite) NSNumber * remoteHostPort;

@property (strong, readwrite) RamblerInputStream  * inputStream;
@property (strong, readwrite) RamblerOutputStream * outputStream;
@property (assign, readwrite) SSLContextRef tlsContext;

@property (strong, readwrite) NSError * error;
@property (assign, readwrite) RamblerStreamStatus status;

@property (strong, readonly) NSNumberFormatter *numberFormatter;

- (BOOL)connect;

@end