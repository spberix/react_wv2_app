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

#import "MSWebView2NavigationCompletedInfo.h"
#import "MSWebView2NavigationStartingDecision.h"
#import "MSWebView2NavigationStartingInfo.h"

NS_ASSUME_NONNULL_BEGIN

@class MSWebView2Frame;
/**
 * Delegate for the frame navigation events.
 */
@protocol MSWebView2FrameNavigationDelegate <NSObject>

@optional

/**
 * @abstract Called when the frame is about to navigate.
 *
 * @param frame The frame that is navigating.
 * @param info The frame navigation starting info.
 *
 * @return A <MSWebView2NavigationStartingDecision> object that indicates how to
 * handle the navigation.
 *
 * @discussion A frame navigation will trigger this method and
 * `webView:frameNavigationStarting` of <MSWebView2NavigationDelegate>. Both
 * methods share a common <MSWebView2NavigationStartingInfo> object. The
 * navigation will be blocked until these two methods(if exist) run.
 *
 * The usage of the returned value is the same as
 * `webView:frameNavigationStarting` of `MSWebView2NavigationDelegate`.
 */
- (nullable MSWebView2NavigationStartingDecision*)
                 frame:(MSWebView2Frame*)frame
    navigationStarting:(MSWebView2NavigationStartingInfo*)info;

/**
 * @abstract Called when the navigation has completed.
 *
 * @param frame The frame that completed the navigation.
 * @param info The details about this finished navigation.
 */
- (void)frame:(MSWebView2Frame*)frame
    navigationCompleted:(MSWebView2NavigationCompletedInfo*)info;

@end

NS_ASSUME_NONNULL_END
