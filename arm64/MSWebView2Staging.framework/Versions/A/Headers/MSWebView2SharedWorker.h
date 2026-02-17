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

#import "MSWebView2SharedWorkerDelegate.h"

NS_ASSUME_NONNULL_BEGIN

/**
 * @abstract This interface represents a shared worker in WebView2 and provides
 * methods and properties for interacting with it, such as listening to
 * destroying events, getting the script URI, origin, and top-level origin of
 * the worker etc.
 */
@interface MSWebView2SharedWorker : NSObject

/**
 * @abstract A string representing the Uri of the script that the worker is
 * executing.
 *
 * @discussion The scriptUri is a fully qualified URI, including the scheme,
 * host, and path. In contrast, the scriptURL property of the Worker object in
 * the DOM returns the relative URL of the script being executed by the worker.
 * For more details on DOM API, see the [DOM API
 * documentation](https://developer.mozilla.org/docs/Web/API/Worker/scriptURL).
 *
 * Refer to the Host Name Canonicalization for details on how normalization is
 * performed. The same process applies to the scriptURL when a worker is created
 * from DOM API. The scriptUri property reflects this normalization, ensuring
 * that the URL is standardized. For example, `HTTPS://EXAMPLE.COM/worker.js` is
 * canonicalized to `https://example.com/worker.js`;
 * `https://bücher.de/worker.js` is canonicalized to
 * `https://xn--bcher-kva.de/worker.js`.
 */
@property(readonly, nonatomic, copy) NSString* scriptUri;

/**
 * @abstract A string representing the URI of the origin where the worker is
 * executing.
 *
 * @discussion If a worker created with scriptUri:
 * https://example.com/worker.js, the origin will be https://example.com/.
 *
 * Refer to the Host Name Canonicalization for details on how normalization is
 * performed.
 */
@property(readonly, nonatomic, copy) NSString* origin;

/**
 * @abstract A string representing the URI of the top-level document that the
 * worker is associated with.
 *
 * @discussion If a worker is created with scriptUri:
 * https://example.com/worker.js, the top-level origin is https://example.com/.
 * If the same worker is created from a iframe at https://example.com/ which is
 * hosted on https://example2.com/, the top-level origin is
 * https://example2.com/.
 *
 * Refer to the Host Name Canonicalization for details on how normalization is
 * performed.
 *
 * When CustomDataPartitionId is set, the topLevelOrigin will be a generated
 * site like guid.invalid. For example, if the top-level document is
 * https://example.com/worker.js, the top-level origin will be
 * `https://guid.invalid/`.
 */
@property(readonly, nonatomic, copy) NSString* topLevelOrigin;

/**
 * @abstract The delegate that receives shared worker events.
 */
@property(nullable, nonatomic, weak) id<MSWebView2SharedWorkerDelegate>
    delegate;

@end

NS_ASSUME_NONNULL_END
