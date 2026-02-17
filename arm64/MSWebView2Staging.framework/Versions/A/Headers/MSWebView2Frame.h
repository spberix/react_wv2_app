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

#import <Foundation/Foundation.h>

#import "MSWebView2FrameDelegate.h"
#import "MSWebView2FrameNavigationDelegate.h"
#import "MSWebView2FramePermissionDelegate.h"
#import "MSWebView2FrameWebMessageDelegate.h"

NS_ASSUME_NONNULL_BEGIN

/**
 * MSWebView2Frame provides direct access to the iframes information.
 */
@interface MSWebView2Frame : NSObject

/**
 * @abstract The name attribute of the frame, as in <iframe name="frame-name"
 * ...>. It is @"" when the frame has no name attribute.
 */
@property(nonatomic, readonly, copy) NSString* name;

/**
 * @abstract Indicates whether a frame is destroyed. It is YES during the
 * Destroyed event.
 */
@property(nonatomic, readonly, getter=isDestroyed) BOOL destroyed;

/**
 * @abstract Posts a message that is a simple string rather than a JSON string
 * representation of a JavaScript object.
 *
 * @param message The message data to be sent. The render process will receive
 * an empty message when message is `nil`.
 *
 * @discussion This function behaves in exactly the same
 * manner as <postWebMessageAsJSON:>, but the `data` property of the event
 * args of the `window.chrome.webview` message is a string with the same
 * value as `webMessageAsString`. Use this instead of
 * <postWebMessageAsJSON:> if you want to communicate using simple strings
 * rather than JSON objects.
 */
- (void)postWebMessageAsString:(nullable NSString*)message;

/**
 * @abstract Posts the specified webMessage to the frame.
 *
 * @param message The message data to be sent.
 *
 * @discussion The <MSWebView2Settings> property `webMessageEnabled` must be YES
 * or the message will not be sent.
 *
 * The frame receives the message by
 * subscribing to the `message` event of the `window.chrome.webview` of the
 * frame document.
 * @textblock
 * window.chrome.webview.addEventListener('message', function(e) {
 *   console.log('Received message from the host page: ' + e.data);
 * });
 * @/textblock
 *
 * When processing the event at js side, the `data` property of the event
 * args is the `webMessage` string parameter parsed as a JSON string into a
 * JavaScript object, and The `source` property of the event args is a
 * reference to the `window.chrome.webview` object.
 * The message is delivered asynchronously. If a navigation occurs before
 * the message is posted to the page, the message is discarded.
 *
 * @exception NSInvalidArgumentException Thrown if `message` is not a valid JSON
 */
- (void)postWebMessageAsJSON:(NSString*)message;

/**
 * @abstract Delegate for the frame's WebMessage event.
 */
@property(nullable, nonatomic, weak) id<MSWebView2FrameWebMessageDelegate>
    frameWebMessageDelegate;

/**
 * @abstract Delegate for the frame's permission requested event.
 */
@property(nullable, nonatomic, weak) id<MSWebView2FramePermissionDelegate>
    framePermissionDelegate;

/**
 * @abstract Delegate for the frame's navigation event.
 */
@property(nullable, nonatomic, weak) id<MSWebView2FrameNavigationDelegate>
    frameNavigationDelegate;

/**
 * @abstract Delegate for frame's event.
 */
@property(nullable, nonatomic, weak) id<MSWebView2FrameDelegate> frameDelegate;

/**
 * @abstract Add the provided host object to script running in the iframe with
 * the specified name of the list of the specified origins.
 *
 * @param object The host object.
 * @param name The name representing the host object.
 * @param origins The supported iframe's origins
 *
 * @discussion The host object will be accessible for this iframe only if the
 * iframe's origin during access matches one of the origins which are passed.
 *
 * The provided origins will be normalized before comparing to the
 * origin of the document. So the scheme name is made lower case, the host will
 * be punycode decoded as appropriate, default port values will be removed,
 * and so on. This means the origin's host may be punycode encoded or not and
 * will match regardless. If list contains malformed origin the call will fail.
 *
 * The method can be called multiple times in a row without calling
 * RemoveHostObjectFromScript for the same object name. It will replace the
 * previous object with the new object and new list of origins.
 *
 * List of origins will be treated as following:
 *    1. empty list - call will succeed and object will be added for the iframe
 *       but it will not be exposed to any origin;
 *    2. list with origins - during access to host object from iframe the origin
 *       will be checked that it belongs to this list;
 *    3. list with "*" element - host object will be available for iframe for
 *       all origins.
 *
 * We suggest not to use list with "*" element without understanding
 * security implications of giving access to host object from iframes with
 * unknown origins.
 *
 * @exception NSInvalidArgumentException Thrown if called after the iframe has
 * been destroyed, or the origins array contains invalid origin strings, or the
 * object is not a derived class of `NSObject`.
 */
- (void)addHostObjectToScript:(NSObject*)object
                     withName:(NSString*)name
                      origins:(NSArray<NSString*>*)origins;

/**
 * @abstract Remove the host object specified by the name so that it is no
 * longer accessible from JavaScript code in the WebView.
 *
 * @param name The name representing the host object
 *
 * @discussion While new access attempts are denied, if the object is already
 * obtained by JavaScript code in the WebView, the JavaScript code continues to
 * have access to that object.
 */
- (void)removeHostObjectFromScript:(NSString*)name;

/**
 * @abstract Export class's properties and methods to JavaScript.
 *
 * @param cls The object's class
 * @param properties The class exported property list.
 * @param methods The class exported method list.
 *
 * @discussion Define the access control of a Objective-C class. If javascript
 * execute a non export property/method, it will failed.
 *
 * All javascript used property/method must be exported before use.
 */
- (void)exportObjectClass:(Class)cls
               properties:(NSArray<NSString*>*)properties
                  methods:(NSDictionary<NSString*, NSString*>*)methods;

/**
 * @abstract Run JavaScript code from the `javaScript` parameter in the current
 * frame.
 *
 * @param javaScript The JavaScript code to be executed
 * @param completionHandler The evaluation result of the provided JavaScript is
 * passed to the completionHandler.
 *
 * @discussion The result value is a JSON encoded string. If the result is
 * undefined, or otherwise is not able to be encoded into JSON, then the result
 * is considered to be null, which is encoded in JSON as the string "null".
 *
 * A function that has no explicit return value returns undefined.
 * If the script that was run throws an unhandled exception, then the result is
 * also "null". This method is applied asynchronously. If the method is run
 * before `ContentLoading`, the script will not be executed and the string
 * "null" will be returned. This operation executes the script even if
 * <MSWebView2Settings> property `scriptEnabled` is set to `NO`.
 *
 * For example:
 * @textblock
 * - (void)injectScriptInvoked:(SheetViewController*)sheetViewController
 *                  intoIFrame:(MSWebView2Frame*)frame{
 *   NSString* script = [sheetViewController getInput];
 *   __weak typeof(self) weakSelf = self;
 *   [frame executeScript:script
 *      completionHandler:^(NSString* result, NSError* error) {
 *        if (weakSelf == nil) {
 *          return;
 *        }
 *        __strong typeof(self) strongSelf = weakSelf;
 *        NSString* text = [@"CallExecuteScriptInIFrameResult - "
 *            stringByAppendingFormat:@"[%@] and error code - [%@]", result,
 *                                    error];
 *        [strongSelf showAlert:text];
 *     }];
 * }
 * @/textblock
 */
- (void)executeScript:(NSString*)javaScript
    completionHandler:
        (void (^_Nullable)(NSString* _Nullable result,
                           NSError* _Nullable error))completionHandler;

/**
 * @abstract The unique identifier of the current frame.
 */
@property(nonatomic, readonly) NSInteger frameID;

@end

NS_ASSUME_NONNULL_END
