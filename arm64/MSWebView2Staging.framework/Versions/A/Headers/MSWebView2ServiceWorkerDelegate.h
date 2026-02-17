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

@class MSWebView2ServiceWorker;
@class MSWebView2WebMessageReceivedInfo;

NS_ASSUME_NONNULL_BEGIN

/**
 * @abstract Delegate protocol for service worker events.
 */
@protocol MSWebView2ServiceWorkerDelegate <NSObject>

@optional
/**
 * @abstract Called when the service worker is being destroying.
 *
 * @param serviceWorker The service worker that is about to be destroying.
 *
 * @discussion A service worker object is destroyed when the worker script is
 * terminated or when the `MSWebView2ServiceWorker` object is destroyed.
 *
 * If the service worker has already been destroying before the event handler is
 * registered, the handler will never be called.
 */
- (void)serviceWorkerWillDestroy:(MSWebView2ServiceWorker*)serviceWorker;

/**
 * @abstract Called when a web message is received from the service worker.
 *
 * @param serviceWorker The service worker that sent the message.
 * @param info Information about the received web message.
 *
 * @discussion `WebMessageReceived` is fired when the <MSWebView2Settings>
 * property `webMessageEnabled` is set to `YES` and the service worker runs
 * `self.chrome.webview.postMessage`. The `postMessage` function is
 * `void postMessage(object)` where object is any object supported by JSON
 * conversion.
 *
 * If the service worker calls `postMessage` multiple times, the corresponding
 * `serviceWorker:webMessageReceived:` delegate method calls are guaranteed to
 * be fired in the same order.
 */
- (void)serviceWorker:(MSWebView2ServiceWorker*)serviceWorker
    webMessageReceived:(MSWebView2WebMessageReceivedInfo*)info;

@end

NS_ASSUME_NONNULL_END
