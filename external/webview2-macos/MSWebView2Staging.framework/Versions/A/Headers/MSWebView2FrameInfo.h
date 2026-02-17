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

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSInteger, MSWebView2FrameKind) {
  /**
   * Specifies that the frame is type of frame we don't differentiate.
   */
  MSWebView2FrameKindOtherFrame = 0,
  /**
   * Specifies that the frame is a primary main frame(WebView).
   */
  MSWebView2FrameKindMainFrame,
  /**
   * Specifies that the frame is an iframe.
   */
  MSWebView2FrameKindIFrame,
  /**
   * Specifies that the frame is an embed element.
   */
  MSWebView2FrameKindEmbed,
  /**
   * Specifies that the frame is an object element.
   */
  MSWebView2FrameKindObject
};

/**
 * Frame info in an Out-of-Process iframes (OOPIFs) render process.
 */
@interface MSWebView2FrameInfo : NSObject

/**
 * @abstract The unique identifier of the frame associated with the current
 * `FrameInfo`.
 *
 * @discussion It's the same kind of ID as with the frame ID in
 * <MSWebView2WebView> and <MSWebView2Frame>. This is only available when
 * it's called from `getProcessInfoDetailsWithCompletionHandler:`. Else, it
 * returns an invalid frame ID `0`. Note that this frame ID could be out of date
 * as it's a snapshot.
 */
@property(nonatomic, readonly) NSInteger frameID;

/**
 * @abstract The name attribute of the frame, as in `<iframe name="frame-name"
 * ...>`.
 *
 * @discussion It is @"" when the frame has no name attribute.
 */
@property(nonatomic, copy, readonly) NSString* name;

/**
 * @abstract The URI of the frame.
 */
@property(nonatomic, copy, readonly) NSString* source;

/**
 * @abstract The frame kind of the frame.
 */
@property(nonatomic, readonly) MSWebView2FrameKind kind;

/**
 * @abstract The parent `FrameInfo`.
 *
 * @discussion This property is `nil` for the top most document in the WebView2
 * which has no parent frame. This is only available when it's called from
 * `getProcessInfoDetailsWithCompletionHandler:`. Else, it returns `nil`. Note
 * that this parent frame info could be out of date as it's a snapshot.
 */
@property(nonatomic, readonly, nullable) MSWebView2FrameInfo* parentFrame;

@end

NS_ASSUME_NONNULL_END
