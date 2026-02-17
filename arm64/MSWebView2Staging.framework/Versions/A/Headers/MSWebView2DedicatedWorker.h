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

#import "MSWebView2DedicatedWorkerDelegate.h"

NS_ASSUME_NONNULL_BEGIN

/**
 * @abstract This interface represents a dedicated worker in WebView2.
 */
@interface MSWebView2DedicatedWorker : NSObject

/**
 * @abstract A string representing the Uri of the script that the worker is
 * executing.
 */
@property(readonly, nonatomic, copy) NSString* scriptUri;

/**
 * @abstract The delegate that receives dedicated worker events.
 */
@property(nullable, nonatomic, weak) id<MSWebView2DedicatedWorkerDelegate>
    delegate;

/**
 * @abstract Posts the specified webMessage to the worker.
 *
 * @param message The message data to be sent
 *
 * @discussion The worker receives the message by subscribing to the
 * `message` event of the `window.chrome.webview` of the worker document.
 * @textblock
 * self.chrome.webview.addEventListener('message', function(e) {
 *   console.log('Received message from the host page: ' + e.data);
 * });
 * @/textblock
 *
 * The <MSWebView2Settings> property `webMessageEnabled` must be `YES` or
 * the message will not be sent.
 *
 * When processing the event at js side, the `data` property of the event
 * args is the `webMessage` string parameter parsed as a JSON string into a
 * JavaScript object, and The `source` property of the event args is a
 * reference to the `self.chrome.webview` object.
 * The message is delivered asynchronously. If a navigation occurs before
 * the message is posted to the page, the message is discarded.
 *
 * @exception NSInvalidArgumentException Thrown if `message` is not a valid
 * JSON.
 */
- (void)postWebMessageAsJSON:(NSString*)message;

/**
 * @abstract Posts a message that is a simple string rather than a JSON string
 * representation of a JavaScript object.
 *
 * @param message The message data to be sent. When `message` is empty or `nil`,
 * JavaScript will receive an empty message.
 *
 * @discussion This function behaves in exactly the same
 * manner as <postWebMessageAsJSON:>, but the `data` property of the event
 * args of the `self.chrome.webview` message is a string with the same
 * value as `webMessageAsString`. Use this instead of
 * <postWebMessageAsJSON:> if you want to communicate using simple strings
 * rather than JSON objects.
 */
- (void)postWebMessageAsString:(NSString*)message;

@end

NS_ASSUME_NONNULL_END
