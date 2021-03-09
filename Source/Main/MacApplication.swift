//
//  MacApplication.swift
//  MacSupport
//
//  Created by 홍영준 on 2021/03/05.
//

import Cocoa

@main
class MacApplication {
    private static var appDelegate = AppDelegate()
    
    static func main() {
        autoreleasepool {
            let appliication = NSApplication.shared
            appliication.setActivationPolicy(.regular)
            appliication.activate(ignoringOtherApps: true)
            appliication.delegate = appDelegate
            appliication.run()
        }
    }
}
