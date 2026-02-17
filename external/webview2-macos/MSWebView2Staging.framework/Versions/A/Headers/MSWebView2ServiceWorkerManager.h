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

#import "MSWebView2ServiceWorkerManagerDelegate.h"
#import "MSWebView2ServiceWorkerRegistration.h"

NS_ASSUME_NONNULL_BEGIN

/**
 * @abstract Completion handler for getting service worker registrations.
 * @param registrations An array of MSWebView2ServiceWorkerRegistration objects,
 * or empty array if none exist.
 * @param error An error if the operation failed, or nil on success.
 */
typedef void (^MSWebView2GetServiceWorkerRegistrationsCompletionHandler)(
    NSArray<MSWebView2ServiceWorkerRegistration*>* _Nullable registrations,
    NSError* _Nullable error);

/**
 * @abstract This interface manages registrations for service workers in
 * WebView2.
 *
 * @discussion A ServiceWorker is a specific type of worker that takes a
 * JavaScript file that can control the web-page/site that it is associated
 * with, intercepting and modifying navigation and resource requests, and
 * caching resources in a very granular fashion to give you complete control
 * over how app behaves in certain situations.
 *
 * Service workers essentially act as proxy servers that sit between web
 * applications, the browser, and the network (when available). They run in a
 * different context from the web page, which means they have no direct access
 * to the DOM. Unlike dedicated and shared workers, which may have direct access
 * to a global scope shared with other scripts, service workers operate in
 * isolation from the DOM, ensuring a more secure and controlled environment.
 */
@interface MSWebView2ServiceWorkerManager : NSObject

/**
 * @abstract The delegate that receives service worker manager events.
 */
@property(nullable, nonatomic, weak) id<MSWebView2ServiceWorkerManagerDelegate>
    delegate;

/**
 * @abstract Gets all service worker registrations asynchronously.
 *
 * @param completionHandler A block to be called when the operation completes.
 * The block receives an array of service worker registrations and an error
 * object if an error occurred.
 *
 * @discussion This method returns a list of
 * `MSWebView2ServiceWorkerRegistration` objects, each representing a service
 * worker registration.
 *
 * This method corresponds to the `getRegistrations` method of the
 * `ServiceWorkerContainer` object in the DOM which returns a Promise that
 * resolves to an array of `ServiceWorkerRegistration` objects. See the
 * [MDN
 * documentation](https://developer.mozilla.org/docs/Web/API/ServiceWorkerContainer/getRegistrations)
 * for more information.
 *
 * Note: If this function is called from different webviews, even with the same
 * profile, the returned registrations are not the same instances.
 */
- (void)getServiceWorkerRegistrationsWithCompletionHandler:
    (MSWebView2GetServiceWorkerRegistrationsCompletionHandler)completionHandler;

/**
 * @abstract Gets service worker registrations for a specific scope URI.
 *
 * @param scopeUri The scope URI to filter registrations.
 * @param completionHandler A block to be called when the operation completes.
 * The block receives an array of service worker registrations matching the
 * scope and an error object if an error occurred.
 *
 * @discussion If a service worker has been registered for the given scope, it
 * gets the list of `MSWebView2ServiceWorkerRegistration` objects otherwise
 * returns empty list.
 *
 * If the service worker is registered with a `scope` of '/app/' for an
 * application at https://example.com/, you should specify the full qualified
 * URI i.e., https://example.com/app/ when calling this method. If the scope was
 * not explicitly specified during registration, you should use the directory
 * where the service worker script resides, for example,
 * https://example.com/app/.
 *
 * The provided `scopeUri` is normalized. For example, `HTTPS://münchen.de/`
 * will be normalized to `https://xn--mnchen-3ya.de/` for comparison.
 *
 * This corresponds to the `getRegistration` method of the
 * `ServiceWorkerContainer` object in the DOM which returns a Promise that
 * resolves to a `ServiceWorkerRegistration` object. See the
 * [MDN
 * documentation](https://developer.mozilla.org/docs/Web/API/ServiceWorkerContainer/getRegistration)
 * for more information.
 *
 * If scopeUri is empty string or null or invalid, NSInvalidArgumentException
 * will be raised.
 *
 * If no registrations match the scope, an empty array is returned.
 *
 * Note: If this function is called from different webviews, even with the same
 * profile, the returned registrations are not the same instances.
 *
 */
- (void)
    getServiceWorkerRegistrationsForScope:(NSString*)scopeUri
                        completionHandler:
                            (MSWebView2GetServiceWorkerRegistrationsCompletionHandler)
                                completionHandler;

@end

NS_ASSUME_NONNULL_END
