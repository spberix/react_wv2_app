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

/**
 * Indicates the error status values for web navigations.
 */
typedef NS_ENUM(NSInteger, MSWebView2WebErrorStatus) {
  /**
   * Indicates that an unknown error occurred.
   */
  MSWebView2WebErrorStatusUnknown = 0,
  /**
   * Indicates that the SSL certificate common name does not match the web
   * address.
   */
  MSWebView2WebErrorStatusCertificateCommonNameIsIncorrect,
  /**
   *  Indicates that the SSL certificate has expired.
   */
  MSWebView2WebErrorStatusCertificateExpired,
  /**
   * Indicates that the SSL client certificate contains errors.
   */
  MSWebView2WebErrorStatusClientCertificateContainsErrors,
  /**
   * Indicates that the SSL certificate has been revoked.
   */
  MSWebView2WebErrorStatusCertificateRevoked,
  /**
   * Indicates that the SSL certificate is not valid.
   */
  MSWebView2WebErrorStatusCertificateIsInvalid,
  /**
   * Indicates that the host is unreachable.
   */
  MSWebView2WebErrorStatusServerUnreachable,
  /**
   * Indicates that the connection has timed out.
   */
  MSWebView2WebErrorStatusTimeout,
  /**
   * Indicates that the server returned an invalid or unrecognized response.
   */
  MSWebView2WebErrorStatusErrorHttpInvalidServerResponse,
  /**
   * Indicates that the connection was stopped.
   */
  MSWebView2WebErrorStatusConnectionAborted,
  /**
   * Indicates that the connection was reset.
   */
  MSWebView2WebErrorStatusConnectionReset,
  /**
   * Indicates that the Internet connection has been lost.
   */
  MSWebView2WebErrorStatusDisconnected,
  /**
   * Indicates that a connection to the destination was not established.
   */
  MSWebView2WebErrorStatusCannotConnect,
  /**
   * Indicates that the provided host name was not able to be resolved.
   */
  MSWebView2WebErrorStatusHostNameNotResolved,
  /**
   *  Indicates that the operation was canceled.
   */
  MSWebView2WebErrorStatusOperationCanceled,
  /**
   *  Indicates that the request redirect failed.
   */
  MSWebView2WebErrorStatusRedirectFailed,
  /**
   * Indicates that an unexpected error occurred.
   */
  MSWebView2WebErrorStatusUnexpectedError,
  /**
   * Indicates that valid authentication credentials required.
   */
  MSWebView2WebErrorStatusValidAuthenticationCredentialsRequired,
  /**
   * Indicates that valid proxy authentication required.
   */
  MSWebView2WebErrorStatusValidProxyAuthenticationRequired,
};

/**
 * Info for `navigationCompleted` event.
 */
@interface MSWebView2NavigationCompletedInfo : NSObject

/**
 * @abstract ID of the navigation.
 */
@property(nonatomic, readonly) uint64_t navigationID;

/**
 * @abstract Whether the navigation was successful.
 */
@property(nonatomic, readonly) BOOL isSuccess;

/**
 * @abstract The error code if the navigation failed.
 */
@property(nonatomic, readonly) MSWebView2WebErrorStatus webErrorStatus;

/**
 * @abstract The HTTP status code of the navigation if it involved an HTTP
 * request.
 *
 * @discussion For instance, this will usually be 200 if the request was
 * successful, 404 if a page was not found, etc. See
 *  https://developer.mozilla.org/docs/Web/HTTP/Status for a list of common
 * status codes.
 *
 * The `HTTPStatusCode` property will be 0 in the following cases:
 * - The navigation did not involve an HTTP request. For instance, if it was a
 * navigation to a file:// URL, or if it was a same-document navigation.
 * - The navigation failed before a response was received. For instance, if the
 * hostname was not found, or if there was a network error.
 *
 * In those cases, you can get more information from the <isSuccess> and
 * <webErrorStatus> properties.
 *
 * If the navigation receives a successful HTTP response, but the navigated page
 * calls `window.stop()` before it finishes loading, then `HTTPStatusCode` may
 * contain a success code like 200, but `isSuccess` will be `NO` and
 * `webErrorStatus` will be `MSWebView2WebErrorStatusConnectionAborted`. Since
 * WebView2 handles HTTP continuations and redirects automatically, it is
 * unlikely for HttpStatusCode to ever be in the 1xx or 3xx ranges.
 */
@property(nonatomic, readonly) int HTTPStatusCode;

@end

NS_ASSUME_NONNULL_END
