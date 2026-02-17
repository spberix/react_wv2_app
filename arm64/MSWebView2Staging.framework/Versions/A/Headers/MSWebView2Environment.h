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

#import <Cocoa/Cocoa.h>

#import "MSWebView2ContextMenuItem.h"
#import "MSWebView2Controller.h"
#import "MSWebView2ControllerOptions.h"
#import "MSWebView2EnvironmentDelegate.h"
#import "MSWebView2ProcessInfo.h"

NS_ASSUME_NONNULL_BEGIN

/**
 * @abstract A block that indicates WebView controller creation result.
 *
 * @param controller A MSWebView2Controller object created.
 * @param error A NSError occured when creating WebView2 controller.
 */
typedef void (^CreateMSWebView2ControllerCompletionHandler)(
    MSWebView2Controller* _Nullable controller,
    NSError* _Nullable error);

/**
 * Represents the WebView2 Environment.
 */
@interface MSWebView2Environment : NSObject

- (instancetype)init NS_UNAVAILABLE;

/**
 * @abstract Create WebView2 controller.
 *
 * @param hostView The NSView object to host WebView contents.
 * @param completionHandler A block to invoke when WebView2 controller creation
 * completes or fails.
 *
 * @discussion Please see `createWebViewController:options:completionHandler:`
 * for details.
 */
- (void)createWebViewController:(NSView*)hostView
              completionHandler:(CreateMSWebView2ControllerCompletionHandler)
                                    completionHandler;

/**
 * @abstract Create WebView2 controller.
 *
 * @param hostView The NSView object to host WebView contents.
 * @param options The Configuration of create WebView2 controller, it can be
 * `nil`.
 * @param completionHandler A block to invoke when WebView2 controller creation
 * completes or fails.
 *
 * @discussion The `completionHandler` is passed the result of WebView2
 * controller creation or an error.
 *
 * `hostView` is the parentView of all WebView2 views. It must be directly or
 * indirectly attached to a NSWindow.
 *
 * It can also accept a <MSWebView2ControllerOptions> as the second parameter
 * for multiple profiles support. As WebView2 is built on top of Edge browser,
 * it follows Edge's behavior pattern. To create an InPrivate WebView, we gets
 * an off-the-record profile (an InPrivate profile) from a regular profile,
 * then create the WebView with the off-the-record profile. Multiple profiles
 * under single user data directory can share some system resources including
 * memory, CPU footprint, disk space (such as compiled shaders and safebrowsing
 * data) etc.
 *
 * The controller can only be created on the main thread because the UIKit is
 * not a thread-safe class on MacOS.
 *
 * WebView creation fails if a running instance using the same user data folder
 * exists, and the Environment objects have different
 * <MSWebView2EnvironmentOptions>. For example, if a WebView was created with
 * one Language, an attempt to create a WebView2 with a different Language
 * using the same user data folder fails.
 *
 * If a profile marked for deletion has not been deleted yet, attempting to
 * create a WebView using same profile name will fail and `completionHandler`
 * will get `MSWebView2ErrorProfileDeletePending` error code.
 *
 * @textblock
 * MSWebView2ControllerOptions* options =
 *     [[MSWebView2ControllerOptions alloc] init];
 * options.profileName = @"my_profile";
 * [self.environment
 *     createWebViewController:self.webContainerView
 *                     options:options
 *           completionHandler:^(MSWebView2Controller* controller,
 *                               NSError* error) {
 *             if (error) {
 *               NSLog(@"Create WebView2 controller failed with error: %@",
 *                     error);
 *             } else {
 *               [controller.webview navigate:@"https://www.bing.com"];
 *             }
 *           }];
 * @/textblock
 *
 * @exception NSInvalidArgumentException Thrown if the `profileName` of
 * <MSWebView2ControllerOptions> is invalid.
 */
- (void)createWebViewController:(NSView*)hostView
                        options:(nullable MSWebView2ControllerOptions*)options
              completionHandler:(CreateMSWebView2ControllerCompletionHandler)
                                    completionHandler;

/**
 * @abstract Create texture stream with a stream id.
 *
 * @param streamID The texture stream id.
 * @param completionHandler the result of create texture stream.
 *
 * @discussion The texture stream with stream id must be created before calling
 * JavaScript API `getTextureStream`.
 *
 * @exception NSInvalidArgumentException Thrown if `streamID` is already used.
 * @exception NSGenericException Thrown if there is no activated WebView.
 */
- (void)createTextureStream:(NSString*)streamID
          completionHandler:(void (^)(MSWebView2TextureStream* textureStream))
                                completionHandler;

/**
 * @abstract Create a <MSWebView2WebResourceResponse> object.
 *
 * @param content The HTTP response content.
 * @param statusCode The HTTP response status code.
 * @param reasonPhrase The HTTP response reason phrase.
 * @param headers The raw response header string delimited by CRLF(optional in
 * last header).
 *
 * @return A new MSWebView2WebResourceResponse object.
 *
 * @discussion It's also possible to create this object with `nil` headers
 * string and then use the <MSWebView2HTTPHeaders> to construct the headers line
 * by line.
 */
- (MSWebView2WebResourceResponse*)
    createWebResourceResponse:(nullable NSData*)content
                   statusCode:(int)statusCode
                 reasonPhrase:(nullable NSString*)reasonPhrase
                      headers:(nullable NSString*)headers;

/**
 * @abstract Create a <MSWebView2WebResourceRequest> object.
 *
 * @param URI The HTTP request URI.
 * @param method The HTTP request method.
 * @param postData The HTTP request body.
 * @param headers The raw request header string delimited by CRLF(optional in
 * last header).
 *
 * @return A new MSWebView2WebResourceRequest object.
 *
 * @discussion `URI` parameter must be absolute URI. It's also possible to
 * create this object with `nil` headers string and then use the
 * <MSWebView2HTTPHeaders> to construct the headers line by line.
 */
- (MSWebView2WebResourceRequest*)
    createWebResourceRequest:(nullable NSString*)URI
                      method:(nullable NSString*)method
                    postData:(nullable NSData*)postData
                     headers:(nullable NSString*)headers;

/**
 * @abstract Create a <MSWebView2ContextMenuItem> object. There is a limit of
 * 1000 active custom context menu items at a given time. Attempting to create
 * more than 1000 active custom context menu items will return nil.
 *
 * @param label The menu item unique identifier.
 * @param icon The menu item icon, we recommend using images that are as small
 * as possible within acceptance in that the bigger icon is, the slower menu
 * shows, and will not show if file size is too big.
 * @param kind A <MSWebView2ContextMenuItemKind> item kind.
 *
 * @return A new MSWebView2ContextMenuItem object whose `isEnabled` property
 * default to `YES` and `isChecked` property will default to `NO`, or nil if
 * the limit of 1000 active custom context menu items has been reached.
 */
- (nullable MSWebView2ContextMenuItem*)
    createContextMenuItem:(nullable NSString*)label
                     icon:(nullable NSImage*)icon
                     kind:(MSWebView2ContextMenuItemKind)kind;

/**
 * @abstract Gets a snapshot collection of <MSWebView2ProcessInfo>.
 *
 * @discussion Provide a list of all processes using same user data folder
 * except for crashpad process.
 */
- (NSArray<MSWebView2ProcessInfo*>*)getProcessInfos;

/**
 * @abstract Gets a snapshot collection of <MSWebView2ProcessInfo> corresponding
 * to all currently running processes associated with this
 * <MSWebView2Environment> except for crashpad process.
 *
 * @discussion This provides the same list of <MSWebView2ProcessInfo> as
 * what's provided in `getProcessInfos`. Plus, this provides a list of
 * associated <MSWebView2FrameInfo> which are actively running(showing UI
 * elements) in the renderer process. Check associatedFrameInfos of
 * <MSWebView2ProcessInfo> for acquiring this detail infos.
 *
 * @exception NSInvalidArgumentException Thrown if `completionHandler` is nil.
 * @exception NSGenericException Thrown if there is no activated WebView.
 */
- (void)getProcessInfoDetailsWithCompletionHandler:
    (void (^)(NSArray<MSWebView2ProcessInfo*>*))completionHandler;

/**
 * @abstract The browser version string of current MSWebView2Environment object.
 */
@property(readonly, nonatomic, copy) NSString* browserVersionString;

/**
 * @abstract The user data path that all MSWebView2WebViews created from this
 * environment are using.
 *
 * @discussion If you call `CreateMSWebView2Environment` API with parameter
 * `userDataPath` is nil, this value will be the default user data path. If
 * parameter `userDataPath` is a valid absolute path, this value is equal to it.
 * Otherwise, this value will be a calculated one.
 */
@property(readonly, nonatomic, copy) NSString* userDataPath;

/**
 * The environment's generic delegate.
 */
@property(nullable, nonatomic, weak) id<MSWebView2EnvironmentDelegate> delegate;

@end

NS_ASSUME_NONNULL_END
