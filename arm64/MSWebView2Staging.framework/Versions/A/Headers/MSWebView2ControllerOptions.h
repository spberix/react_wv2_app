/*
 * Copyright (C) Microsoft Corporation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *    * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *    * The name of Microsoft Corporation, or the names of its
 * contributors may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#import <AppKit/AppKit.h>
#import "MSWebView2Foundation.h"

NS_ASSUME_NONNULL_BEGIN

/**
 * Configuration of WebView controller.
 */
MS_WEBVIEW2_EXTERN
@interface MSWebView2ControllerOptions : NSObject <NSCopying>

/**
 * @abstract Specify a profile name. which is only allowed to contain the
 * following ASCII characters.
 *
 * @discussion It has a maximum length of 64 characters excluding the
 * null-terminator. It is ASCII case insensitive.
 *    * alphabet characters: a-z and A-Z
 *    * digit characters: 0-9
 *    * and '#','@','$','(',')','+','-','_','~','.',' '(space).
 * Note: the text must not end with a period '.' or ' ' (space). And, although
 * upper-case letters are allowed, they're treated just as lower-case
 * counterparts because the profile name will be mapped to the real profile
 * directory path on disk and MacOS file system handles path names in a
 * case-insensitive way.
 */
@property(nullable, nonatomic, copy) NSString* profileName;

/**
 * @abstract Enable/Disable InPrivate Mode.
 */
@property(nonatomic, getter=isInPrivateModeEnabled) BOOL inPrivateModeEnabled;

/**
 * @abstract Enables or disables the WebView's built-in ability to automatically
 * manage visibility.
 *
 * @discussion By default this value is `YES`. When enabled, the WebView
 * automatically updates `document.visibilityState` to `hidden` in JavaScript
 * when the window is occluded, minimized, or hidden, and restores it to
 * `visible` when the window becomes visible again. This behavior is designed to
 * enhance performance and suits most use cases.
 *
 * If developers prefer to manage the visibility state manually, they must set
 * this property to `NO` when creating the WebView. Visibility state can then be
 * controlled by setting the `visible` property of <MSWebView2Controller>.
 */
@property(nonatomic, getter=isAutoUpdateVisibilityEnabled)
    BOOL autoUpdateVisibilityEnabled;

/**
 * @abstract Default background color of all web content.
 *
 * @discussion Webview renders this color when there is no web content loaded
 * such as before the initial navigation or between navigations. Web pages with
 * undefined css background properties or background properties containing
 * transparent pixels will render their contents over this color. Web pages with
 * defined and opaque background properties that span the page will obscure the
 * defaultBackgroundColor and display normally.
 *
 * The default value for this property is white to resemble the native browser
 * experience.
 *
 * If set defaultBackgroundColor with nil, webview will reset the
 * defaultBackgroundColor to white.
 *
 * @exception The alpha property of defaultBackgroundColor must be equal to 0.0
 * or 1.0. Setting an alpha value between 0.0 and 1.0 will result in an error
 * being thrown.
 */
@property(nullable, nonatomic, copy) NSColor* defaultBackgroundColor;

@end

NS_ASSUME_NONNULL_END
