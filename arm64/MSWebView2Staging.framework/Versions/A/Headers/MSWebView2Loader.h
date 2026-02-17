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

#import "MSWebView2Environment.h"
#import "MSWebView2EnvironmentOptions.h"
#import "MSWebView2Foundation.h"

/**
 * @abstract A block that indicates WebView environment creation result.
 *
 * @param environment A MSWebView2Environment object created.
 * @param error A NSError occured when create WebView environment.
 */
typedef void (^CreateMSWebView2EnvironmentCompletionHandler)(
    MSWebView2Environment* _Nullable environment,
    NSError* _Nullable error);

/**
 * @abstract Create WebView environment.
 *
 * @param browserExecutableFolder The folder contains the WebView2 Runtime.
 * @param userDataFolder The user data folder used for storing user data, it can
 * be `nil`.
 * @param options Other MSWebView2EnvironmentOptions options, it can be `nil`
 * @param completionHandler A block to invoke when WebView environment creation
 * completes or fails.
 *
 * @discussion Ideally, the `browserExecutableFolder` could be `nil` to use the
 * default path: `~/Library/Application Support/Microsoft WebView2/Microsoft
 * Edge WebView2.app`.
 *
 * The `userDataFolder` could be `nil`, the default user data folder will be
 * `~/Library/Group Containers/{appgroupidentifier}` if sandbox is enabled, and
 * `~/Library/Application Support/{bundleid}/{bundlefullpathhash}` if
 * sandbox is disabled.
 *
 * - `appgroupidentifier` is the value of
 *   `com.apple.security.application-groups` of your application.
 * - `bundleid` is the value of `CFBundleIdentifier` of your application.
 * - `bundlefullpathhash` is the hash value of your application path.
 *
 * The `completionHandler` pass the result of WebView environment
 * creation or an error.
 *
 * As a browser process may be shared among WebViews, WebView creation fails
 * with `MSWebView2ErrorInvalidState` if the specified options does not match
 * the options of the WebViews that are currently running in the shared browser
 * process.
 *
 * The `browserExecutableFolder`, `userDataFolder` and
 * `additionalBrowserArguments` of the `options` may be overridden
 * by values specified in system environment variables.
 *
 * When creating a `MSWebView2Environment` the following system environment
 * variables are verified.
 *
 * ```text
 * WEBVIEW2_BROWSER_EXECUTABLE_FOLDER
 * WEBVIEW2_USER_DATA_FOLDER
 * WEBVIEW2_ADDITIONAL_BROWSER_ARGUMENTS
 * ```
 * If find an override system environment variable,
 * `CreateMSWebView2Environment` will replace `browserExecutableFolder` and
 * `userDataFolder` values with the environment variable.  If
 * `additionalBrowserArguments` is specified in environment variable, it is
 * appended to the corresponding values in `CreateMSWebView2Environment`
 * parameters.
 *
 * For example:
 * @textblock
 * MSWebView2EnvironmentOptions* options =
 *     [[MSWebView2EnvironmentOptions alloc] init];
 * options.additionalBrowserArguments =
 *     @[ @"--enable-features=msWebView2TextureStream" ];
 * CreateMSWebView2Environment(
 *     @"path/to/webview2/executable/Microsoft Edge WebView2.app",
 *     @"path/to/user/data/folder",
 *     options,
 *     ^(MSWebView2Environment* environment, NSError* error) {
 *         if (error) {
 *           // Deal with the error
 *         } else {
 *           // Use the environment to create WebView controller
 *         }
 *     });
 * @/textblock
 *
 * @exception NSInvalidArgumentException Thrown if the WebView2 Runtime version
 * is not compatible with that of `MSWebView2.framework`, and also thrown when
 * `browserExecutableFolder` is `nil`, but the system does not install the
 * WebView2 environment.
 */
MS_WEBVIEW2_EXTERN
void CreateMSWebView2Environment(
    NSString* _Nullable browserExecutableFolder,
    NSString* _Nullable userDataFolder,
    MSWebView2EnvironmentOptions* _Nullable options,
    CreateMSWebView2EnvironmentCompletionHandler _Nonnull completionHandler);
