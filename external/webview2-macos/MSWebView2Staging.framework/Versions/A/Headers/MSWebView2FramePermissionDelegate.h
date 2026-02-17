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

@class MSWebView2Frame;

/**
 * Delegate for the permission requested events in iframe.
 */
@protocol MSWebView2FramePermissionDelegate <NSObject>

@optional

/**
 * @abstract Called when a permission is requested.
 *
 * @param frame The frame that requests the permission.
 * @param info The details about this request.
 * @param decisionHandler The decision handler to call to grant the permission
 * request or not.
 *
 * @discussion The decisionHandler has same usage as that of
 * `webView:permissionRequested:decisionHandler:` of
 * <MSWebView2PermissionDelegate>. For `handled` parameter, if it equals to
 * `NO`, the state will be ignored, the permission delegate method
 * `webView:permissionRequested:decisionHandler:` in main frame will be
 * triggered.
 *
 * @textblock
 * - (void)frame:(MSWebView2Frame*)frame
 *     permissionRequested:(MSWebView2PermissionRequestedInfo*)info
 *         decisionHandler:
 *             (void (^)(BOOL handled,
 *                       MSWebView2PermissionState state,
 *                       BOOL savesInProfile))decisionHandler {
 *   NSAlert *alert = [[NSAlert alloc] init];
 *   [alert addButtonWithTitle:@"Allow"];
 *   [alert addButtonWithTitle:@"Deny"];
 *   alert.messageText = @"Permission Request";
 *   alert.informativeText = [NSString
 *       stringWithFormat:
 *           @"Receive a permission request for %@ from iframe of website at "
 *           @"%@.\nThis request %@ from a user gesture.\nDo you want to grant "
 *           @"permission?",
 *           [self getTextStringByPermissionKind:info.permissionKind], info.uri,
 *           info.userInitiated ? @"came" : @"did not come"];
 *
 *   NSModalResponse response = [alert runModal];
 *   if (response == NSAlertFirstButtonReturn) {
 *     decisionHandler(YES, MSWebView2PermissionStateAllow, YES);
 *   } else if (response == NSAlertSecondButtonReturn) {
 *     decisionHandler(YES, MSWebView2PermissionStateDeny, NO);
 *   }
 * }
 * @/textblock
 */
- (void)frame:(MSWebView2Frame*)frame
    permissionRequested:(MSWebView2PermissionRequestedInfo*)info
        decisionHandler:(void (^)(BOOL handled,
                                  MSWebView2PermissionState state,
                                  BOOL savesInProfile))decisionHandler;

@end

NS_ASSUME_NONNULL_END
