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

#import "MSWebView2ServiceWorkerDelegate.h"

NS_ASSUME_NONNULL_BEGIN

/**
 * @abstract This interface represents a service worker in WebView2 and provides
 * methods and properties for interacting with it, such as getting the script
 * uri, posting messages to it etc.
 */
@interface MSWebView2ServiceWorker : NSObject

/**
 * @abstract A string representing the Uri of the script that the worker is
 * executing.
 *
 * @discussion The `scriptUri` is a fully qualified URI, including the scheme,
 * host, and path. In contrast, the `scriptURL` property of the `Worker` object
 * in the DOM returns the relative URL of the script being executed by the
 * worker. For more details on DOM API, see the
 * [DOM API
 * documentation](https://developer.mozilla.org/docs/Web/API/Worker/scriptURL).
 *
 * The `scriptUri` property reflects normalization, ensuring that the URL is
 * standardized. For example, `HTTPS://EXAMPLE.COM/worker.js` is canonicalized
 * to `https://example.com/worker.js`; `https://bücher.de/worker.js` is
 * canonicalized to `https://xn--bcher-kva.de/worker.js`.
 */
@property(readonly, nonatomic, copy) NSString* scriptUri;

/**
 * @abstract The delegate that receives service worker events.
 */
@property(nullable, nonatomic, weak) id<MSWebView2ServiceWorkerDelegate>
    delegate;

/**
 * @abstract Posts the specified webMessage to the service worker.
 *
 * @param message The message data to be sent
 *
 * @discussion The service worker receives the message by subscribing to the
 * `message` event of the `self.chrome.webview` of the service worker.
 * @textblock
 * self.chrome.webview.addEventListener('message', handler)
 * self.chrome.webview.removeEventListener('message', handler)
 * @/textblock
 *
 * The <MSWebView2Settings> property `webMessageEnabled` must be `YES` or
 * the message will not be sent.
 *
 * The event args is an instance of `MessageEvent`. The `data` property of the
 * event arg is the `webMessage` string parameter parsed as a JSON string into
 * a JavaScript object. The `source` property of the event arg is the path to
 * the service worker script. The message is delivered asynchronously. If the
 * service worker is terminated or destroyed before the message is posted, the
 * message is discarded.
 *
 * See also the equivalent methods: <[MSWebView2WebView postWebMessageAsJSON:]>,
 * <[MSWebView2Frame postWebMessageAsJSON:]>,
 * <[MSWebView2DedicatedWorker postWebMessageAsJSON:]>.
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
 * @discussion This function behaves in exactly the same manner as
 * <postWebMessageAsJSON:>, but the `data` property of the event args of the
 * `self.chrome.webview` message is a string with the same value as
 * `webMessageAsString`. Use this instead of <postWebMessageAsJSON:> if you want
 * to communicate using simple strings rather than JSON objects. Please see
 * <postWebMessageAsJSON:> for additional information.
 *
 * See also the equivalent methods: <[MSWebView2WebView
 * postWebMessageAsString:]>,
 * <[MSWebView2Frame postWebMessageAsString:]>,
 * <[MSWebView2DedicatedWorker postWebMessageAsString:]>.
 */
- (void)postWebMessageAsString:(nullable NSString*)message;

@end

NS_ASSUME_NONNULL_END
