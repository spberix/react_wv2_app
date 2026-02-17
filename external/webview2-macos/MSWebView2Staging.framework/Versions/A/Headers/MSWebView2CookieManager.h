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

#import "MSWebView2Cookie.h"

NS_ASSUME_NONNULL_BEGIN

/**
 * Creates, adds or updates, gets, or view the cookies.
 */
@interface MSWebView2CookieManager : NSObject

- (instancetype)init NS_UNAVAILABLE;

/**
 * @abstract Creates a cookie whose params matches those of the specified
 * cookie.
 *
 * @param name Cookie name. Leading or trailing whitespace(s), empty string,
 * and special characters are not allowed for name.
 * @param value Cookie value.
 * @param domain The domain for which the cookie is valid. It can be `nil`, then
 * the default value will be used which is an empty string.
 * @param path The path for which the cookie is valid. It can be `nil`, then the
 * default value "/" will be used.
 *
 * @return Returns the created cookie <MSWebView2Cookie> object. Returns `nil`
 * if input argument is invalid.
 *
 * @discussion One can set other optional properties after cookie creation.
 * This only creates a cookie object and it is not added to the cookie manager
 * until you call <addOrUpdateCookie>
 */
- (MSWebView2Cookie*)createCookieWithName:(NSString*)name
                                    value:(NSString*)value
                                   domain:(nullable NSString*)domain
                                     path:(nullable NSString*)path;

/**
 * @abstract Adds or updates a cookie with the given cookie data; may overwrite
 * cookies with matching name, domain, and path if they exist.
 *
 * @param cookie The cookie will be added.
 *
 * @textblock
 * void SetRememberedUserId(NSString * id) {
 *   MSWebView2CookieManager *cookieManager = webview.profile.cookieManager;
 *   MSWebView2Cookie *cookie =
 *       [cookieManager createCookieWithName:@"last_userid"
 *                                     value:id
 *                                    domain:@"contoso.com"
 *                                      path:@"/"];
 *   cookie.secure = YES;
 *   [cookieManager addOrUpdateCookie:cookie];
 * }
 * @/textblock
 */
- (void)addOrUpdateCookie:(MSWebView2Cookie*)cookie;

/**
 * @abstract Gets a list of cookies matching the specific URI.
 *
 * @param URI The URI used to query. If URI is an empty string or `nil`, all
 * cookies under the same profile are returned.
 * @param completionHandler A block to invoke when get cookies finished.
 *
 * @discussion You can modify the cookie objects by calling
 * `addOrUpdateCookie:`, and the changes will be applied to the WebView.
 * @textblock
 * void IsUserSignedInToContoso() {
 *   __block BOOL isSigned = NO;
 *   [webview.profile.cookieManager
 *       getCookiesWithURI:@"https://contoso.com"
 *       completionHandler:^(NSArray<MSWebView2Cookie*>* cookies,
 *                           NSError* err) {
 *         for (MSWebView2Cookie* cookie in cookies) {
 *           if ([cookie.name isEqualToString:@"is_signed_in"]) {
 *             isSigned = [cookie.value isEqualToString:@"1"];
 *             return;
 *           }
 *         }
 *       }];
 * }
 * @/textblock
 */
- (void)getCookiesWithURI:(nullable NSString*)URI
        completionHandler:(void (^)(NSArray<MSWebView2Cookie*>* cookies,
                                    NSError* _Nullable error))completionHandler;

/**
 * @abstract Deletes all cookies under the same profile.
 *
 * @discussion This could affect other WebViews under the same user profile.
 */
- (void)deleteAllCookies;

/**
 * @abstract Deletes the cookie with matching name and domain/path pair.
 *
 * @param name The name of the cookie to be deleted.
 * @param domain The domain of the cookie to be deleted.
 * @param path The path of the cookie to be deleted.
 *
 * @textblock
 * void ClearRememberedUserId() {
 *   MSWebView2CookieManager *cookieManager = webview.profile.cookieManager;
 *   [cookieManager deleteCookiesWithName:@"last_userid"
 *                                 domain:@"https://contoso.com"
 *                                   path:@"/"];
 * }
 * @/textblock
 */
- (void)deleteCookiesWithName:(NSString*)name
                       domain:(NSString*)domain
                         path:(NSString*)path;

/**
 * @abstract Deletes the cookie.
 *
 * @param cookie The cookie to be deleted.
 *
 * @discussion It will only use name, domain and path to match the cookie.
 * @textblock
 * void ClearNonContosoCookies() {
 *   MSWebView2CookieManager *cookieManager = webview.profile.cookieManager;
 *   [cookieManager
 *       getCookiesWithURI:@""
 *       completionHandler:^(NSArray<MSWebView2Cookie *> *cookies,
 *                           NSError *err) {
 *         for (MSWebView2Cookie *cookie in cookies) {
 *           if (![cookie.domain isEqualToString:@"contoso.com"]) {
 *             [cookieManager deleteCookie:cookie];
 *           }
 *         }
 *       }];
 * }
 * @/textblock
 */
- (void)deleteCookie:(MSWebView2Cookie*)cookie;

/**
 * @abstract Deletes cookie with matching name and URI.
 *
 * @param name The name of the cookie to be deleted.
 * @param URI The URI of the cookie to be deleted.
 *
 * @discussion Cookie with the given name where domain and path match provided
 * URI are deleted.
 */
- (void)deleteCookiesWithName:(NSString*)name URI:(NSString*)URI;

@end

NS_ASSUME_NONNULL_END
