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

#import "MSWebView2WindowFeatures.h"

NS_ASSUME_NONNULL_BEGIN

/**
 * Info for the MSWebView2NewWindowDelegate.
 */
@interface MSWebView2NewWindowRequestedInfo : NSObject

/**
 * @abstract The target URL of the new window requested.
 */
@property(nonatomic, readonly, copy) NSString* URL;

/**
 * @abstract YES when the new window request was initiated through a user
 * gesture such as clicking an anchor tag with target.
 */
@property(nonatomic, readonly, getter=isUserInitiated) BOOL userInitiated;

/**
 * @abstract The window features that should be considered for positioning and
 * sizing of new WebView window requested.
 */
@property(nonatomic, readonly, strong) MSWebView2WindowFeatures* windowFeatures;

/**
 * @abstract The name of the new WebView window requested.
 */
@property(nonatomic, readonly, copy) NSString* name;

@end

NS_ASSUME_NONNULL_END
