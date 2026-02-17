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

#import "MSWebView2SharedWorker.h"
#import "MSWebView2SharedWorkerManagerDelegate.h"

NS_ASSUME_NONNULL_BEGIN

/**
 * @abstract Completion handler for getting shared workers.
 * @param sharedWorkers An array of MSWebView2SharedWorker objects, or empty
 * array if none exist.
 * @param error An error if the operation failed, or nil on success.
 */
typedef void (^MSWebView2GetSharedWorkersCompletionHandler)(
    NSArray<MSWebView2SharedWorker*>* sharedWorkers,
    NSError* _Nullable error);

/**
 * @abstract This interface manages shared workers in WebView2.
 */
@interface MSWebView2SharedWorkerManager : NSObject

/**
 * @abstract Gets a list of the shared workers created under the same profile.
 *
 * @param completionHandler A block that receives an array of
 * MSWebView2SharedWorker objects and an error if the operation failed.
 */
- (void)getSharedWorkersWithCompletionHandler:
    (MSWebView2GetSharedWorkersCompletionHandler)completionHandler;

/**
 * @abstract The delegate that receives events about the shared workers which
 * are managed by this manager.
 */
@property(nullable, nonatomic, weak) id<MSWebView2SharedWorkerManagerDelegate>
    delegate;

@end

NS_ASSUME_NONNULL_END
