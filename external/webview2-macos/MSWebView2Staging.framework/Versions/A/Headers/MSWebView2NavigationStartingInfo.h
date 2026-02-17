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

#import "MSWebView2HTTPHeaders.h"

NS_ASSUME_NONNULL_BEGIN

/**
 * Indicates the kind of the navigation.
 */
typedef NS_ENUM(NSInteger, MSWebView2NavigationKind) {
  /**
   * Reload navigation kind.
   */
  MSWebView2NavigationKindReload = 0,

  /**
   * Back or forward navigation kind.
   */
  MSWebView2NavigationKindBackOrForward = 1,

  /**
   * New document navigation kind.
   */
  MSWebView2NavigationKindNewDocument = 2,

};

/**
 * Info for `navigationStarting` event.
 */
@interface MSWebView2NavigationStartingInfo : NSObject

/**
 * @abstract ID of the navigation.
 */
@property(nonatomic, readonly) uint64_t navigationID;

/**
 * @abstract URI of the navigation.
 */
@property(nonatomic, copy, readonly) NSString* URI;

/**
 * @abstract Whether the navigation is redirected.
 */
@property(nonatomic, readonly, getter=isRedirected) BOOL redirected;

/**
 * @abstract Return `YES` if the navigation is initiated through a user gesture.
 * Return `NO` if it is a programmatic navigation by page script.
 */
@property(nonatomic, readonly, getter=isUserInitiated) BOOL userInitiated;

/**
 * @abstract Request headers of the navigation.
 *
 * @discussion You are not able to modify the HTTP request headers in a
 * `NavigationStarting` event.
 */
@property(nonatomic, readonly) MSWebView2HTTPHeaders* requestHeaders;

/**
 * @abstract Kind of the navigation.
 */
@property(nonatomic, readonly) MSWebView2NavigationKind navigationKind;

@end

NS_ASSUME_NONNULL_END
