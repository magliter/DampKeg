/**************************************************************************************************
 * @file    RamblerStreamStatus.h
 * @date    2015-01-19
 * @author  Omar Stefan Evans
 * @brief   <# Brief Description #>
 * @details <# Detailed Description #>
 **************************************************************************************************/


typedef NS_ENUM(NSUInteger, RamblerStreamStatus) {
    RamblerStreamStatusNotOpen           = 0x00,
    RamblerStreamStatusOpening           = 0x01,
    RamblerStreamStatusOpen              = 0x02,
    RamblerStreamStatusReading           = 0x03,
    RamblerStreamStatusWriting           = 0x04,
    RamblerStreamStatusAtEnd             = 0x05,
    RamblerStreamStatusClosed            = 0x06,
    RamblerStreamStatusError             = 0x07,

    RamblerStreamStatusOpenAndSecuring   = 0xF1,
    RamblerStreamStatusOpenAndSecured    = 0xF2,
    RamblerStreamStatusReadingSecurely   = 0xF3,
    RamblerStreamStatusWritingSecurely   = 0xF4,
};