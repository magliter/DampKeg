//
//  TCPStream.h
//  Rambler
//
//  Created by Omar Evans on 2015/01/18.
//  Copyright (c) 2015 DampKeg. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "RamblerStreamStatus.h"
#import "RamblerTCPStreamDelegate.h"

@interface RamblerTCPStream : NSObject<NSStreamDelegate>

@property (strong, readonly) NSString * domainName;
@property (strong, readonly) NSString * serviceName;
@property (strong, readonly) NSString * remoteHostName;
@property (strong, readonly) NSNumber * remoteHostPort;

@property (strong, readonly) NSInputStream  * inputStream;
@property (strong, readonly) NSOutputStream * outputStream;

@property (assign, readwrite) id<RamblerTCPStreamDelegate> delegate;

@property (strong, readonly) NSError * error;
@property (assign, readonly) RamblerStreamStatus status;


- (instancetype) initWithDomainName:(NSString *)domainName serviceName:(NSString *) serviceName NS_DESIGNATED_INITIALIZER;

- (void)open;
- (void)close;
- (void)secure;

- (void)writeData:(NSData *)data;

@end
