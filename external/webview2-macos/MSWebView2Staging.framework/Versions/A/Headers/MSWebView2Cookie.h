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
 * Kind of cookie SameSite status used in the MSWebView2Cookie class. These
 * fields match those as specified in
 * https://developer.mozilla.org/docs/Web/HTTP/Cookies#. Learn more about
 * SameSite cookies here:
 * https://tools.ietf.org/html/draft-west-first-party-cookies-07
 */
typedef NS_ENUM(NSInteger, MSWebView2CookieSameSiteKind) {
  /**
   * None SameSite type. No restrictions on cross-site requests.
   */
  MSWebView2CookieSameSiteKindNone,
  /**
   * Lax SameSite type. The cookie will be sent with "same-site" requests, and
   * with "cross-site" top level navigation.
   */
  MSWebView2CookieSameSiteKindLax,
  /**
   * Strict SameSite type. The cookie will only be sent along with "same-site"
   * requests.
   */
  MSWebView2CookieSameSiteKindStrict,
};

/**
 * @abstract Provides a set of properties that are used to manage an cookie
 * object.
 */
@interface MSWebView2Cookie : NSObject <NSCopying>

- (instancetype)init NS_UNAVAILABLE;

/**
 * @abstract Cookie name.
 */
@property(nonatomic, copy, readonly) NSString* name;

/**
 * @abstract Cookie value.
 */
@property(nonatomic, copy) NSString* value;

/**
 * @abstract The domain for which the cookie is valid.
 *
 * @discussion The default is the host that this cookie has been received from.
 * Note that, for instance, ".bing.com", "bing.com", and "www.bing.com" are
 * considered different domains.
 */
@property(nonatomic, copy, readonly) NSString* domain;

/**
 * @abstract The path for which the cookie is valid.
 *
 * @discussion The default is "/", which means this cookie will be sent to all
 * pages on the Domain.
 */
@property(nonatomic, copy, readonly) NSString* path;

/**
 * @abstract The expiration date and time for the cookie.
 *
 * @discussion The default is UNIX epoch, if the date is less than the current
 * time it will be treated as session cookies.
 */
@property(nonatomic) NSDate* expires;

/**
 * @abstract Whether this cookie is http-only.
 *
 * @discussion `YES` if a page script or other active content cannot access this
 * cookie. The default is `NO`.
 */
@property(nonatomic, getter=isHttpOnly) BOOL httpOnly;

/**
 * @abstract The security level of this cookie.
 *
 * @discussion 'YES' if the client is only to return the cookie in subsequent
 * requests if those requests use HTTPS. The default is `NO`. Note that cookie
 * that requests MSWebView2CookieSameSiteKindNone but is not marked Secure will
 * be rejected.
 */
@property(nonatomic, getter=isSecure) BOOL secure;

/**
 * @abstract Whether this is a session cookie.
 *
 * @discussion The default is `YES`. If <expires> is less than the current date,
 * then it is a session cookie.
 */
@property(nonatomic, readonly) BOOL isSession;

/**
 * @abstract SameSite status of the cookie which represents the enforcement mode
 * of the cookie.
 *
 * @discussion The default is `MSWebView2CookieSameSiteKindLax`.
 */
@property(nonatomic) MSWebView2CookieSameSiteKind sameSite;

@end

NS_ASSUME_NONNULL_END
