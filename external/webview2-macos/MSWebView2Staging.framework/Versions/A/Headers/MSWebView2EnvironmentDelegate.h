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

#import "MSWebView2BrowserProcessExitedInfo.h"

@class MSWebView2Environment;

NS_ASSUME_NONNULL_BEGIN

/**
 * Delegate for the environment.
 */
@protocol MSWebView2EnvironmentDelegate <NSObject>

@optional

/**
 * @abstract Called when the entire collection of WebView Runtime processes
 * for the browser process exit.
 *
 * @param environment The environment on which `BrowserProcessExited` event
 * happens.
 * @param info The details about `BrowserProcessExited` event.
 *
 * @discussion Multiple app processes can share a browser process by creating
 * their WebViews from a <MSWebView2Environment> with the same user data folder.
 * When the entire collection of WebView Runtime processes for the browser
 * process exit, all associated `MSWebView2Environment` objects receive the
 * `BrowserProcessExited` event. Multiple processes sharing the same browser
 * process need to coordinate their use of the shared user data folder to avoid
 * race conditions and unnecessary waits. For example, one process should not
 * clear the user data folder at the same time that another process recovers
 * from a crash by recreating its WebView controls; one process should not block
 * waiting for the event if other app processes are using the same browser
 * process (the browser process will not exit until those other processes have
 * closed their WebViews too).
 *
 * The difference between `BrowserProcessExited` and `ProcessFailed` is that
 * `BrowserProcessExited` is raised for any browser process exit (expected
 * or unexpected, after all associated processes have exited too), while
 * `ProcessFailed` is raised for unexpected process exits of any kind
 * (browser, render, GPU, and all other types), or for main frame
 * render process unresponsiveness.
 *
 * In the case the browser process crashes, both `BrowserProcessExited` and
 * `ProcessFailed` events are raised, but the order is not guaranteed. These
 * events are intended for different scenarios. It is up to the app to
 * coordinate the handlers so they do not try to perform reliability recovery.
 *
 * It's not recommended to call any WebView API in this method, if you have to
 * do it, be careful of catching exception when calling some APIs.
 *
 * @textblock
 * - (void)environment:(MSWebView2Environment*)environment
 *     browserProcessExited:(MSWebView2BrowserProcessExitedInfo*)info {
 *   // If a new WebView is created from this MSWebView2Environment after
 *   // the browser has exited but before our handler gets to run, a new
 *   // browser process will be created and lock the user data folder
 *   // again. Do not attempt to cleanup the user data folder in these
 *   // cases. If this happens, the PID of the new browser process will be
 *   // different to the PID of the older process, we check against the
 *   // PID of the browser process to which our last WebView attached.
 *   if (info.browserProcessID == self.newestBrowserPID) {
 *     // Watch for graceful browser process exit.
 *     if (info.browserProcessExitKind ==
 *              MSWebView2BrowserProcessExitKindNormal) {
 *       [self cleanupUserDataFolder];
 *       // Release the environment only after the handler is invoked.
 *       self.environment = nil;
 *     }
 *   } else {
 *     [self showAlert:@"A new browser process prevented cleanup of the user
 *                     @"data folder."];
 *   }
 * }
 * @/textblock
 */
- (void)environment:(MSWebView2Environment*)environment
    browserProcessExited:(MSWebView2BrowserProcessExitedInfo*)info;

@end

NS_ASSUME_NONNULL_END
