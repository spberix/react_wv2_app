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

#import "MSWebView2ContentLoadingInfo.h"
#import "MSWebView2DOMContentLoadedInfo.h"
#import "MSWebView2NavigationCompletedInfo.h"
#import "MSWebView2NavigationStartingDecision.h"
#import "MSWebView2NavigationStartingInfo.h"
#import "MSWebView2SourceChangedInfo.h"

NS_ASSUME_NONNULL_BEGIN

@class MSWebView2WebView;
/**
 * Delegate for the navigation events.
 */
@protocol MSWebView2NavigationDelegate <NSObject>

@optional

/**
 * @abstract Called before the navigation will start.
 *
 * @param webView The WebView that started to navigate.
 * @param info The details about this navigation request.
 *
 * @return A <MSWebView2NavigationStartingDecision> object that indicates how to
 * handle the navigation.
 *
 * @discussion This method is called before the an navigation
 * starts. If the returned value is `nil`, the navigation will continue as
 * normal. If a <MSWebView2NavigationStartingDecision> is returned, then the
 * navigation behavior will change according to the properties of the
 * `MSWebView2NavigationStartingDecision`.
 *
 * For example, when `shouldCancel` of `MSWebView2NavigationStartingDecision` is
 * `YES`, the navigation will be cancelled.  The
 * `additionalAllowedFrameAncestors` of `MSWebView2NavigationStartingDecision`
 * is an iframe-only property, and will be ignored for main frame.
 * @textblock
 * - (MSWebView2NavigationStartingDecision*)webView:(MSWebView2WebView*)webView
 *                               navigationStarting:
 *                                   (MSWebView2NavigationStartingInfo*)info {
 *   if (IsAllowListURI(info.URI)) return nil;
 *   MSWebView2NavigationStartingDecision* decision =
 *       [[MSWebView2NavigationStartingDecision alloc] init];
 *   decision.shouldCancel = YES;
 *   return decision;
 * }
 * @/textblock
 */
- (nullable MSWebView2NavigationStartingDecision*)
               webView:(MSWebView2WebView*)webView
    navigationStarting:(MSWebView2NavigationStartingInfo*)info;

/**
 * @abstract Called when the `source` changed.
 *
 * @param webView The WebView that started to navigate.
 * @param info The details about this navigation.
 *
 * @discussion It runs when navigating to a different site or fragment
 * navigations. It does not trigger for other types of navigations such as page
 * refreshes or history.pushState with the same URL as the current page.
 * webView:sourceChanged: runs before <webView:contentLoading:> for navigation
 * to a new document.
 * @textblock
 * - (void)webView:(MSWebView2WebView*)webView
 *     sourceChanged:(MSWebView2SourceChangedInfo*)info {
 *   [self.addressTextField setStringValue:webView.source];
 * }
 * @/textblock
 */
- (void)webView:(MSWebView2WebView*)webView
    sourceChanged:(MSWebView2SourceChangedInfo*)info;

/**
 * @abstract Called before any content is loaded.
 *
 * @param webView The WebView that started to navigate.
 * @param info The details about this navigation.
 *
 * @discussion This method is called before any content is loaded, including
 * scripts added with `addScriptToExecuteOnDocumentCreated:completionHandler:`.
 * But it is not called if a same-document navigation occurs (such as through
 * `fragment` navigations or `history.pushState` navigations). This operation
 * follows the <webView:navigationStarting> and <webView:sourceChanged> and
 * precedes the <webViewHistoryChanged> and <webView:navigationCompleted>.
 */
- (void)webView:(MSWebView2WebView*)webView
    contentLoading:(MSWebView2ContentLoadingInfo*)info;

/**
 * @abstract Called when the navigation history of the top level document
 * changed.
 *
 * @param webView The WebView that started to navigate.
 *
 * @discussion It is called after <webView:sourceChanged:> and
 * <webView:contentLoading:>. You can use this to check whether `canGoBack` or
 * `canGoForward` values have changed.
 * @textblock
 * - (void)webViewHistoryChanged:(MSWebView2WebView*)webView {
 *   self.backButton.enabled = webView.canGoBack;
 *   self.forwardButton.enabled = webView.canGoForward;
 * }
 * @/textblock
 */
- (void)webViewHistoryChanged:(MSWebView2WebView*)webView;

/**
 * @abstract Called when the initial HTML document has been parsed.
 *
 * @param webView The WebView that started to navigate.
 * @param info The details about this navigation.
 *
 * @discussion This aligns with the document's `DOMContentLoaded` event in HTML.
 * @textblock
 * - (void)webView:(MSWebView2WebView*)webView
 *     DOMContentLoaded:(MSWebView2DOMContentLoadedInfo*)info {
 *   [webView executeScript:
 *            @"let content = document.createElement('h2');"
 *             "content.style.color = 'blue';"
 *             "content.textContent = 'This text was added by the host app';"
 *             "document.body.appendChild(content);"
 *     completionHandler:nil];
 * }
 * @/textblock
 */
- (void)webView:(MSWebView2WebView*)webView
    DOMContentLoaded:(MSWebView2DOMContentLoadedInfo*)info;

/**
 * @abstract Called after the navigation has finished.
 *
 * @param webView The WebView that started to navigate.
 * @param info The details about this finished navigation.
 *
 * @discussion This method is called when the web view has completely loaded
 * (concurrently when body.onload runs) or loading stopped with error.
 * @textblock
 * - (void)webView:(MSWebView2WebView*)webView
 *     navigationCompleted:(MSWebView2NavigationCompletedInfo*)info {
 *   if (info.isSuccess) {
 *     self.backButton.enabled = webView.canGoBack;
 *     self.forwardButton.enabled = webView.canGoForward;
 *     return;
 *   }
 *   // Handle navigation failed
 *   if (info.webErrorStatus == MSWebView2WebErrorStatusDisconnected) {
 *     [self showAlert:@"Please check your network connection"];
 *   }
 * }
 * @/textblock
 */
- (void)webView:(MSWebView2WebView*)webView
    navigationCompleted:(MSWebView2NavigationCompletedInfo*)info;

/**
 * @abstract Called when any child frame in the WebView requests permission to
 * navigate to a different URI.
 *
 * @param webView The WebView whose frame is about to navigate.
 * @param info The frame navigation starting info.
 *
 * @return A <MSWebView2NavigationStartingDecision> object that indicates how to
 * handle the navigation.
 *
 * @discussion This method will be triggered before the iframe navigation
 * starts. Redirects trigger this operation as well, and the navigation
 * id is the same as the original one.
 *
 * Note: A frame's navigation will trigger this method and
 * `frame:navigationStarting:` of <MSWebView2FrameNavigationDelegate>,
 * and the latter will run after this method. WebView uses the last returned
 * `MSWebView2NavigationStartingDecision`, so if both delegate methods are
 * implemented by the developer, the `MSWebView2NavigationStartingDecision`
 * returned by `frame:navigationStarting:`will take effect.
 * @textblock
 * // This will prevent a frame from navigating to a blocked domain.
 * - (MSWebView2NavigationStartingDecision*)webView:(MSWebView2WebView*)
 *                       frameNavigationStarting:
 *                            (MSWebView2NavigationStartingInfo*)info {
 *   if (IsAllowListURI(info.URI)) return nil;
 *   MSWebView2NavigationStartingDecision* decision =
 *      [[MSWebView2NavigationStartingDecision alloc] init];
 *   decision.shouldCancel = YES;
 *   return decision;
 * }
 * @/textblock
 */
- (nullable MSWebView2NavigationStartingDecision*)
                    webView:(MSWebView2WebView*)webView
    frameNavigationStarting:(MSWebView2NavigationStartingInfo*)info;

/**
 * @abstract Called when any child frame has completely loaded (`body.onload`
 * has been raised) or loading stopped with error.
 *
 * @param webView The WebView whose frame finishes navigation.
 * @param info The frame navigation completed info.
 *
 * @discussion It will be triggered when a child frame has completely loaded
 * (concurrently when body.onload has triggered) or loading stopped with error.
 * @textblock
 * - (void)webView:(MSWebView2WebView*)webView
 *    frameNavigationCompleted:(MSWebView2NavigationCompletedInfo*)info {
 * if (!info.isSuccess) {
 *   // The web page can cancel its own iframe loads, so we'll ignore that.
 *   if (info.webErrorStatus != MSWebView2WebErrorStatusOperationCanceled) {
 *     [self showAlert:@"Iframe navigation failed."];
 *   }
 * }
 * }
 * /@textblock
 */
- (void)webView:(MSWebView2WebView*)webView
    frameNavigationCompleted:(MSWebView2NavigationCompletedInfo*)info;

@end

NS_ASSUME_NONNULL_END
