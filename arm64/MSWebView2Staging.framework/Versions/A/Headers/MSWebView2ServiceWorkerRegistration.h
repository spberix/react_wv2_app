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

#import "MSWebView2ServiceWorker.h"
#import "MSWebView2ServiceWorkerRegistrationDelegate.h"

NS_ASSUME_NONNULL_BEGIN

/**
 * @abstract This interface represents a service worker registration in WebView2
 * and provides methods and properties for interacting with it, such as getting
 * the scope uri, active service worker, listening for service worker
 * activation, unregistering events etc.
 */
@interface MSWebView2ServiceWorkerRegistration : NSObject

/**
 * @abstract The scope URI of the service worker registration.
 *
 * @discussion The `scopeUri` is a fully qualified URI, including the scheme,
 * host and path, that specifies the range of URLs a service worker can control.
 *
 * When registering a service worker, if no scope is specified, it defaults to
 * the directory where the service worker script resides. For example, if the
 * script is located at https://example.com/app/sw.js, the default `scopeUri`
 * would be https://example.com/app/. However, if a scope is provided, it is
 * defined relative to the application's base URI. For instance, if an
 * application at https://example.com/ registers a service worker with a scope
 * of /app/, the resulting `scopeUri` is https://example.com/app/.
 *
 * The `scopeUri` property reflects normalization, ensuring that the URI is
 * standardized. For example, `HTTPS://EXAMPLE.COM/app/` is canonicalized to
 * `https://example.com/app/`; `https://bücher.de/` is canonicalized to
 * `https://xn--bcher-kva.de/`.
 *
 * The `scope` property of the `ServiceWorkerRegistration` object in the DOM
 * returns the relative URL based on the application's base URI, while this
 * property always returns a fully qualified URI. For more information on DOM
 * API, see the
 * [MDN
 * documentation](https://developer.mozilla.org/docs/Web/API/ServiceWorkerRegistration/scope).
 */
@property(readonly, nonatomic, copy) NSString* scopeUri;

/**
 * @abstract The origin of the service worker.
 *
 * @discussion A string representing the URI of the origin where the worker is
 * executing.
 *
 * If a worker is created with `scriptUri`: https://example.com/worker.js,
 * the origin will be https://example.com/.
 */
@property(readonly, nonatomic, copy) NSString* origin;

/**
 * @abstract The top-level origin associated with the service worker.
 *
 * @discussion A string representing the URI of the top-level document that the
 * worker is associated with.
 *
 * If a worker is created with `scriptUri`: https://example.com/worker.js,
 * the top-level origin is https://example.com/. If the same worker is created
 * from an iframe at https://example.com/ which is hosted on
 * https://example2.com/, the top-level origin is https://example2.com/.
 *
 * When CustomDataPartitionId is set, the `topLevelOrigin` will be a generated
 * site like guid.invalid. For example, if the top-level document is
 * https://example.com/worker.js, the top-level origin will be
 * `https://guid.invalid/`.
 */
@property(readonly, nonatomic, copy) NSString* topLevelOrigin;

/**
 * @abstract The active service worker, or nil if there is no active service
 * worker.
 *
 * @discussion The active service worker is the service worker that controls the
 * pages within the scope of the registration. See the
 * [Service Worker](https://developer.mozilla.org/docs/Web/API/ServiceWorker)
 * for more information.
 *
 * This corresponds to the `active` property of the `ServiceWorkerRegistration`
 * object in the DOM. For more information, see the
 * [MDN
 * documentation](https://developer.mozilla.org/docs/Web/API/ServiceWorkerRegistration/active).
 */
@property(readonly, nonatomic, strong, nullable)
    MSWebView2ServiceWorker* activeServiceWorker;

/**
 * @abstract The delegate that receives service worker registration events.
 */
@property(nullable, nonatomic, weak)
    id<MSWebView2ServiceWorkerRegistrationDelegate>
        delegate;

@end

NS_ASSUME_NONNULL_END
