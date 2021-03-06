/**********************************************************************************************************************
 * @file    Element.hpp
 * @date    2014-12-12
 * @brief   <# Brief Description#>
 * @details <#Detailed Description#>
 **********************************************************************************************************************/

#pragma once

#include "NamespaceableNode.hpp"
#include "Attribute.hpp"
#include "TextNode.hpp"

#include "rambler/types.hpp"

namespace rambler { namespace XML {

    class Element : public NamespaceableNode, public std::enable_shared_from_this<Element> {
    public:
        Element(String name, StrongPointer<Namespace const> xmlnamespace, StrongPointer<Namespace const> defaultNamespace);
        
        static StrongPointer<Element> createWithName(String name);
        static StrongPointer<Element> createWithName(String name, StrongPointer<Namespace const> defaultNamespace);
        static StrongPointer<Element> createWithNameAndNamespace(String name,
                                                                 StrongPointer<Namespace const> xmlnamespace);
        static StrongPointer<Element> createWithNameAndNamespace(String name,
                                                                 StrongPointer<Namespace const> xmlnamespace,
                                                                 StrongPointer<Namespace const> defaultNamespace);

        StrongPointer<Element> getPtr();
        StrongPointer<Element> getParent() const;

        void addChild(StrongPointer<Element> child);
        void addChild(StrongPointer<TextNode> child);

        std::vector<StrongPointer<Node>> getChildren() const;

        std::vector<StrongPointer<Element>> getElementsByName(String const name) const;
        std::vector<StrongPointer<Element>> getElementsByName(String const name,
                                                              StrongPointer<Namespace const> const xmlnamespace) const;
        std::vector<StrongPointer<Element>> getElementsByNamespace(StrongPointer<Namespace const> const xmlnamespace) const;

        StrongPointer<Element> getFirstElementByName(String const name) const;
        StrongPointer<Element> getFirstElementByName(String const name,
                                                     StrongPointer<Namespace const> const xmlnamespace) const;
        StrongPointer<Element> getFirstElementByNamespace(StrongPointer<Namespace const> const xmlnamespace) const;

        StrongPointer<Element> getLastElementByName(String const name) const;
        StrongPointer<Element> getLastElementByName(String const name,
                                                    StrongPointer<Namespace const> const xmlnamespace) const;
        StrongPointer<Element> getLastElementByNamespace(StrongPointer<Namespace const> const xmlnamespace) const;

        StrongPointer<Element> getElementByID(String const id) const;

        String getTextContent() const;

        virtual StrongPointer<Namespace const> getNamespace() const override;
        StrongPointer<Namespace const> getDefaultNamespace() const;

        void addNamespace(StrongPointer<Namespace const> xmlnamespace);
        std::vector<StrongPointer<Namespace const>> getNamespaces() const;
        StrongPointer<Namespace const> getNamespaceByPrefix(String prefix) const;


        void addAttribute(StrongPointer<Attribute const> attribute);

        template<typename... Args>
        void addAttribute(Args... args) {
            addAttribute(Attribute::create(args...));
        }

        void addAttributes(std::set<StrongPointer<Attribute const>> attributes);

        StrongPointer<Attribute const> getAttribute(String name) const;
        StrongPointer<Attribute const> getAttribute(StrongPointer<Namespace const> xmlnamespace, String name) const;
        std::set<StrongPointer<Attribute const>> getAttributes() const;

        void setAttributes(std::set<StrongPointer<Attribute const>> attributes);

        void removeAttribute(String name);
        void removeAttribute(StrongPointer<Namespace const> xmlnamespace, String name);

        virtual String getStringValue() const override;

        virtual bool isValid() const;

        bool operator == (Element const & other);
        bool operator != (Element const & other);
    private:
        Element();

        StrongPointer<Namespace const> defaultNamespace = Namespace::DefaultNamespace();
        std::vector<StrongPointer<Namespace const>> namespaces;
        std::set<StrongPointer<Attribute const>> attributes;

        WeakPointer<Element> parent;
        std::vector<StrongPointer<Node>> children;
    };

    bool equivalent(StrongPointer<Element> const anElement, StrongPointer<Element> const anotherElement);
    bool equivalentByName(StrongPointer<Element> const anElement, StrongPointer<Element> const anotherElement);
    bool equivalentByNameAndNamespace(StrongPointer<Element> const anElement, StrongPointer<Element> const anotherElement);

}}