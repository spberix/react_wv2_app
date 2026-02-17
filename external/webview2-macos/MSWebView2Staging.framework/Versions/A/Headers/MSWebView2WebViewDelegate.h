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

#import "MSWebView2BasicAuthenticationRequestedInfo.h"
#import "MSWebView2BasicAuthenticationResponse.h"
#import "MSWebView2ContextMenuItem.h"
#import "MSWebView2ContextMenuRequestedInfo.h"
#import "MSWebView2ContextMenuTarget.h"
#import "MSWebView2DedicatedWorkerCreatedInfo.h"
#import "MSWebView2Frame.h"
#import "MSWebView2LaunchingExternalUriSchemeInfo.h"
#import "MSWebView2NavigationStartingDecision.h"
#import "MSWebView2NavigationStartingInfo.h"
#import "MSWebView2ProcessFailedInfo.h"
#import "MSWebView2ScriptDialogOpeningInfo.h"

NS_ASSUME_NONNULL_BEGIN

@class MSWebView2WebView;
/**
 * Delegate for the generic WebView events.
 */
@protocol MSWebView2WebViewDelegate <NSObject>

@optional

/**
 * @abstract Called when content inside the WebView requested to close the
 * window, such as after `window.close` is run.
 *
 * @param webView The WebView on which the window close request occurred.
 *
 * @discussion The app should close the WebView and related app window if that
 * makes sense to the app. After the first window.close() call, this event may
 * not fire for any immediate back to back window.close() calls.
 *
 * @textblock
 * - (void)webViewWindowCloseRequested:(MSWebView2WebView*)webView {
 *   NSString* alertMessage = @"windowCloseRequested event occurred.";
 *   NSLog(@"%@", alertMessage);
 *   // Only prompt an alert here, but the host app can also choose to close
 *   // the window completely.
 *   [self showAlert:alertMessage];
 * }
 * @/textblock
 */
- (void)webViewWindowCloseRequested:(MSWebView2WebView*)webView;

/**
 * @abstract Called when MSWebView2WebView's `containsFullScreenElement`
 * property changes.
 *
 * @param webView The WebView that contains the full screen element.
 *
 * @discussion An HTML element inside the WebView may enter fullscreen to
 * the size of the WebView or leave fullscreen. This event is useful when, for
 * example, a video element requests to go fullscreen, the host app may resize
 * the WebView in response.
 *
 * @textblock
 * - (void)webViewContainsFullScreenElementChanged:(MSWebView2WebView*)webView {
 *   BOOL containsFullScreenElement = webView.containsFullScreenElement;
 *   NSLog(@"containsFullScreenElement changed to: %@",
 *         containsFullScreenElement ? @"YES" : @"NO");
 *   NSWindow* window = self.view.window;
 *   BOOL isFullScreen = (window.styleMask & NSWindowStyleMaskFullScreen) != 0;
 *   // Simulate what the host app might want to do, i.e., change the window
 *   // full screen state if needed, to align with the webpage element's full
 *   // screen state.
 *   if (isFullScreen != containsFullScreenElement)
 *     [window toggleFullScreen:nil];
 * }
 * @/textblock
 */
- (void)webViewContainsFullScreenElementChanged:(MSWebView2WebView*)webView;

/**
 * @abstract Tells the delegate that an iframe is created.
 *
 * @param webView The WebView on which a new iframe is created.
 * @param frame The created frame.
 */
- (void)webView:(MSWebView2WebView*)webView
    frameCreated:(MSWebView2Frame*)frame;

/**
 * @abstract Called when the `documentTitle` property of WebView changes.
 *
 * @param webView The WebView whose document title changes.
 *
 * @discussion The delegate may get the event before or after the
 * `NavigationCompleted` event.
 *
 * @textblock
 * - (void)webViewDocumentTitleChanged:(MSWebView2WebView*)webView {
 *   // When the title of the webpage changes, we modify the
 *   // title of the WebView synchronously.
 *   [self updateWindowTitleWithDocumentTitle:webView.documentTitle];
 * }
 * @/textblock
 */
- (void)webViewDocumentTitleChanged:(MSWebView2WebView*)webView;

/**
 * @abstract Called when a JavaScript dialog is opening.
 *
 * @param webView The WebView on which a JavaScript dialog is opening.
 * @param info The details about this event.
 * @param decisionHandler The decision handler to call to response the opening
 * JavaScript dialog.
 *
 * @discussion This method runs when a JavaScript dialog (`alert`, `confirm`,
 * `prompt`, or `beforeunload`) displays for the WebView. This event only
 * triggers if the <MSWebView2Settings> class member
 * `defaultScriptDialogsEnabled` property is set to `NO`. Override this method
 * suppresses dialogs or replaces default dialogs with custom dialogs.
 *
 * If the `decisionHandler` is not run, the subsequent scripts are blocked
 * until it returns.
 *
 * The host needs to pass the `accept` parameter in decisionHandler `YES` to
 * respond with **OK** to `confirm`, `prompt`, and `beforeunload` dialogs.
 * Pass it `NO` to indicate cancel. From JavaScript, this means that the
 * `confirm` returns `true` if `accept` is YES. And for the alert function,
 * `accept` value is ignored.
 *
 * The `resultText` parameter in decisionHandler is the return value from the
 * JavaScript prompt function if `accept` is YES. This value is ignored for
 * dialog kinds other than prompt.  If `accept` is NO, this value is ignored
 * and null is returned from prompt.
 *
 * @textblock
 * // Replace the default JavaScript dialog with custom dialog.
 * - (void)webView:(MSWebView2WebView*)webView
 *     scriptDialogOpening:(MSWebView2ScriptDialogOpeningInfo*)info
 *       decisionHandler:
 *         (void (^)(BOOL accept, NSString* resultText))decisionHandler {
 *   NSString* instruction = [NSString
 *     stringWithFormat:@"The page at '%@' says: %@", info.URI, info.message];
 *   NSString* title = @"Script Dialog";
 *   [self createSheetWindowWithTitle:title
 *                        instruction:instruction
 *                      inputTooltips:info.defaultText
 *                          sheetType:SheetTypeScriptDialogOpening
 *                       popupHandler:^(NSModalResponse returnCode,
 *                                      SheetViewController* controller) {
 *                         if (returnCode == NSModalResponseOK) {
 *                           NSString* text = [controller getInput];
 *                           // User clicks OK button.
 *                           decisionHandler(YES, text);
 *                         } else {
 *                           decisionHandler(NO, info.defaultText);
 *                         }
 *                       }];
 * }
 * @/textblock
 */
- (void)webView:(MSWebView2WebView*)webView
    scriptDialogOpening:(MSWebView2ScriptDialogOpeningInfo*)info
        decisionHandler:(void (^)(BOOL accept, NSString* _Nullable resultText))
                            decisionHandler;

/**
 * @abstract Called when context menu is requested.
 *
 * @param webView The WebView that requests context menu.
 * @param info The details about this request.
 * @param decisionHandler The decision handler to call to response.
 *
 * @discussion The parameter `handled` of decisionHandler indicates whether the
 * `ContextMenuRequested` event is handled by host. If `handled` is set to
 * `YES`, then WebView will not display a context menu and will instead use the
 * parameter `selectedCommandID` property to indicate which, if any, context
 * menu item command to invoke. If set to `NO` then WebView will display a
 * context menu based on `modifiedItems`, its default value is `NO`.
 *
 * The parameter `selectedCommandID` default value is -1 which means that no
 * selection occurred. If `selectedCommandID` is set to a value other than -1,
 * then WebView will invoke the corresponding context menu item command. Cannot
 * use custom context menu item's commandID here.
 *
 * The parameter `modifiedItems` is an array of context menu items that will be
 * displayed by WebView if `handled` set to `NO`. The app can modify the `items`
 * then pass the modified array `modifiedItems` to WebView.
 * @textblock
 *- (void)webView:(MSWebView2WebView*)webView
 *    contextMenuRequested:(MSWebView2ContextMenuRequestedInfo*)info
 *      decisionHandler:
 *        (void (^)(BOOL handled,
 *                  int32_t selectedCommandID,
 *                  NSArray<MSWebView2ContextMenuItem*>* _Nullable
 *modifiedItems)) decisionHandler {  
 *    // Modify the context menu items.  
 *NSMutableArray<MSWebView2ContextMenuItem*>* modifiedItems =
 *        [NSMutableArray arrayWithArray:info.menuItems];
 *   if (modifiedItems.count > 0) {
 *       // Remove the first item.
 *       [modifiedItems removeObjectAtIndex:0];
 *       // Add a custom item.
 *       MSWebView2ContextMenuItem* customItem = [webview.environment
 *           createContextMenuItem:@"Custom Item"
 *                            icon:nil
 *                            kind:MSWebView2ContextMenuItemKindCommand];
 *       [modifiedItems addObject:customItem];
 *   }
 *   // Display the modified context menu.
 *   decisionHandler(NO, 0, modifiedItems);
 * }
 *@/textblock
 */
- (void)webView:(MSWebView2WebView*)webView
    contextMenuRequested:(MSWebView2ContextMenuRequestedInfo*)info
         decisionHandler:
             (void (^)(
                 BOOL handled,
                 int32_t selectedCommandID,
                 NSArray<MSWebView2ContextMenuItem*>* _Nullable modifiedItems))
                 decisionHandler;

/**
 * @abstract Called when `BasicAuthenticationRequested` event raises.
 *
 * @param webView The WebView that requests authentication.
 * @param info The details about authentication request info.
 * @param decisionHandler The decision handler to call to response the basic
 * authentication request.
 *
 * @discussion `BasicAuthenticationRequested` event is raised when WebView
 * encounters a Basic HTTP Authentication request as described in
 * https://developer.mozilla.org/docs/Web/HTTP/Authentication, a Digest
 * HTTP Authentication request as described in
 * https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers/Authorization#digest,
 * an NTLM authentication or a Proxy Authentication request.
 *
 * The host can provide a response with credentials for the
 * authentication or cancel the request. If the host sets the cancel parameter
 * to NO but does not provide either userName or password properties on the
 * <MSWebView2BasicAuthenticationResponse>, then WebView will show
 * the default authentication challenge dialog prompt to the user.
 *
 * @textblock
 *  - (void)webView:(MSWebView2WebView*)webView
 *     basicAuthenticationRequested:
 *         (MSWebView2BasicAuthenticationRequestedInfo*)info
 *                  decisionHandler:
 *                      (void (^)(BOOL cancel,
 *                                MSWebView2BasicAuthenticationResponse*
 *                                    response))decisionHandler {
 *   if ([info.URI isEqualToString:
 *                 @"https://authenticationtest.com/HTTPAuth/"]) {
 *     MSWebView2BasicAuthenticationResponse* res =
 *         [[MSWebView2BasicAuthenticationResponse alloc] init];
 *     res.userName = @"user";
 *     res.password = @"pass";
 *     decisionHandler(NO, res);
 *   } else {
 *     decisionHandler(NO, nil);
 *   }
 * }
 * @/textblock
 */
- (void)webView:(MSWebView2WebView*)webView
    basicAuthenticationRequested:
        (MSWebView2BasicAuthenticationRequestedInfo*)info
                 decisionHandler:
                     (void (^)(
                         BOOL cancel,
                         MSWebView2BasicAuthenticationResponse* _Nullable response))
                         decisionHandler;

/**
 * @abstract Called when a navigation request is made to a URI scheme that is
 * registered with the OS.
 *
 * @param webView The WebView that launching external uri scheme.
 * @param info The details about external uri scheme info.
 * @param decisionHandler The decision handler to call to response the
 * launching.
 *
 * @discussion The `NavigationStarting` and `NavigationCompleted` events will be
 * raised, regardless of whether the `shouldCancel` is set to `YES` or `NO`. The
 * `NavigationCompleted`event will be raised with the `isSuccess` set to `NO`
 * and `webErrorStatus` set to `MSWebView2WebErrorStatusConnectionAborted`. The
 * `SourceChanged`, `ContentLoading` and `HistoryChanged` events will not be
 * raised regardless of the `shouldCancel`. This event will be raised after the
 * `NavigationStarting` event and before the `NavigationCompleted` event.
 *
 * If the request is initiated by a cross-origin frame without a user gesture,
 * the request will be blocked and the event will not be raised.
 *
 * @textblock
 * - (void)webView:(MSWebView2WebView*)webView
 *     launchingExternalUriScheme:(MSWebView2LaunchingExternalUriSchemeInfo*)info
 *                decisionHandler:(void (^)(BOOL shouldCancel))decisionHandler {
 *   if ([info.URI hasPrefix:@"calc"]) {
 *     decisionHandler(YES);
 *   } else {
 *     decisionHandler(NO);
 *   }
 * }
 * @/textblock
 */
- (void)webView:(MSWebView2WebView*)webView
    launchingExternalUriScheme:(MSWebView2LaunchingExternalUriSchemeInfo*)info
               decisionHandler:(void (^)(BOOL shouldCancel))decisionHandler;

/**
 * @abstract Called when browser process failed.
 *
 * @param webView The WebView that receives process failed info.
 * @param info Interface which includes process failed info.
 *
 * @discussion `ProcessFailed` event fires when a WebView process ends
 * unexpectedly or becomes unresponsive. The app can use this event to collect
 * additional information about the failure and decide whether to reload the
 * WebView.
 *
 * The `ProcessFailed` event is raised for **unexpected** process exits of any
 * kind (browser, render, GPU, and all other types), or for main frame
 * **render process** unresponsiveness.
 * @textblock
 * - (void)webView:(MSWebView2WebView*)webView
 * processFailed:(MSWebView2ProcessFailedInfo*)info
 * {
 *     NSLog(@"Process failed with error code: %d", info.errorCode);
 *     if (info.errorCode ==
 * MSWebView2ProcessFailedKindRenderProcessUnresponsive)
 *     {
 *         // Reload the page
 *         [webView reload];
 *     }
 * }
 * @/textblock
 */
- (void)webView:(MSWebView2WebView*)webView
    processFailed:(MSWebView2ProcessFailedInfo*)info;

/**
 * @abstract Called when a dedicated worker is created.
 * @param webView The WebView that receives dedicatedWorkerCreated event.
 * @param info The dedicated worker created info.
 */
- (void)webView:(MSWebView2WebView*)webView
    dedicatedWorkerCreated:(MSWebView2DedicatedWorkerCreatedInfo*)info;

@end

NS_ASSUME_NONNULL_END
