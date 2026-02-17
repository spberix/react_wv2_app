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

#import <AppKit/AppKit.h>

#import "MSWebView2ControllerDelegate.h"
#import "MSWebView2MoveFocusReason.h"
#import "MSWebView2WebView.h"

NS_ASSUME_NONNULL_BEGIN

/**
 * The owner of the <MSWebView2WebView> object that provides support for
 * resizing, showing and hiding, focusing, and other functionality related to
 * windowing and composition. The MSWebView2Controller object must be
 * deallocated on the main thread that creates the controller, deallocating on
 * other threads will throw NSException.
 */
@interface MSWebView2Controller : NSObject

- (instancetype)init NS_UNAVAILABLE;

/**
 * @abstract Closes the WebView and cleans up the underlying browser instance.
 * Cleaning up the browser instance releases the resources powering the
 * WebView. The browser instance is shut down if no other WebViews are
 * using it.
 *
 * @discussion After running `close`, most methods will fail and event
 * delegates stop running. Specifically, the WebView releases the associated
 * references to any associated event delegates when `close` is run. Actually
 * `close` is implicitly run when the MSWebViewController loses the final
 * reference and is destructed. But it is best practice to explicitly run
 * `close` to avoid any accidental cycle of references between the WebView and
 * the app code. Specifically, if you capture a reference to the WebView in an
 * event delegate you create a reference cycle between the WebView and the event
 * delegate. Run `close` to break the cycle by releasing all event delegates.
 * But to avoid the situation, it is best to both explicitly run `close` on the
 * WebView and to not capture a reference to the WebView to ensure the WebView
 * is cleaned up correctly. The `close` must be called on the main thread that
 * creates the controller, deallocating on other threads will throw NSException.
 */
- (void)close;

/**
 * @abstract Moves focus into WebView.
 *
 * @param reason Indicates which element of the WebView will be focused.
 *
 * @discussion WebView gets focus and focus is set to correspondent element in
 * the page hosted in the WebView. For `Programmatic` reason, focus is set to
 * previously focused element or the default element if no previously focused
 * element exists. For `Next` reason, focus is set to the first element. For
 * `Previous` reason, focus is set to the last element.
 */
- (void)moveFocus:(MSWebView2MoveFocusReason)reason;

/**
 * @abstract <MSWebView2WebView> instance owned by MSWebView2Controller.
 */
@property(readonly, nonatomic, strong) MSWebView2WebView* webview;

/**
 * @abstract Gets or sets the color that renders underneath all web content.
 *
 * @discussion WebView renders this color when there is no web content loaded
 * such as before the initial navigation or between navigations. Web pages with
 * undefined CSS background properties or background properties containing
 * transparent pixels will render their contents over this color. Web pages with
 * defined and opaque background properties that span the page will obscure the
 * `defaultBackgroundColor` and display normally.
 *
 * The default value for this property is white to resemble the
 * native browser experience.
 *
 * If set `defaultBackgroundColor` with `nil`, WebView will reset the
 * `defaultBackgroundColor` to white.
 *
 * The alpha property of `defaultBackgroundColor` must be equal to 0.0
 * or 1.0.
 *
 * This value may also be set by using the `WEBVIEW2_DEFAULT_BACKGROUND_COLOR`
 * environment variable. There is a known issue with background color where
 * setting the color by API can still leave the app with a white flicker before
 * the `defaultBackgroundColor` takes effect. Setting the color via environment
 * variable solves this issue. The value must be a hex value that can optionally
 * prepend a 0x. The value must account for the alpha value which is represented
 * by the first 2 digits. So any hex value fewer than 8 digits will assume a
 * prepended 00 to the hex value and result in a transparent color.
 * get_DefaultBackgroundColor will return the result of this environment
 * variable if used. This environment variable can only set the
 * DefaultBackgroundColor once. Subsequent updates to background color must be
 * done through API call.
 *
 * @textblock
 * self.controller.defaultBackgroundColor =
 *                   [NSColor colorWithRed:1.0 green:0.0 blue:0.0 alpha:1.0];
 * @/textblock
 */
@property(nonatomic, null_resettable, copy) NSColor* defaultBackgroundColor;

/**
 * @abstract The WebView's controller delegate.
 */
@property(nullable, nonatomic, weak) id<MSWebView2ControllerDelegate> delegate;

/**
 * @abstract Gets or sets the ZoomFactor that applies on the current web
 * content.
 *
 * @discussion This zoom factor applies across navigations. When the zoom
 * factor is changed by the user (resulting in the app receiving
 * ZoomFactorChanged event), that zoom applies only for the current page and
 * will be reset on a navigation.
 *
 * Specifying a ZoomFactor less than or equal to 0 is not allowed.
 *
 * WebView also has an internal supported zoom factor range.
 * When a specified zoom factor is out of that range, it is normalized to be
 * within the range, and a ZoomFactorChanged event is raised for the real
 * applied zoom factor. When this range normalization happens, this reports the
 * zoom factor specified during the previous modification of the ZoomFactor
 * property until the ZoomFactorChanged event is received after WebView applies
 * the normalized zoom factor.
 */
@property(nonatomic) double zoomFactor;

/**
 * @abstract Determines whether to show or hide the WebView.
 *
 * @discussion This property must be used for WebViews created with
 * `autoUpdateVisibilityEnabled` set to `NO` in `MSWebView2ControllerOptions`.
 *
 * The `visible` property is synchronized with the JavaScript
 * `document.visibilityState`. When `visible` is set to `NO`, the WebView
 * becomes transparent and stops rendering content. Note that this does not
 * affect the `NSView` containing the WebView (the `NSView` passed to
 * `createWebViewController:completionHandler:`). To hide that `NSView` as well,
 * set its `hidden` property directly.
 *
 * For optimal performance, set `visible` to `NO` when the app window is hidden
 * and back to `YES` when the app window is restored. If
 * `autoUpdateVisibilityEnabled` is set to `YES` during WebView creation, this
 * behavior is managed automatically by the WebView SDK to enhance performance.
 *
 * When the visibility is hidden, there are significant CPU and memory benefits.
 * For example, Chromium throttles page activities like animations and reduces
 * the frequency of certain tasks. Similarly, WebView purges caches to lower
 * memory usage.
 *
 * @exception NSGenericException Thrown if a WebView is created without setting
 * `autoUpdateVisibilityEnabled` to `NO` in `MSWebView2ControllerOptions`.
 */
@property(nonatomic, getter=isVisible) BOOL visible;

@end

NS_ASSUME_NONNULL_END
