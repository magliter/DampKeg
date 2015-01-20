/**********************************************************************************************************************
 * @file    TCPConnection.cpp
 * @date    2014-07-04
 * @brief   <# Brief Description#>
 * @details <#Detailed Description#>
 **********************************************************************************************************************/

#include "rambler/Connection/TCPConnection.hpp"

namespace rambler { namespace Connection {

    void TCPConnection::handleOpeningFailedEvent()
    {
        this->close();
        BidirectionalStream<UInt8>::handleOpeningFailedEvent();
    }

    void TCPConnection::handleSecuringFailedEvent()
    {
        this->close();
        BidirectionalStream<UInt8>::handleSecuringFailedEvent();
    }

}}