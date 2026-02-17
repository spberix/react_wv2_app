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

@class MSWebView2DedicatedWorker;
@class MSWebView2DedicatedWorkerCreatedInfo;
@class MSWebView2WebMessageReceivedInfo;

NS_ASSUME_NONNULL_BEGIN

/**
 * @abstract Delegate protocol for dedicated worker events.
 */
@protocol MSWebView2DedicatedWorkerDelegate <NSObject>

@optional
/**
 * @abstract Called when the dedicated worker is being destroyed.
 * @param worker The worker that is about to be destroyed.
 */
- (void)dedicatedWorkerWillDestroy:(MSWebView2DedicatedWorker*)worker;

/**
 * @abstract Called when a new dedicated worker is created from current worker.
 * @param worker The parent worker that created the new worker.
 * @param info Information about the newly created dedicated worker.
 */
- (void)dedicatedWorker:(MSWebView2DedicatedWorker*)worker
    dedicatedWorkerCreated:(MSWebView2DedicatedWorkerCreatedInfo*)info;

/**
 * @abstract Called when a web message is received from the dedicated worker.
 * @param worker The worker that sent the message.
 * @param info Information about the received web message.
 */
- (void)dedicatedWorker:(MSWebView2DedicatedWorker*)worker
     webMessageReceived:(MSWebView2WebMessageReceivedInfo*)info;

@end

NS_ASSUME_NONNULL_END
