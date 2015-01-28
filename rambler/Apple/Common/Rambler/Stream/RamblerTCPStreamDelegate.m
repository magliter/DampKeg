//
//  RamblerStreamDelegate.m
//  Rambler
//
//  Created by Omar Evans on 2015/01/18.
//  Copyright (c) 2015 DampKeg. All rights reserved.
//

#import "RamblerTCPStreamDelegate.h"

@implementation RamblerTCPStreamDelegate

- (void) streamDidOpen:(RamblerTCPStream *)stream {
    if (self.openedEventHandler) {
        self.openedEventHandler(stream);
    }
}

- (void) streamDidSecure:(RamblerTCPStream *)stream {
    if (self.securedEventHandler) {
        self.securedEventHandler(stream);
    }
}

- (void) streamDidClose:(RamblerTCPStream *)stream {
    if (self.closedEventHandler) {
        self.closedEventHandler(stream);
    }
}

- (void) stream:(RamblerTCPStream *)stream didFailToOpenWithError:(NSError *)error {
    if (self.failedToOpenEventHandler) {
        self.failedToOpenEventHandler(stream, error);
    }
}

- (void) stream:(RamblerTCPStream *)stream didFailToSecureWithError:(NSError *)error {
    if (self.failedToSecureEventHandler) {
        self.failedToSecureEventHandler(stream, error);
    }
}

- (void) stream:(RamblerTCPStream *)stream hasData:(NSData *)data {
    if (self.hasDataEventHandler) {
        self.hasDataEventHandler(stream, data);
    }
}

@end