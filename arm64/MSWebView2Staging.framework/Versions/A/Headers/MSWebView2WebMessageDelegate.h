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

#import "MSWebView2WebMessageReceivedInfo.h"

NS_ASSUME_NONNULL_BEGIN

@class MSWebView2WebView;

/**
 * Delegate for the WebMessage events.
 */
@protocol MSWebView2WebMessageDelegate <NSObject>

@optional

/**
 * @abstract Tells the delegate that a web message is received.
 *
 * @param webView The WebView sending the web message.
 * @param info Details about the web message.
 *
 * @discussion webMessageReceived runs when the
 * <MSWebView2Settings> property `webMessageEnabled` is set and the
 * frame document runs `window.chrome.webview.postMessage`.
 * The postMessage function is `void postMessage(object)`
 * where object is any object supported by JSON conversion.
 */
- (void)webView:(MSWebView2WebView*)webView
    webMessageReceived:(MSWebView2WebMessageReceivedInfo*)info;

@end

NS_ASSUME_NONNULL_END
