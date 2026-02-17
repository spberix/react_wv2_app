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

#import "MSWebView2PermissionRequestedInfo.h"
#import "MSWebView2PermissionState.h"

NS_ASSUME_NONNULL_BEGIN

@class MSWebView2WebView;

/**
 * Delegate for the permission requested events.
 */
@protocol MSWebView2PermissionDelegate <NSObject>

@optional

/**
 * @abstract Called when a permission is requested.
 *
 * @param webView The WebView that requests the permission.
 * @param info The details about this request.
 * @param decisionHandler The decision handler to call to grant the permission
 * request or not.
 *
 * @discussion The decision handler only applies to the current
 * request, and does not prevent the `PermissionRequested` event from getting
 * raised for new requests. However, for some permission kinds the WebView will
 * avoid creating a new request due to browser behavior. It has description in
 * `setPermission:origin:state:completionHandler:` of <MSWebView2Profile>.
 *
 * `state` is the permission state to set for this request.
 * `savesInProfile` gives developers an option whether permission
 * state set from the `PermissionRequested` event is saved in the profile or
 * not. Set the `savesInProfile` parameter to `NO` to not persist the state
 * beyond the current request and continue to receive `PermissionRequested`
 * event for this origin and permission kind.
 *
 * @textblock
 * - (void)webView:(MSWebView2WebView*)webView
 *     permissionRequested:(MSWebView2PermissionRequestedInfo*)info
 *         decisionHandler:
 *             (void (^)(MSWebView2PermissionState state,
 *                       BOOL savesInProfile))decisionHandler {
 *   NSAlert* alert = [[NSAlert alloc] init];
 *   [alert addButtonWithTitle:@"Allow"];
 *   [alert addButtonWithTitle:@"Deny"];
 *   alert.messageText = @"Permission Request";
 *   alert.informativeText = [NSString
 *       stringWithFormat:
 *           @"Receive a permission request for %@ from website at %@.\nThis "
 *           @"request %@ from a user gesture.\nDo you want to grant "
 *           @"permission?",
 *           [self getTextStringByPermissionKind:info.permissionKind], info.uri,
 *           info.userInitiated ? @"came" : @"did not come"];
 *
 *   NSModalResponse response = [alert runModal];
 *   if (response == NSAlertFirstButtonReturn) {
 *     decisionHandler(MSWebView2PermissionStateAllow, YES);
 *   } else if (response == NSAlertSecondButtonReturn) {
 *     decisionHandler(MSWebView2PermissionStateDeny, NO);
 *   }
 * }
 * @/textblock
 */
- (void)webView:(MSWebView2WebView*)webView
    permissionRequested:(MSWebView2PermissionRequestedInfo*)info
        decisionHandler:(void (^)(MSWebView2PermissionState state,
                                  BOOL savesInProfile))decisionHandler;

@end

NS_ASSUME_NONNULL_END
