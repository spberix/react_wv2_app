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

@class MSWebView2Frame;
@class MSWebView2DedicatedWorkerCreatedInfo;

/**
 * Delegate for the frame event.
 */
@protocol MSWebView2FrameDelegate <NSObject>

@optional

/**
 * @abstract Called when the frame is destroyed.
 *
 * @param frame The frame that is destroyed.
 *
 * @discussion The `FrameDestroyed` event is raised when the
 * iframe corresponding to this `MSWebView2Frame` object is
 * removed or the document containing that iframe is destroyed. When
 * this event is raised, the `MSWebView2Frame` object has been
 * destroyed and cannot be used anymore.
 */
- (void)frameDestroyed:(MSWebView2Frame*)frame;

/**
 * @abstract Called when a dedicated worker is created in an iframe.
 *
 * @param frame The frame that the dedicated worker is created in.
 * @param info The dedicated worker created info.
 */
- (void)frame:(MSWebView2Frame*)frame
    dedicatedWorkerCreated:(MSWebView2DedicatedWorkerCreatedInfo*)info;

@end

NS_ASSUME_NONNULL_END
