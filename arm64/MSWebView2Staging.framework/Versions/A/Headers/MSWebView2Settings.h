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
 * Defines properties that enable, disable, or modify WebView features. Setting
 * changes(except `userAgent`) made after `navigationStarting` event do not
 * apply until the next top-level navigation. Setting changes made to
 * `userAgent` take effects immediately.
 */
@interface MSWebView2Settings : NSObject

- (instancetype)init NS_UNAVAILABLE;

/**
 * @abstract The `userAgent` property that specifies the User-Agent header.
 *
 * @discussion The default value is the default User Agent of the Microsoft Edge
 * browser. This property can be overridden if a non-empty value is reassigned.
 * If this property value is nil after reassigned a non-empty value, that
 * usually means the assignment failed due to WebView being closed.
 *
 * The User-Agent set will also be effective on service workers and shared
 * workers associated with the WebView. If there are multiple WebViews
 * associated with the same service worker or shared worker, the last User Agent
 * set will be used.
 */
@property(nullable, nonatomic, copy) NSString* userAgent;

/**
 * @abstract The `defaultContextMenusEnabled` property is used to prevent
 * default context menus from being shown to user in WebView.
 *
 * @discussion The default value is `YES`.
 */
@property(nonatomic, getter=areDefaultContextMenusEnabled)
    BOOL defaultContextMenusEnabled;

/**
 * @abstract The `devToolsEnabled` controls whether the user is able to use
 * the context menu or keyboard shortcuts to open the DevTools window.
 *
 * @discussion The default value is `YES`.
 */
@property(nonatomic, getter=areDevToolsEnabled) BOOL devToolsEnabled;

/**
 * @abstract The `swipeNavigationEnabled` property is used to enable or
 * disable horizontal swipe navigation in WebView.
 * @discussion The default value is `YES`. This property will take effect
 * immediately.
 */
@property(nonatomic, getter=isSwipeNavigationEnabled)
    BOOL swipeNavigationEnabled;

/**
 * @abstract The `builtInErrorPageEnabled` property is used to disable built
 * in error page for navigation failure and render process failure.
 *
 * @discussion When disabled, a blank page is displayed when the related error
 * happens. The default value is `YES`.
 */
@property(nonatomic, getter=isBuiltInErrorPageEnabled)
    BOOL builtInErrorPageEnabled;

/**
 * @abstract The `scriptEnabled` property controls if running JavaScript is
 * enabled in all future navigations in WebView.
 *
 * @discussion This only affects scripts in the document. Scripts injected with
 * ExecuteScript runs even if script is disabled. The default value is `YES`.
 */
@property(nonatomic, getter=isScriptEnabled) BOOL scriptEnabled;

/**
 * @abstract The `statusBarEnabled` controls whether the status bar is
 * displayed.
 *
 * @discussion The status bar is usually displayed in the lower left of the web
 * view and shows things such as the URI of a link when the user hovers over it
 * and other information. The default value is `YES`. The status bar UI can be
 * altered by web content and should not be considered secure.
 */
@property(nonatomic, getter=isStatusBarEnabled) BOOL statusBarEnabled;

/**
 * @abstract The `webMessageEnabled` property is used when loading a new HTML
 * document.
 *
 * @discussion If set to `YES`, communication from the host to the top-level
 * HTML document of the WebView is allowed using `postWebMessageAsJSON`,
 * `postWebMessageAsString`, and message event of `window.chrome.webview`. For
 * more information, navigate to `postWebMessageAsJSON` API in
 * MSWebView2WebView. Communication from the top-level HTML document of the web
 * view to the host is allowed using the `postMessage` function of
 * `window.chrome.webview` and `webMessageReceived` method in
 * MSWebView2WebMessageDelegate. For more information, navigate to
 * MSWebView2WebMessageDelegate. If set to `NO`, then bidirectional
 * communications are disallowed. The default value is `YES`.
 */
@property(nonatomic, getter=isWebMessageEnabled) BOOL webMessageEnabled;

/**
 * @abstract The `browserAcceleratorKeysEnabled` determines whether
 * browser-specific accelerator keys are enabled.
 *
 * @discussion When `browserAcceleratorKeysEnabled` is set to `NO`, it disables
 * all accelerator keys that access features specific to a web browser,
 * including but not limited to:
 *  - Command-F for Find
 *  - Command-LeftArrow for Back
 *  - Command-RightArrow for Forward
 *  - Command-P for Print
 *  - Command-Shift-C for Dev Tools Inspect
 *  - Command-Option-C for Dev Tools Inspect
 *  - Command-Option-I for Dev Tools
 *  - Command-Option-J for Dev Tools Console
 *  - Command-Option-U for View Source
 *  - Command-0 for Zoom Normal
 *  - Command-Minus for Zoom Minus
 *  - Command-Equal for Zoom Plus
 *  - Command-Shift-Equal for Zoom Plus
 *  - Command-G for Find Next
 *  - Command-Shift-G for Find Previous
 *  - Command-R for Reload
 *  - Command-Shift-R for Reload Bypassing Cache
 *  - Command-Period for Stop
 *
 * It does not disable accelerator keys related to movement and text editing,
 * such as:
 *  - Command-UpArrow for Page Up, Command-DownArrow for Page Down
 *  - Command-X for Cut, Command-C for Copy, Command-V for Paste
 *  - Command-A for Select All
 *  - Command-Z for Undo
 *
 * Those accelerator keys will always be enabled unless they are handled in the
 * `AcceleratorKeyPressed` event.
 *
 * This setting has no effect on the `AcceleratorKeyPressed` event. The event
 * will be fired for all accelerator keys, whether they are enabled or not.
 *
 * The default value for `browserAcceleratorKeysEnabled` is `YES`.
 */
@property(nonatomic, getter=areBrowserAcceleratorKeysEnabled)
    BOOL browserAcceleratorKeysEnabled;

/**
 * @abstract The `zoomControlEnabled` property is used to prevent the user
 * from impacting the zoom of the WebView.
 *
 * @discussion When disabled, the user is not able to zoom using Command++ or
 * Command+-, but the `zoomFactor` API still works no matter the
 * `zoomControlEnabled` is disabled or not. The default value is `YES`.
 */
@property(nonatomic, getter=isZoomControlEnabled) BOOL zoomControlEnabled;

/**
 * @abstract Determines whether host objects are accessible from the page
 *
 * @discussion The default value is `YES`.
 */
@property(nonatomic, getter=areHostObjectsAllowed) BOOL hostObjectsAllowed;

/**
 * @abstract Determines whether rendering the default JavaScript dialog box.
 *
 * @discussion `defaultScriptDialogsEnabled` is used when loading a new HTML
 * document. If set to `NO`, WebView does not render the default JavaScript
 * dialog box (Specifically those displayed by the JavaScript alert,
 * confirm, prompt functions and `beforeunload` event). Instead, if developer
 * implements <MSWebView2WebViewDelegate> method
 * `webView:scriptDialogOpening:decisionHandler:`, WebView will call it that
 * contains all of the information for the dialog and allow the host app to show
 * a custom UI.
 *
 * The default value is `YES`.
 */
@property(nonatomic, getter=areDefaultScriptDialogsEnabled)
    BOOL defaultScriptDialogsEnabled;

/**
 * @abstract Pinch-zoom, referred to as "Page Scale" zoom, is performed as a
 * post-rendering step, it changes the page scale factor property and scales
 * the surface the web page is rendered onto when user performs a pinch
 * zooming action.
 *
 * @discussion It does not change the layout but rather changes the
 * viewport and clips the web content, the content outside of the viewport
 * isn't visible onscreen and users can't reach this content using mouse.
 *
 * The `pinchZoomEnabled` property enables or disables the ability of the end
 * user to use a pinching motion on touch input enabled devices to scale the
 * web content in the WebView. It defaults to `YES`. When set to `NO`, the end
 * user cannot pinch zoom after the next navigation. Disabling/Enabling
 * `pinchZoomEnabled` only affects the end user's ability to use pinch motions
 * and does not change the page scale factor.
 *
 * This property only affects the Page Scale zoom and has no effect on the
 * existing browser zoom properties (`zoomControlEnabled` and `zoomFactor`) or
 * other end user mechanisms for zooming.
 */
@property(nonatomic, getter=isPinchZoomEnabled) BOOL pinchZoomEnabled;

@end

NS_ASSUME_NONNULL_END
