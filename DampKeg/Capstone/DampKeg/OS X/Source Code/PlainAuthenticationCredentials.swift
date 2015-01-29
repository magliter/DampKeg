//
//  PlainAuthenticationCredentials.swift
//  Swift Cocoa
//
//  Created by Omar Stefan Evans on 6/12/14.
//  Copyright (c) 2014 Damp Keg. All rights reserved.
//

import Foundation

class PlainAuthenticationCredentials : NSObject {
    var username:String? {
        didSet {
            if let JID = username {
                isValid = JID.rangeOfString(".+@.+", options: .RegularExpressionSearch) != nil ? true : false
            }
        }
    }

    var password:String?
    dynamic var isValid:Bool = false
}
