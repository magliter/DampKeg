//
//  RamblerStreamDelegate.h
//  Rambler
//
//  Created by Omar Evans on 2015/01/18.
//  Copyright (c) 2015 DampKeg. All rights reserved.
//

#import <Foundation/Foundation.h>

@class RamblerTCPStream;

@protocol RamblerTCPStreamDelegate <NSObject>

- (void) streamDidOpen:(RamblerTCPStream *)stream;
- (void) streamDidSecure:(RamblerTCPStream *)stream;
- (void) streamDidClose:(RamblerTCPStream *)stream;
- (void) stream:(RamblerTCPStream *)stream didFailToOpenWithError:(NSError *)error;
- (void) stream:(RamblerTCPStream *)stream didFailToSecureWithError:(NSError *)error;
- (void) stream:(RamblerTCPStream *)stream hasData:(NSData *)data;

@end

typedef void (^OpenedEventHandler)(RamblerTCPStream *);
typedef void (^SecuredEventHandler)(RamblerTCPStream *);
typedef void (^ClosedEventHandler)(RamblerTCPStream *);
typedef void (^HasDataEventHandler)(RamblerTCPStream *, NSData *);
typedef void (^FailedToOpenEventHandler)(RamblerTCPStream *, NSError *);
typedef void (^FailedToSecureEventHandler)(RamblerTCPStream *, NSError *);

@interface RamblerTCPStreamDelegate : NSObject <RamblerTCPStreamDelegate>

@property (copy, readwrite) OpenedEventHandler         openedEventHandler;
@property (copy, readwrite) SecuredEventHandler        securedEventHandler;
@property (copy, readwrite) ClosedEventHandler         closedEventHandler;
@property (copy, readwrite) HasDataEventHandler        hasDataEventHandler;
@property (copy, readwrite) FailedToOpenEventHandler   failedToOpenEventHandler;
@property (copy, readwrite) FailedToSecureEventHandler failedToSecureEventHandler;

@end