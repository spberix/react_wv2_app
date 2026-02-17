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

#import "MSWebView2NewWindowRequestedInfo.h"

NS_ASSUME_NONNULL_BEGIN

@class MSWebView2WebView;

/**
 * Delegate for the new window events.
 */
@protocol MSWebView2NewWindowDelegate <NSObject>

@optional

/**
 * @abstract Called when new window is requested.
 *
 * @param webView The web view that requests new window.
 * @param info The details about this request.
 * @param decisionHandler The decision handler to call to response new window
 * request.
 *
 * @discussion The parameter `handled` of decisionHandler indicates whether the
 * NewWindowRequest event is handled by host app and the parameter `newWebView`
 * indicates the new web view instance of the new window created by host app. If
 * `newWebView` is not nil, that means host app creates a new web view window
 * successfully and will report the new window requested response to browser
 * process through the `newWebView` instance. If `newWebView` is nil, that means
 * host app doesn't create new window, then the behavior depends on the value of
 * `handled`. If `handled` is `YES`, no window will be created. If `handled` is
 * `NO`, the original `webview` takes over and creates a default window through
 * its browser process.
 */
- (void)webView:(MSWebView2WebView*)webView
    newWindowRequested:(MSWebView2NewWindowRequestedInfo*)info
       decisionHandler:(void (^)(BOOL handled,
                                 MSWebView2WebView* newWebView))decisionHandler;

@end

NS_ASSUME_NONNULL_END
