/**************************************************************************************************
 * @file    TCPStream.cpp
 * @date    2015-01-27
 * @brief   <# Brief Description#>
 * @details <#Detailed Description#>
 **************************************************************************************************/

#include "rambler/Stream/TCPStream.hpp"

namespace rambler { namespace Stream {

    void TCPStream::handleOpeningFailedEvent()
    {
        this->close();
        BidirectionalStream<UInt8>::handleOpeningFailedEvent();
    }

    void TCPStream::handleSecuringFailedEvent()
    {
        this->close();
        BidirectionalStream<UInt8>::handleSecuringFailedEvent();
    }

}}