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

#import "MSWebView2WebResourceRequest.h"
#import "MSWebView2WebResourceRequestedInfo.h"
#import "MSWebView2WebResourceResponse.h"

NS_ASSUME_NONNULL_BEGIN

@class MSWebView2WebView;

/**
 * Delegate for the web resource events.
 */
@protocol MSWebView2WebResourceDelegate <NSObject>

@optional

/**
 * @abstract Called when receiving a web resource request.
 *
 * @param webView The WebView sending the web resource request.
 * @param info The details about the intercepted request.
 * @param completionHandler The `completionHandler` is to
 * customize HTTP request or HTTP request response.
 *
 * @discussion The parameter `newRequest` of completionHandler is to override
 * HTTP request. One thing to note is that we use original HTTP request if
 * `newRequest` is `nil`. The parameter `response` of completionHandler is to
 * override HTTP response. You can call
 * `createWebResourceResponse:statusCode:reasonPhrase:headers:` to create a
 * <MSWebView2WebResourceResponse> object. If `response` is `nil`, we will
 * continue the network request.
 *
 * The following are common use cases
 * - If you only want to continue the request, call `completionHandler(nil,
 * nil)`.
 * - If you want to use customized request to continue, pass it like
 * `completionHandler(newRequest, nil)`.
 * - If you want to customize HTTP response, pass it like
 * `completionHandler(nil, response)`
 *
 * @textblock
 * - (void)webView:(MSWebView2WebView*)webView
 *     webResourceRequested:(MSWebView2WebResourceRequestedInfo*)info
 *        completionHandler:(void (^)(MSWebView2WebResourceRequest* newRequest,
 *                                    MSWebView2WebResourceResponse* response))
 *                              completionHandler {
 *   MSWebView2WebResourceResponse* response = nil;
 *   if (info.resourceContext == MSWebView2WebResourceContextImage) {
 *     response = [webView.environment
 *         createWebResourceResponse:nil
 *                        statusCode:403
 *                      reasonPhrase:@"Blocked"
 *                           headers:@"Content-Type : image/jpeg"];
 *   }
 *   completionHandler(nil, response);
 * }
 * @/textblock
 */
- (void)webView:(MSWebView2WebView*)webView
    webResourceRequested:(MSWebView2WebResourceRequestedInfo*)info
       completionHandler:
           (void (^)(MSWebView2WebResourceRequest* newRequest,
                     MSWebView2WebResourceResponse* response))completionHandler;

@end

NS_ASSUME_NONNULL_END
