/**********************************************************************************************************************
 * @file    XMLStream.h
 * @date    2014-07-02
 * @brief   <# Brief Description#>
 * @details <#Detailed Description#>
 **********************************************************************************************************************/

#import <Foundation/Foundation.h>

@class JID;

@interface XMLStream : NSObject

- (instancetype)initWithJID:(JID *)aJID;
- (instancetype)initWithHostname:(NSString *)aHostname;
- (instancetype)initWithHostname:(NSString *)aHostname jid:(JID *)aJID;
- (instancetype)initWithHostname:(NSString *)aHostname port:(UInt16)aPort;
- (instancetype)initWithHostname:(NSString *)aHostname port:(UInt16)aPort jid:(JID *)aJID;

- (void)open;
- (void)secure;
- (void)close;

- (void)sendData:(NSData *) data;
- (void)sendString:(NSString *) string;


@end