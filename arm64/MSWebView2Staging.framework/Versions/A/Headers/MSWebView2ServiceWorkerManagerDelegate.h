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

@class MSWebView2ServiceWorkerManager;
@class MSWebView2ServiceWorkerRegistrationInfo;

NS_ASSUME_NONNULL_BEGIN

/**
 * @abstract Delegate protocol for service worker manager events.
 */
@protocol MSWebView2ServiceWorkerManagerDelegate <NSObject>

@optional
/**
 * @abstract Called when a new service worker registration is created.
 *
 * @param manager The service worker manager.
 * @param info Information about the newly created service worker registration.
 *
 * @discussion This event is raised when a web application registers a service
 * worker using the `navigator.serviceWorker.register("/sw.js")` method. See the
 * [Service Worker
 * Registration](https://developer.mozilla.org/docs/Web/API/ServiceWorkerRegistration)
 * for more information.
 */
- (void)serviceWorkerManager:(MSWebView2ServiceWorkerManager*)manager
     serviceWorkerRegistered:(MSWebView2ServiceWorkerRegistrationInfo*)info;

@end

NS_ASSUME_NONNULL_END
