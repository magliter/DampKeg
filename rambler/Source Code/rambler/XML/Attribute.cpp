/**********************************************************************************************************************
 * @file    Attribute.cpp
 * @date    2014-06-25
 * @brief   <# Brief Description#>
 * @details <#Detailed Description#>
 **********************************************************************************************************************/

#include "Attribute.hpp"

namespace rambler { namespace XML {

    Attribute::Attribute() : NamespaceableNode(Type::Attribute)
    {
        /* Nothing to do here */
    }

    Attribute::Attribute(String name, String value) : value(value), NamespaceableNode(name, Type::Attribute)
    {
        /* Nothing to do here */
    }

    Attribute::Attribute(StrongPointer<Namespace const> xmlnamespace, String name, String value)
    : value(value), NamespaceableNode(name, xmlnamespace, Type::Attribute)
    {
        /* Nothing to do here */
    }

    String Attribute::getValue() const
    {
        return value;
    }

    String Attribute::getEscapedValue() const
    {
        //TODO: Actually escape the value in rambler::XML::Attribute::getEscapedValue()
        return value;
    }

    String Attribute::getQuotedValue() const
    {
        return '"' + getEscapedValue() + '"';
    }

    String Attribute::getStringValue() const
    {
        return getQualifiedName() + "=" + getQuotedValue();
    }

    bool Attribute::isValid() const
    {
        return !name.empty() && !value.empty();
    }

    bool Attribute::operator == (Attribute const & other) const
    {
        return (value == other.value) && (NamespaceableNode::operator==(other));
    }

    bool Attribute::operator != (Attribute const & other) const
    {
        return !(*this == other);
    }

    bool operator <  (StrongPointer<Attribute const> a, StrongPointer<Attribute const> b) {
        return *a < *b;
    }

    bool operator == (StrongPointer<Attribute const> a, StrongPointer<Attribute const> b) {
        return *a == *b;
    }

    bool operator != (StrongPointer<Attribute const> a, StrongPointer<Attribute const> b) {
        return *a != *b;
    }


}}