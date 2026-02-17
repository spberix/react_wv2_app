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

#import "MSWebView2MoveFocusRequestedInfo.h"

@class MSWebView2Controller;

NS_ASSUME_NONNULL_BEGIN

/**
 * Delegate for the controller.
 */
@protocol MSWebView2ControllerDelegate <NSObject>

@optional

/**
 * @abstract Adds an event handler for the `AcceleratorKeyPressed` event.
 *
 * @param controller MSWebView2Controller type, the controller of
 * the WebView on which acceleratorKey was pressed.
 * @param event An keyEquivalent event, contains relative infos about
 * acceleratorKey.
 *
 * @return Returns YES if the host application handled the event, or returns NO.
 *
 * @discussion `AcceleratorKeyPressed` event fires when an accelerator key or
 * key combo is pressed.
 *
 * If return value is NO, WebView will continue to handle the event.
 *
 * @textblock
 * // Ask host whether to handle the key when an accelerator key is triggered.
 * - (BOOL)controller:(MSWebView2Controller*)controller
 *     acceleratorKeyPressed:(NSEvent*)event {
 *   if ([event type] == NSEventTypeKeyDown &&
 *       (([event modifierFlags] & NSEventModifierFlagCommand) != 0)) {
 *     NSString* keyTriggered = [event characters];
 *     if ([keyTriggered length] == 1) {
 *       unichar keyChar = [keyTriggered characterAtIndex:0];
 *       switch (keyChar) {
 *         case 'n':
 *           return ^(RootViewController* rootViewController) {
 *           [rootViewController CreateNewWindow];
 *         };
 *         case 'j':
 *           return ^(RootViewController* rootViewController) {
 *           [rootViewController CloseWebView];
 *         };
 *       }
 *     }
 *   }
 *   return NO;
 * }
 * @/textblock
 */
- (BOOL)controller:(MSWebView2Controller*)controller
    acceleratorKeyPressed:(NSEvent*)event;

/**
 * @abstract Adds an event handler for the `ZoomFactorChanged` event.
 *
 * @param controller MSWebView2Controller type, the controller of
 * the WebView on which zoomFactor changed.
 *
 * @discussion `ZoomFactorChanged` runs when the `ZoomFactor` property of the
 * WebView changes.  The event may run because the `ZoomFactor` property was
 * modified, or due to the user manually modifying the zoom.  When it is
 * modified using the `ZoomFactor` property, the internal zoom factor is
 * updated immediately and no `ZoomFactorChanged` event is triggered.
 *
 * WebView associates the last used zoom factor for each site.  It
 * is possible for the zoom factor to change when navigating to a different
 * page.  When the zoom factor changes due to a navigation change, the
 * `ZoomFactorChanged` event runs right after the `ContentLoading` event.
 *
 * @textblock
 * // Display the changed zoom factor in the title bar.
 * - (void)controllerZoomFactorChanged:(MSWebView2Controller*)controller {
 *   NSLog(@"new zoom factor %f", controller.zoomFactor);
 *   NSInteger zoom = (NSInteger)(controller.zoomFactor * 100);
 *   NSString* suffix =
 *       [NSString stringWithFormat:@"%@%ld%@", @"(Zoom:", (long)zoom, @"%)"];
 *   [self addWindowTitleSuffix:suffix];
 * }
 * @/textblock
 */
- (void)controllerZoomFactorChanged:(MSWebView2Controller*)controller;

/**
 * @abstract Called when WebView has focus.
 *
 * @param controller MSWebView2Controller type, the controller of
 * the WebView which has focus.
 */
- (void)gotFocus:(MSWebView2Controller*)controller;

/**
 * @abstract Called when WebView loses focus.
 *
 * @param controller MSWebView2Controller type, the controller of
 * the WebView which loses focus.
 *
 * @discussion In the case where `MoveFocusRequested` event is run, the focus is
 * still on WebView when `MoveFocusRequested` event runs. `LostFocus` only runs
 * afterwards when code of the app or default action of `MoveFocusRequested`
 * event set focus away from WebView.
 */
- (void)lostFocus:(MSWebView2Controller*)controller;

/**
 * @abstract Called when user tries to tab out of the WebView.
 *
 * @param controller MSWebView2Controller type, the controller of the WebView
 * where user is moving focus.
 * @param info The details about this tab out of WebView.
 *
 * @return Returns `YES` if the host application handled the event, or returns
 * `NO`.
 *
 * @discussion If the return value is `NO`, the default action will be taken.
 * The default action is to try to find the next tab stop view in the app and
 * try to move focus to that view. If no other view exists to move focus, focus
 * is cycled within the web content of the WebView.
 *
 * @textblock
 * - (BOOL)controller:(MSWebView2Controller*)controller
 *    moveFocusRequested:(MSWebView2MoveFocusRequestedInfo*)info {
 *  // When the user is about to tab out of the WebView, focus on a specific
 *  // native view.
 *  switch (info.reason) {
 *    case MSWebView2MoveFocusReasonPrevious:
 *    // Tab out of the WebView from the first element of the web page
 *    // DoSomethingToFocusNativeView();
 *    case MSWebView2MoveFocusReasonNext:
 *    // Tab out of the WebView from the last element of the web page
 *    // DoSomethingToFocusAnotherNativeView();
 *  }
 *  return YES;
 * }
 * @/textblock
 */
- (BOOL)controller:(MSWebView2Controller*)controller
    moveFocusRequested:(MSWebView2MoveFocusRequestedInfo*)info;

@end

NS_ASSUME_NONNULL_END
