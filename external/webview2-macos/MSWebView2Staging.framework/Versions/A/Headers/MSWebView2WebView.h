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

#import "MSWebView2DevToolsProtocolEventInfo.h"
#import "MSWebView2DownloadDelegate.h"
#import "MSWebView2NavigationDelegate.h"
#import "MSWebView2NewWindowDelegate.h"
#import "MSWebView2PermissionDelegate.h"
#import "MSWebView2Profile.h"
#import "MSWebView2Settings.h"
#import "MSWebView2TextureStream.h"
#import "MSWebView2WebMessageDelegate.h"
#import "MSWebView2WebResourceDelegate.h"
#import "MSWebView2WebViewDelegate.h"

NS_ASSUME_NONNULL_BEGIN

/**
 * Indicates the desired memory consumption level of a WebView.
 */
typedef NS_ENUM(NSInteger, MSWebView2MemoryUsageTargetLevel) {
  /**
   * Low memory usage target level. Used for inactivate WebView for reduced
   * memory consumption.
   */
  MSWebView2MemoryUsageTargetLevelLow,

  /**
   * Normal memory usage target level.
   */
  MSWebView2MemoryUsageTargetLevelNormal,

};

@class MSWebView2Environment;
@class MSWebView2ExecuteScriptResult;

/**
 * Kind of cross origin resource access allowed for host resources during
 * download.  Note that other normal access checks like same origin DOM access
 * check and [Content Security
 * Policy](https://developer.mozilla.org/docs/Web/HTTP/CSP) still apply. The
 * following table illustrates the host resource cross origin access according
 * to access context and `MSWebView2HostResourceAccessKind`.
 *
 * | Cross Origin Access Context | DENY | ALLOW | DENY_CORS |
 * | --- | --- | --- | --- |
 * | From DOM like src of img, script or iframe element| Deny | Allow | Allow |
 * | From Script like Fetch or XMLHttpRequest| Deny | Allow | Deny |
 */
typedef NS_ENUM(NSInteger, MSWebView2HostResourceAccessKind) {
  /**
   * All cross origin resource access is denied, including normal sub resource
   * access as src of a script or image element.
   */
  MSWebView2HostResourceAccessKindDeny = 0,
  /**
   * All cross origin resource access is allowed, including accesses that are
   * subject to Cross-Origin Resource Sharing(CORS) check. The behavior is
   * similar to a web site sends back http header Access-Control-Allow-Origin:
   * *.
   */
  MSWebView2HostResourceAccessKindAllow,
  /**
   * Cross origin resource access is allowed for normal sub resource access like
   * as src of a script or image element, while any access that subjects to CORS
   * check will be denied. See [Cross-Origin Resource
   * Sharing](https://developer.mozilla.org/docs/Web/HTTP/CORS) for more
   * information.
   */
  MSWebView2HostResourceAccessKindDenyCors
};

/**
 * Indicates the default corner alignment of the download dialog in a WebView.
 */
typedef NS_ENUM(NSUInteger, MSWebView2DefaultDownloadDialogCornerAlignment) {
  /**
   * The download dialog is aligned to the top-left corner.
   */
  MSWebView2DefaultDownloadDialogCornerAlignmentTopLeft = 0,
  /**
   * The download dialog is aligned to the top-right corner.
   */
  MSWebView2DefaultDownloadDialogCornerAlignmentTopRight,
  /**
   * The download dialog is aligned to the bottom-left corner.
   */
  MSWebView2DefaultDownloadDialogCornerAlignmentBottomLeft,
  /**
   * The download dialog is aligned to the bottom-right corner.
   */
  MSWebView2DefaultDownloadDialogCornerAlignmentBottomRight
};

/**
 * WebView2 enables you to host web content using the latest Microsoft Edge
 * browser and web technology.
 */
@interface MSWebView2WebView : NSObject

- (instancetype)init NS_UNAVAILABLE;

/**
 * @abstract The web view's navigation delegate.
 */
@property(nullable, nonatomic, weak) id<MSWebView2NavigationDelegate>
    navigationDelegate;

/**
 * @abstract The web view's permission delegate.
 */
@property(nullable, nonatomic, weak) id<MSWebView2PermissionDelegate>
    permissionDelegate;

/**
 * @abstract The web view's new window delegate.
 */
@property(nullable, nonatomic, weak) id<MSWebView2NewWindowDelegate>
    windowDelegate;

/**
 * @abstract The web view's web resource delegate.
 */
@property(nullable, nonatomic, weak) id<MSWebView2WebResourceDelegate>
    webResourceDelegate;

/**
 * @abstract The web view's download delegate.
 */
@property(nullable, nonatomic, weak) id<MSWebView2DownloadDelegate>
    downloadDelegate;

/**
 * @abstract Whether the web view can navigate to a previous page in the
 * navigation history.
 *
 */
@property(nonatomic, readonly) BOOL canGoBack;

/**
 * @abstract Whether the web view can navigate to a next page in the navigation
 * history.
 */
@property(nonatomic, readonly) BOOL canGoForward;

/**
 * @abstract The URI of the current top level document.
 */
@property(nonatomic, readonly) NSString* source;

/**
 * @abstract The process ID of the browser process that hosts the WebView.
 */
@property(nonatomic, readonly) int browserProcessID;

/**
 * @abstract Whether the web view contains a fullscreen HTML element.
 */
@property(nonatomic, readonly) BOOL containsFullScreenElement;

/**
 * @abstract The custom data partition ID for partition cookie and local
 * storage.
 *
 * @discussion This API requires enabling 2 experimental browser features to
 * work properly. These features will be enabled by default in the future.
 * Before these features are enabled by default, please enable them by ensuring
 * `--enable-features=ThirdPartyStoragePartitioning,PartitionedCookies` is set
 * in `additionalBrowserArguments` in <MSWebView2EnvironmentOptions> used to
 * create <MSWebView2Environment>. If these features are not enabled, all data
 * are treated as unpartitioned and stored in the global default location for
 * the profile. When it is set, the page in the WebView will act as if the page
 * were hosted in a top level site uniquely associated with the
 * `customDataPartitionID` and have a separate storage partition as described
 * in [Storage
 * Partitioning](https://developer.chrome.com/docs/privacy-sandbox/storage-partitioning/)
 * and separate cookie partition as described in
 * [Cookies Having Independent Partitioned State
 * (CHIPS)](https://developer.chrome.com/docs/privacy-sandbox/chips/) with all
 * cookies partitioned.
 *
 * If `customDataPartitionID` is nil or empty string, the page
 * inside the WebView will work normally with data treated as unpartitioned. The
 * `customDataPartitionID` parameter is case sensitive. The default is an empty
 * string. There is no restriction on the length or what characters can be used
 * in partition ID. The change of the custom data partition ID will be applied
 * to new page or iframe navigations and not impact existing pages and iframes.
 * To avoid accidentally using the new partition ID for new page or iframe
 * navigations started by the old page, it is recommended to create a new
 * WebView for new partition instead of changing partition. If you really have
 * to change partition, it is recommended to navigate to a blank page before
 * setting the new partition ID and navigating to a page with the new partition.
 *
 * As setting custom data partition ID does not change DOM security
 * model, developers should be very careful for WebViews with opener and
 * opened window relationship, especially when the pages in the WebViews
 * have same origin, like when the opened window is the same website or
 * about:blank. The pages in these WebViews can access each other’s DOM and
 * therefore can potentially access DOM storage and cookies in different
 * partition for the same website. It is recommended to set the same custom
 * data partition ID for these WebViews, unless there is an absolute need
 * to set different partition ids and only trusted code is hosted in them.
 */
@property(nonatomic, copy) NSString* customDataPartitionID;

/**
 * @abstract The web view's settings object that contains various modifiable
 * setting properties for the running web view.
 */
@property(nonatomic, readonly, strong) MSWebView2Settings* settings;

/**
 * @abstract The profile associated with this WebView2 instance.
 */
@property(nonatomic, readonly, strong) MSWebView2Profile* profile;

/**
 * @abstract Indicates the desired memory consumption level of a WebView.
 *
 * @discussion An app may set `memoryUsageTargetLevel` to indicate desired
 * memory consumption level of WebView. Scripts will not be impacted and
 * continue to run.
 *
 * It is a best effort operation to change memory usage level, and the API will
 * return before the operation completes.
 *
 * The app should set the level back to Normal when the app becomes active
 * again. Setting `memoryUsageTargetLevel` back to
 * `MSWebView2MemoryUsageTargetLevelNormal` will not happen automatically.
 *
 * For example:
 * @textblock
 * -(void)onBecomingInactive() {
 *     webView.memoryUsageTargetLevel = MSWebView2MemoryUsageTargetLevelLow;
 * }
 * -(void)OnBecomingActive() {
 *     webView.memoryUsageTargetLevel = MSWebView2MemoryUsageTargetLevelNormal;
 * }
 * @/textblock
 */
@property(nonatomic) MSWebView2MemoryUsageTargetLevel memoryUsageTargetLevel;

/**
 * @abstract Navigate to a webpage.
 *
 * @param URI The specified URI for navigation.
 *
 * @discussion This method starts a navigation, some navigation-related events
 * will be triggered next. See <MSWebView2NavigationDelegate>.
 *
 * @textblock
 * NSString* URI = @"https://www.bing.com";
 * [self.webview navigate:URI];
 * @/textblock
 *
 * @exception NSInvalidArgumentException Thrown if `URI` is invalid, such as
 * missing the scheme.
 */
- (void)navigate:(NSString*)URI;

/**
 * @abstract Reload the current page.
 */
- (void)reload;

/**
 * @abstract Stop all navigations and pending resource fetches.
 */
- (void)stop;

/**
 * @abstract Navigates the WebView to the previous page in the navigation
 * history.
 */
- (void)goBack;

/**
 * @abstract Navigates the WebView to the next page in the navigation history.
 */
- (void)goForward;

/**
 * @abstract Indicate whether telemetry is enabled for the web view.
 */
@property(nonatomic) BOOL enableTelemetry;

/**
 * @abstract Add script to execute on document created.
 *
 * @param javaScript The script to be added.
 * @param completionHandler A block to invoke when the script is added.
 *
 * @discussion Add the provided `JavaScript` to a list of scripts that should be
 * run after the global object has been created, but before the HTML document
 * has been parsed and before any other script included by the HTML document is
 * run. This method injects a script that runs on all top-level document
 * and child frame page navigations. This method runs asynchronously, and
 * you must wait for the `completionHandler` to finish before the injected
 * script is ready to run. When this method completes, the `completionHandler`
 * block is run with the `scriptID` of the injected script. `scriptID` is a
 * string. To remove the injected script, use
 * <removeScriptToExecuteOnDocumentCreated:>.
 *
 * @textblock
 * - (void)sendScriptToDocument {
 *   [webview addScriptToExecuteOnDocumentCreated:@"alert(\"script executed\");"
 *                              completionHandler:^(NSString *scriptID,
 *                                                  NSError *error) {
 *                                if (error == nil) {
 *                                  NSLog(@"script will be executed when the "
 *                                        @"document is loaded.");
 *                                } else {
 *                                  NSLog(@"Failed to add script:%@", error);
 *                                }
 *                              }];
 * }
 * @/textblock
 */
- (void)addScriptToExecuteOnDocumentCreated:(NSString*)javaScript
                          completionHandler:
                              (void (^_Nullable)(NSString* _Nullable scriptID,
                                                 NSError* _Nullable error))
                                  completionHandler;

/**
 * @abstract Remove script to execute on document created.
 *
 * @param scriptID The ID of the script to be removed.
 *
 * @discussion Remove the corresponding `JavaScript` added using
 * <addScriptToExecuteOnDocumentCreated:completionHandler:> with the specified
 * `scriptID`.
 */
- (void)removeScriptToExecuteOnDocumentCreated:(NSString*)scriptID;

/**
 * @abstract The web view's generic delegate.
 */
@property(nullable, nonatomic, weak) id<MSWebView2WebViewDelegate> delegate;

/**
 * @abstract Delegate for the WebMessage event.
 */
@property(nullable, nonatomic, weak) id<MSWebView2WebMessageDelegate>
    webMessageDelegate;

/**
 * @abstract Posts a message that is a simple string rather than a JSON string
 * representation of a JavaScript object.
 *
 * @param message The message data to be sent. When `message` is empty or `nil`,
 * JavaScript will receive an empty message.
 *
 * @discussion This function behaves in exactly the same
 * manner as <postWebMessageAsJSON:>, but the `data` property of the event
 * args of the `window.chrome.webview` message is a string with the same
 * value as `webMessageAsString`. Use this instead of
 * <postWebMessageAsJSON:> if you want to communicate using simple strings
 * rather than JSON objects.
 */
- (void)postWebMessageAsString:(nullable NSString*)message;

/**
 * @abstract Posts the specified webMessage to the main frame.
 *
 * @param message The message data to be sent
 *
 * @discussion The main frame receives the message by subscribing to the
 * `message` event of the `window.chrome.webview` of the main frame document.
 * @textblock
 * window.chrome.webview.addEventListener('message', function(e) {
 *   console.log('Received message from the host page: ' + e.data);
 * });
 * @/textblock
 *
 * The <MSWebView2Settings> property `webMessageEnabled` must be `YES` or
 * the message will not be sent.
 *
 * When processing the event at js side, the `data` property of the event
 * args is the `webMessage` string parameter parsed as a JSON string into a
 * JavaScript object, and The `source` property of the event args is a
 * reference to the `window.chrome.webview` object.
 * The message is delivered asynchronously. If a navigation occurs before
 * the message is posted to the page, the message is discarded.
 *
 * @exception NSInvalidArgumentException Thrown if `message` is not a valid
 * JSON.
 */
- (void)postWebMessageAsJSON:(NSString*)message;

/**
 * @abstract Opens DevTools window for current document in the WebView.
 *
 * @discussion Does nothing if the DevTools window is already open.
 */
- (void)openDevToolsWindow;

/**
 * @abstract Opens Task Manager window which shows memory and CPU usage of all
 * browser processes.
 *
 * @discussion If the Browser Task Manager is already open, this will bring it
 * into the foreground
 */
- (void)openTaskManagerWindow;

/**
 * @abstract Call specific devtools protocol method and get result as JSON
 * string.
 *
 * @param methodName Full name of the method in the `{domain}.{method}` format.
 * @param parametersAsJSON A JSON formatted string containing the parameters for
 * the corresponding method.
 * @param completionHandler When `methodName` method asynchronously completes,
 * this handler will receive the return object of the method as a JSON string.
 *
 * @discussion Run an asynchronous `DevToolsProtocol` method.
 * For more information about available methods, navigate to
 * [debugger-protocol-viewer](https://github.com/chromedevtools/debugger-protocol-viewer)
 * [devtools-protocol](https://github.com/ChromeDevTools/devtools-protocol/)
 *
 * @textblock
 * - (void)callCdpMethodInvoked:(SheetViewController*)sheetViewController {
 *   NSString* methodParamString = [sheetViewController getInput];
 *   NSArray* inputArray = [methodParamString componentsSeparatedByString:@" "];
 *   NSString* methodName = inputArray[0];
 *   NSString* methodParams = [inputArray count] == 1 ? @"{}" : inputArray[1];
 *   __weak typeof(self) weakSelf = self;
 *   [self.webview
 *       callDevToolsProtocolMethod:methodName
 *                           params:methodParams
 *                completionHandler:^(NSString* methodParamString, NSError* err)
 *                {
 *                  if (weakSelf == nil) {
 *                    return;
 *                  }
 *                  __strong typeof(self) strongSelf = weakSelf;
 *                  NSString* result = [@"CallProtocolMethodResult - "
 *                      stringByAppendingFormat:@"[%@]", methodParamString];
 *                  if (err != nil) {
 *                    result = [result
 *                        stringByAppendingFormat:@"\r error code - [%@]", err];
 *                  }
 *                  [strongSelf showAlert:result];
 *                }];
 * }
 * @/textblock
 *
 * @exception NSInvalidArgumentException Thrown if `parametersAsJSON` cannot be
 * formatted as json.
 */
- (void)callDevToolsProtocolMethod:(NSString*)methodName
                            params:(NSString*)parametersAsJSON
                 completionHandler:
                     (void (^_Nullable)(NSString* _Nullable,
                                        NSError* _Nullable))completionHandler;

/**
 * @abstract Runs an asynchronous `DevToolsProtocol` method for a specific
 * session of an attached target.
 *
 * @param sessionID SessionId for an attached target. `nil` or empty string is
 * treated as the session for the default target for the top page.
 * @param methodName Full name of the method in the `{domain}.{method}` format
 * @param parametersAsJSON A JSON formatted string containing the parameters for
 * the corresponding method.
 * @param completionHandler When `methodName` method asynchronously completes,
 * this handler will receive the return object of the method as a JSON string.
 *
 * @discussion There could be multiple `DevToolsProtocol`
 * targets in a WebView. Besides the top level page, iframes from different
 * origin and web workers are also separate targets. Attaching to these targets
 * allows interaction with them. When the `DevToolsProtocol` is attached to a
 * target, the connection is identified by a sessionID. To use this API, you
 * must set the `flatten` parameter to true when calling `Target.attachToTarget`
 * or `Target.setAutoAttach` `DevToolsProtocol` method. Using
 * `Target.setAutoAttach` is recommended as that would allow you to attach to
 * dedicated worker targets, which are not discoverable via other APIs like
 * `Target.getTargets`.
 * For more information about available methods, navigate to
 * [debugger-protocol-viewer](https://github.com/chromedevtools/debugger-protocol-viewer)
 * [devtools-protocol](https://github.com/ChromeDevTools/devtools-protocol/)
 *
 * @textblock
 * - (void)callCdpMethodForSessionInvoked:
 *     (SheetViewController*)sheetViewController {
 *   NSString* methodParamString = [sheetViewController getInput];
 *   NSArray* inputArray = [methodParamString componentsSeparatedByString:@" "];
 *   NSString* sessionID = inputArray[0];
 *   NSString* methodName = inputArray[1];
 *   NSString* methodParams = [inputArray count] == 2 ? @"{}" : inputArray[2];
 *   __weak typeof(self) weakSelf = self;
 *   [self.webview
 *       callDevToolsProtocolMethodForSession:sessionID
 *                                 methodName:methodName
 *                                     params:methodParams
 *                          completionHandler:^(NSString* resultAsString,
 *                                              NSError* err) {
 *                            __strong typeof(self) strongSelf = weakSelf;
 *                            NSString* result =
 *                                [@"CallProtocolMethodForSessionResult - "
 *                                    stringByAppendingFormat:@"[%@]",
 *                                                            resultAsString];
 *                            if (err != nil) {
 *                              result = [result stringByAppendingFormat:
 *                                          @"\r error code - [%@]", err];
 *                            }
 *                            [strongSelf showAlert:result];
 *                          }];
 * }
 * @/textblock
 *
 * @exception NSInvalidArgumentException Thrown if `parametersAsJSON` cannot be
 * formatted as json.
 */
- (void)callDevToolsProtocolMethodForSession:(nullable NSString*)sessionID
                                  methodName:(NSString*)methodName
                                      params:(NSString*)parametersAsJSON
                           completionHandler:
                               (void (^_Nullable)(NSString* _Nullable,
                                                  NSError* _Nullable))
                                   completionHandler;

/**
 * @abstract Set specific DevToolsProtocol event's receiver.
 *
 * @param eventName The DevToolsProtocol event which wants to subscribe.
 * @param receiver  A block to be invoked when subscribed
 * DevToolsProtocol event fires. It receives a
 * <MSWebView2DevToolsProtocolEventInfo> object as parameters.
 *
 * @discussion If called multi-times, it will only store the last receiver.
 *
 * @textblock
 * - (void)subscribeToCdpEventInvoked:(SheetViewController*)sheetViewController
 * { NSString* eventName = [sheetViewController getInput];
 *   __weak typeof(self) weakSelf = self;
 *   [self.webview
 *       setDevToolsProtocolEvent:eventName
 *                   withReceiver:^(MSWebView2DevToolsProtocolEventInfo* info) {
 *                     if (weakSelf == nil) {
 *                       return;
 *                     }
 *                     __strong typeof(self) strongSelf = weakSelf;
 *                     NSString* result = [@"SubscribeCdpEventResult - "
 *                         stringByAppendingFormat:@"[%@] \r sessionID - [%@]",
 *                                                 info.parameterObjectAsJson,
 *                                                 info.sessionID];
 *                     [strongSelf showAlert:result];
 *                   }];
 * }
 * /@textblock
 */
- (void)
    setDevToolsProtocolEvent:(NSString*)eventName
                withReceiver:
                    (void (^)(
                        MSWebView2DevToolsProtocolEventInfo* _Nullable info))
                        receiver;

/**
 * @abstract Clear specific DevToolsProtocol event's receiver.
 *
 * @param eventName The DevToolsProtocol event which wants to remove.
 *
 * @discussion Will remove relative DevToolsProtocol event receiver.
 *
 * @textblock
 * - (void)clearCdpEventInvoked:(SheetViewController*)sheetViewController {
 *   NSString* eventName = [sheetViewController getInput];
 *   [self.webview clearDevToolsProtocolEvent:eventName];
 * }
 * /@textblock
 */
- (void)clearDevToolsProtocolEvent:(NSString*)eventName;

/**
 * @abstract A web resource request with a resource context that matches this
 * filter's resource context and a URI that matches this filter's URI wildcard
 * string for corresponding request sources will be raised via the
 * `WebResourceRequested` event.
 *
 * @param URIPattern The `URIPattern` parameter is a wildcard string used to
 * match the URI of the web resource request.
 * @param context The `context` parameter is a <MSWebView2WebResourceContext>
 * enum value used to filter specific resource context to intercept.
 * @param kinds The `kinds` parameter is a mask of one
 * or more <MSWebView2WebResourceRequestSourceKinds>. OR operation(s) can be
 * applied to multiple `MSWebView2WebResourceRequestSourceKinds` to create a
 * mask representing those data types.
 *
 * @discussion To receive all raised events, filters have to be added before
 * main page navigation.
 *
 * The empty `URIPattern` or `URIPattern` is `nil` does not match any URI. The
 * wildcards ('*' -> zero or more, '?' -> exactly one) are allowed to
 * URIPattern. Escape character is backslash.
 *
 * For example, if you want to call intercept the image requests of any URI, you
 * can call like this
 *
 * @textblock
 * [webview addWebResourceRequestedFilter:@"*"
 *     webResourceContext:MSWebView2WebResourceContextImage
 *     requestSourceKinds:MSWebView2WebResourceRequestSourceKindsAll];
 * @/textblock
 *
 * In addition, because shared workers run separately from
 * any one HTML document, their `WebResourceRequested` will be raised for all
 * MSWebView2WebViews that share the same user data dir and have appropriate
 * filters added in the corresponding MSWebView2Environment. You should only add
 * a WebResourceRequested filter for
 * `MSWebView2WebResourceRequestSourceKindsSharedWorker` on one
 * MSWebView2WebView to avoid handling the same `WebResourceRequested` event
 * multiple times.
 *
 * @exception NSInvalidArgumentException Thrown if the `kinds` is
 * `MSWebView2WebResourceRequestSourceKindsNone`
 */
- (void)addWebResourceRequestedFilter:(nullable NSString*)URIPattern
                   webResourceContext:(MSWebView2WebResourceContext)context
                   requestSourceKinds:
                       (MSWebView2WebResourceRequestSourceKinds)kinds;

/**
 * @abstract Remove a matching filter that was previously added for the
 * `WebResourceRequested` event.
 *
 * @param URIPattern Same as `URIPattern` parameter in
 * <addWebResourceRequestedFilter:webResourceContext:requestSourceKinds:>.
 * @param context Same as `context` parameter in
 * <addWebResourceRequestedFilter:webResourceContext:requestSourceKinds:>.
 * @param kinds Same as `kinds` parameter in
 * <addWebResourceRequestedFilter:webResourceContext:requestSourceKinds:>.
 *
 * @discussion If the same filter was added multiple times, then it must be
 * removed as many times as it was added for the removal to be effective.
 *
 * @exception NSInvalidArgumentException Thrown if the `kinds` is
 * `MSWebView2WebResourceRequestSourceKindsNone`
 */
- (void)removeWebResourceRequestedFilter:(nullable NSString*)URIPattern
                      webResourceContext:(MSWebView2WebResourceContext)context
                      requestSourceKinds:
                          (MSWebView2WebResourceRequestSourceKinds)kinds;

/**
 * @abstract Host object trigger JavaScript method execution callback.
 *
 * @param arguments method arguments
 *
 * @return local execute result or nil
 * @discussion If host object wants to trigger javascript to execute, the block
 * must be MSWebView2JavaScriptExecuteBlock type.
 */
typedef _Nullable id (^MSWebView2JavaScriptExecuteBlock)(
    NSArray* _Nullable arguments);

/**
 * @abstract Add the provided host object to script running in the WebView with
 * the specified name.
 *
 * @param object The host object.
 * @param name The name representing the host object.
 *
 * @discussion Host objects are exposed as host object proxies using
 * window.chrome.webview.hostObjects.{name}. Host object proxies are promises
 * and resolves to an object representing the host object. The promise is
 * rejected if the app has not added an object with the name. When JavaScript
 * code access a property or method of the object, a promise is return, which
 * resolves to the value returned from the host for the property or method, or
 * rejected in case of error, for example, no property or method on the object
 * or parameters are not valid.
 *
 * Additionally, all host objects are exposed as
 * window.chrome.webview.hostObjects.sync.{name}. Here the host objects are
 * exposed as synchronous host object proxies. These are not promises and
 * function runtimes or property access synchronously block running script
 * waiting to communicate cross process for the host code to run. Accordingly
 * the result may have reliability issues and it is recommended that you use the
 * promise-based asynchronous window.chrome.webview.hostObjects.{name} API.
 *
 * Synchronous host object proxies and asynchronous host object
 * proxies may both use a proxy to the same host object. Remote changes made by
 * one proxy propagates to any other proxy of that same host object whether the
 * other proxies and synchronous or asynchronous.
 *
 * @exception NSInvalidArgumentException Thrown if the provided `object` is not
 * an instance of NSObject.
 */
- (void)addHostObjectToScript:(NSObject*)object withName:(NSString*)name;

/**
 * @abstract Remove the host object specified by the name so that it is no
 * longer accessible from JavaScript code in the WebView.
 *
 * @param name The name representing the host object
 *
 * @discussion While new access attempts are denied, if the object is already
 * obtained by JavaScript code in the WebView, the JavaScript code continues to
 * have access to that object.
 */
- (void)removeHostObjectFromScript:(NSString*)name;

/**
 * @abstract Export class's properties and methods to JavaScript.
 *
 * @param cls The object's class
 * @param properties The class exported property list.
 * @param methods The class exported method list.
 *
 * @discussion Define the access control of a Objective-C class. If javascript
 * execute a non export property/method, it will failed.
 *
 * All javascript used property/method must be exported before use.
 */
- (void)exportObjectClass:(Class)cls
               properties:(nullable NSArray<NSString*>*)properties
                  methods:(nullable NSDictionary<NSString*, NSString*>*)methods;

/**
 * @abstract Sets a mapping between a virtual host name and a folder path to
 * make available to web sites via that host name.
 *
 * @param hostName The name that can be used to access host resources after
 * setting.
 * @param folderPath The local target that needs to be accessed, the length
 * cannot exceed the path length limit of system. Both absolute and relative
 * paths are supported for folderPath. Relative paths are interpreted s relative
 * to the folder where the main bundle of the app is in.
 * @param kind Specifies the level of access to resources under the
 * virtual host from other sites.
 *
 * @discussion After setting the mapping, documents loaded in the WebView can
 * use HTTP or HTTPS URLs at the specified host name specified by hostName to
 * access files in the local folder specified by folderPath.
 *
 * This mapping applies to both top-level document and iframe navigations as
 * well as subresource references from a document. This also applies to web
 * workers including dedicated/shared worker and service worker, for loading
 * either worker scripts or subresources (importScripts(), fetch(), XHR, etc.)
 * issued from within a worker. For virtual host mapping to work with service
 * worker, please keep the virtual host name mappings consistent among all
 * WebViews sharing the same browser instance. As service worker works
 * independently of WebViews, we merge mappings from all WebViews when resolving
 * virtual host name, inconsistent mappings between WebViews would lead
 * unexpected behavior.
 *
 * Due to a current implementation limitation, media files accessed using
 * virtual host name can be very slow to load. As the resource loaders for the
 * current page might have already been created and running, changes to the
 * mapping might not be applied to the current page and a reload of the page is
 * needed to apply the new mapping.
 *
 * As the resource loaders for the current page might have already been created
 * and running, changes to the mapping might not be applied to the current page
 * and a reload of the page is needed to apply the new mapping.
 *
 * DOM elements that want to reference local files will have their host
 * reference virtual host in the source.If there are multiple folders being
 * used, define one unique virtual host per folder.
 *
 * You should typically choose virtual host names that are never used by real
 * sites.If you own a domain such as example.com, another option is to use a
 * subdomain reserved for the app (like my-app.example.com).
 *
 * [RFC 6761](https://tools.ietf.org/html/rfc6761) has reserved several
 * special-use domain names that are guaranteed to not be used by real sites
 * (for example, .example, .test, and .invalid.)
 *
 * Note that using `.local` as the top-level domain name will work but can cause
 * a delay during navigations. You should avoid using `.local` if you can.
 *
 * Apps should use distinct domain names when mapping folder from different
 * sources that should be isolated from each other. For instance, the app might
 * use app-file.example for files that ship as part of the app, and
 * book1.example might be used for files containing books from a less trusted
 * source that were previously downloaded and saved to the disk by the app.
 *
 * The host name used in the APIs is canonicalized using Chromium's host name
 * parsing logic before being used internally. For more information see
 * [HTML5 2.6 URLs](https://dev.w3.org/html5/spec-LC/urls.html).
 *
 * All host names that are canonicalized to the same string are considered
 * identical.For example, `EXAMPLE.COM` and `example.com` are treated as the
 * same host name. An international host name and its Punycode-encoded host name
 * are considered the same host name. There is no DNS resolution for host name
 * and the trailing '.' is not normalized as part of canonicalization.
 *
 * Therefore `example.com` and `example.com.` are treated as different host
 * names. Similarly, `virtual-host-name` and `virtual-host-name.example.com` are
 * treated as different host names even if the machine has a DNS suffix of
 * `example.com`.
 *
 * Specify the minimal cross-origin access necessary to run the app. If there is
 * not a need to access local resources from other origins, use
 * MSWebView2HostResourceAccessKindDeny.
 *
 * @textblock
 * void accessTheAssetsFolder(MSWebView2WebView* webview) {
 *   [webview setVirtualHostName:@"appassets.example"
 *               toFolderMapping:@"appassets"
 *                    accessKind:MSWebView2HostResourceAccessKindDeny];
 *   // Appassets/index.html in the bundlePath directory will be accessed.
 *   [webview navigate:@"appassets.example/index.html"];
 * }
 * @/textblock
 *
 * @exception NSInvalidArgumentException Thrown if `hostName` cannot be
 * normalized.
 * @exception NSGenericException Thrown if `folderPath` does not exist.
 */
- (void)setVirtualHostName:(NSString*)hostName
           toFolderMapping:(NSString*)folderPath
                accessKind:(MSWebView2HostResourceAccessKind)kind;

/**
 *
 * @abstract Clears a host name mapping for local folder that was added by
 * <setVirtualHostName:toFolderMapping:accessKind:>.
 *
 * @param hostName The name of the virtual host to be cleared.
 *
 * @exception NSInvalidArgumentException Thrown if `hostName` cannot be
 * normalized.
 */
- (void)clearVirtualHostNameToFolderMapping:(NSString*)hostName;

/**
 * @abstract The title for the current top-level document.
 *
 * @discussion If the document has no explicit title or is empty, a default that
 * may or may not match the URI of the document is used.
 */
@property(nullable, nonatomic, readonly) NSString* documentTitle;

/**
 * @abstract Run JavaScript code from the `javaScript` parameter in the current
 * top-level document rendered in the WebView.
 *
 * @param javaScript The JavaScript code to be executed
 * @param completionHandler The evaluation result of the provided JavaScript is
 * passed to the completionHandler.
 *
 * @discussion The result value is a JSON encoded string. If the result is
 * undefined, or otherwise is not able to be encoded into JSON, then the result
 * is considered to be null, which is encoded in JSON as the string "null".
 *
 * A function that has no explicit return value returns undefined.
 * If the script that was run throws an unhandled exception, then the result is
 * also "null". This method is applied asynchronously. If the method is run
 * after the `NavigationStarting` event during a navigation, the script runs in
 * the new document when loading it, around the time `ContentLoading` is run.
 * This operation executes the script even if <MSWebView2Settings> property
 * `scriptEnabled` is set to `NO`.
 *
 * For example:
 * @textblock
 * - (void)injectScriptInvoked:(SheetViewController*)sheetViewController {
 *   NSString* input = [sheetViewController getInput];
 *   __weak typeof(self) weakSelf = self;
 *   [self.webview executeScript:input
 *             completionHandler:^(NSString* result, NSError* error) {
 *               if (weakSelf == nil) {
 *                 return;
 *               }
 *               __strong typeof(self) strongSelf = weakSelf;
 *               NSString* text = [@"CallExecuteScriptResult - "
 *                   stringByAppendingFormat:@"[%@] and error code - [%@]",
 *                                           result, error];
 *               [strongSelf showAlert:text];
 *             }];
 * }
 * @/textblock
 */
- (void)executeScript:(NSString*)javaScript
    completionHandler:
        (void (^_Nullable)(NSString* _Nullable result,
                           NSError* _Nullable error))completionHandler;

/**
 * @abstract Run JavaScript code from the javaScript parameter in the current
 * top-level document rendered in the WebView.
 *
 * @param javaScript The JavaScript code to be executed
 * @param completionHandler The evaluation result of the provided JavaScript is
 * passed to the completionHandler, the NSError parameter currently always
 * returns nil.
 *
 * @discussion The result of the execution is returned asynchronously in the
 * MSWebView2ExecuteScriptResult object which has methods and properties to
 * obtain the successful result of script execution as well as any unhandled
 * JavaScript exceptions.
 *
 * If this method is run after the NavigationStarting event during a navigation,
 * the script runs in the new document when loading it, around the time
 * ContentLoading is run. This operation executes the script even if
 * MSWebView2Settings::isScriptEnabled is set to NO.
 *
 * For example:
 * @textblock
 * - (void)injectScriptWithResultInvoked:
 *     (SheetViewController*)sheetViewController {
 *   NSString* input = [sheetViewController getInput];
 *   __weak typeof(self) weakSelf = self;
 *   [self.webview
 *       executeScriptWithResult:input
 *             completionHandler:^(MSWebView2ExecuteScriptResult* result,
 *                                 NSError* error) {
 *               if (weakSelf == nil) {
 *                 return;
 *               }
 *               __strong typeof(self) strongSelf = weakSelf;
 *
 *               NSString* resultString = [NSString
 *                   stringWithFormat:@"succeeded:%d\n"
 *                                    @"resultAsJson:%@\n"
 *                                    @"tryGetResultAsString:%@\n"
 *                                    @"exception:",
 *                                    result.succeeded, result.resultAsJson,
 *                                    result.tryGetResultAsString];
 *               if (result.exception == nil) {
 *                 resultString = [resultString stringByAppendingString:@"nil"];
 *               } else {
 *                 resultString = [resultString
 *                     stringByAppendingFormat:@"\n"
 *                                             @"  lineNumber:%lu\n"
 *                                             @"  columnNumber:%lu\n"
 *                                             @"  name:%@\n"
 *                                             @"  message:%@\n"
 *                                             @"  toJson:%@",
 *                                             result.exception.lineNumber,
 *                                             result.exception.columnNumber,
 *                                             result.exception.name,
 *                                             result.exception.message,
 *                                             result.exception.toJson];
 *               }
 *               [strongSelf showAlert:resultString];
 *             }];
 * }
 * @/textblock
 *
 * @exception NSInvalidArgumentException Thrown if `javaScript` is empty.
 */
- (void)executeScriptWithResult:(NSString*)javaScript
              completionHandler:
                  (void (^_Nullable)(MSWebView2ExecuteScriptResult*,
                                     NSError* _Nullable))completionHandler;

/**
 * @abstract Open the default download dialog.
 *
 * @discussion If the dialog is opened before there are recent downloads,
 * the dialog shows all past downloads for the current profile. Otherwise,
 * the dialog shows only the recent downloads with a "See more" button for
 * past downloads. Calling this method raises the
 * `IsDefaultDownloadDialogOpenChanged` event if the dialog was closed. No
 * effect if the dialog is already open.
 */
- (void)openDefaultDownloadDialog;

/**
 * @abstract Close the default download dialog.
 *
 * @discussion Calling this method raises the
 * `IsDefaultDownloadDialogOpenChanged` event if the dialog was open. No effect
 * if the dialog is already closed.
 */
- (void)closeDefaultDownloadDialog;

/**
 * @abstract MSWebView2Environment instance creates this web view.
 */
@property(readonly, nonatomic, strong) MSWebView2Environment* environment;

/**
 * @abstract The unique identifier of the current frame.
 *
 * @discussion Note that frame Id is not valid if <MSWebView2WebView> has not
 * started a navigation, and it returns an invalid frame Id 0.
 */
@property(nonatomic, readonly) NSInteger mainFrameID;

/**
 * @abstract The default download dialog corner alignment.
 *
 * @discussion Set the default download dialog corner alignment. The dialog can
 * be aligned to any of the WebView corners (see
 * `MSWebView2DefaultDownloadDialogCornerAlignment`). When the WebView or dialog
 * changes size, the dialog keeps its position relative to the corner. The
 * dialog may become partially or completely outside of the WebView bounds if
 * the WebView is small enough. Set the margin relative to the corner with the
 * `defaultDownloadDialogMargin` property. The corner alignment and margin
 * should be set during initialization to ensure that they are correctly applied
 * when the layout is first computed, otherwise they will not take effect until
 * the next time the WebView position or size is updated.
 */
@property(nonatomic) MSWebView2DefaultDownloadDialogCornerAlignment
    defaultDownloadDialogCornerAlignment;

/**
 * @abstract The default download dialog margin.
 *
 * @discussion Set the default download dialog margin relative to the WebView
 * corner specified by `MSWebView2DefaultDownloadDialogCornerAlignment`. The
 * margin is a point that describes the vertical and horizontal distances
 * between the chosen WebView corner and the default download dialog corner
 * nearest to it. Positive values move the dialog towards the center of the
 * WebView from the chosen WebView corner, and negative values move the dialog
 * away from it. Use (0, 0) to align the dialog to the WebView corner with no
 * margin. The corner alignment and margin should be set during initialization
 * to ensure that they are correctly applied when the layout is first computed,
 * otherwise they will not take effect until the next time the WebView position
 * or size is updated.
 */
@property(nonatomic) NSPoint defaultDownloadDialogMargin;

/**
 * @abstract The default download dialog visibility.
 *
 * @discussion `YES` if the default download dialog is currently open. The value
 * of this property changes only when the default download dialog is explicitly
 * opened or closed. Hiding the WebView implicitly hides the dialog, but does
 * not change the value of this property.
 */
@property(nonatomic, readonly) BOOL isDefaultDownloadDialogOpen;
@end
NS_ASSUME_NONNULL_END
