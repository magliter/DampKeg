/**********************************************************************************************************************
 * @file    Attribute
 * @date    2014-06-25
 * @brief   <# Brief Description#>
 * @details <#Detailed Description#>
 **********************************************************************************************************************/

#pragma once

#include "NamespaceableNode.hpp"

namespace rambler { namespace XML {

    class Attribute : public NamespaceableNode {
    public:
        template<typename... Args>
        static StrongPointer<Attribute const> create(Args... args) {
            return makeStrongPointer<Attribute const>(args...);
        }

        Attribute();
        Attribute(String name, String value);
        Attribute(StrongPointer<Namespace const> xmlnamespace, String name, String value);

        String getValue() const;
        String getEscapedValue() const;
        String getQuotedValue() const;

        virtual String getStringValue() const override;

        virtual bool isValid() const;

        virtual bool operator == (Attribute const & other) const;
        virtual bool operator != (Attribute const & other) const;

    private:
        String const value;
    };

    bool operator <  (StrongPointer<Attribute const> a, StrongPointer<Attribute const> b);
    bool operator == (StrongPointer<Attribute const> a, StrongPointer<Attribute const> b);
    bool operator != (StrongPointer<Attribute const> a, StrongPointer<Attribute const> b);

}}