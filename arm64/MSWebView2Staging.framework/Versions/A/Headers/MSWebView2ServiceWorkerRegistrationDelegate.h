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

@class MSWebView2ServiceWorkerRegistration;
@class MSWebView2ServiceWorker;

NS_ASSUME_NONNULL_BEGIN

/**
 * @abstract Delegate protocol for service worker registration events.
 */
@protocol MSWebView2ServiceWorkerRegistrationDelegate <NSObject>

@optional
/**
 * @abstract Called when a service worker becomes activated.
 *
 * @param registration The service worker registration.
 * @param serviceWorker The service worker that was activated.
 *
 * @discussion This event is raised when a service worker is activated. A
 * service worker is activated when its script has been successfully registered
 * and it is ready to control the pages within the scope of the registration.
 *
 * This event is also raised when an updated version of a service worker reaches
 * the active state. In such a case, the existing MSWebView2ServiceWorker object
 * is destroyed, and this event is raised with a new MSWebView2ServiceWorker
 * object representing the updated service worker. The active service worker is
 * the one that receives fetch and message events for the pages it controls. See
 * the
 * [Service
 * Worker](https://developer.mozilla.org/en-US/docs/Web/API/ServiceWorkerRegistration/active)
 * documentation for more information.
 *
 * If you register for the `serviceWorkerRegistration:serviceWorkerActivated:`
 * delegate method and the registration already has an active worker, the method
 * is not called immediately. Instead, it waits for the next activation event to
 * occur. Therefore, you should first check if an active service worker is
 * running by using the `activeServiceWorker` property.
 */
- (void)serviceWorkerRegistration:
            (MSWebView2ServiceWorkerRegistration*)registration
           serviceWorkerActivated:(MSWebView2ServiceWorker*)serviceWorker;

/**
 * @abstract Called when a service worker registration is being unregistered.
 *
 * @param registration The service worker registration that is being
 * unregistered.
 *
 * @discussion This event is raised when the worker registration is unregistered
 * using the JS API `registration.unregister()`. See the
 * [Unregister](https://developer.mozilla.org/docs/Web/API/ServiceWorkerRegistration/unregister)
 * for more information.
 */
- (void)serviceWorkerRegistrationUnregistering:
    (MSWebView2ServiceWorkerRegistration*)registration;

@end

NS_ASSUME_NONNULL_END
