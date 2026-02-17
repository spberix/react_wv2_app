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

#import "MSWebView2FrameInfo.h"

NS_ASSUME_NONNULL_BEGIN

/**
 * Specifies the process failure type
 */
typedef NS_ENUM(NSInteger, MSWebView2ProcessFailedKind) {
  /**
   * Indicates that the browser process ended unexpectedly.  The WebView
   * automatically moves to the Closed state.  The app has to recreate a new
   * WebView to recover from this failure.
   */
  MSWebView2ProcessFailedKindBrowserProcessExited = 0,
  /**
   * Indicates that the render process ended unexpectedly.  A new render process
   * is created automatically and navigated to an error page.  The app runs
   * `Reload()` to try to recover from the failure.
   */
  MSWebView2ProcessFailedKindRenderProcessExited,
  /**
   * Indicates that the render process is unresponsive.
   */
  MSWebView2ProcessFailedKindRenderProcessUnresponsive,
  /**
   * Indicates that a frame-only render process ended unexpectedly. The process
   * exit does not affect the top-level document, only a subset of the subframes
   * within it. The content in these frames is replaced with an error page in
   * the frame.
   */
  MSWebView2ProcessFailedKindFrameRenderProcessExited,
  /**
   * Indicates that a utility process ended unexpectedly.
   */
  MSWebView2ProcessFailedKindUtilityProcessExited,
  /**
   * Indicates that a sandbox helper process ended unexpectedly.
   */
  MSWebView2ProcessFailedKindSandboxHelperProcessExited,
  /**
   * Indicates that the GPU process ended unexpectedly.
   */
  MSWebView2ProcessFailedKindGPUProcessExited,
  /**
   * Indicates that a PPAPI plugin process ended unexpectedly.
   */
  MSWebView2ProcessFailedKindPpApiPluginProcessExited,
  /**
   * Indicates that a PPAPI plugin broker process ended unexpectedly.
   */
  MSWebView2ProcessFailedKindPpApiBrokerProcessExited,
  /**
   * Indicates that a process of unspecified kind ended unexpectedly.
   */
  MSWebView2ProcessFailedKindUnknownProcessExited,
};

/**
 * Specifies the process failure reason.
 */
typedef NS_ENUM(NSInteger, MSWebView2ProcessFailedReason) {
  /**
   * Indicates an unexpected process failure occurred.
   */
  MSWebView2ProcessFailedReasonUnexpected = 0,
  /**
   * Indicates the process became unresponsive. this only applies to the main
   * frame's render process.
   */
  MSWebView2ProcessFailedReasonUnresponsive,
  /**
   * Indicates the process was terminated. For example, from Task Manager.
   */
  MSWebView2ProcessFailedReasonTerminated,
  /**
   * Indicates the process crashed.
   */
  MSWebView2ProcessFailedReasonCrashed,
  /**
   * Indicates the process failed to launch.
   */
  MSWebView2ProcessFailedReasonLaunchFailed,
  /**
   * Indicates the process died due to running out of memory.
   */
  MSWebView2ProcessFailedReasonOutOfMemory,
};

/**
 * Interface depicts runtime process failure relative info.
 */
@interface MSWebView2ProcessFailedInfo : NSObject

/**
 * @abstract Refer to enum `MSWebView2ProcessFailedKind`.
 */
@property(nonatomic, readonly) MSWebView2ProcessFailedKind kind;

/**
 * @abstract Exit code for the failed runtime process, for telemetry purpose.
 */
@property(nonatomic, readonly) int32_t exitCode;

/**
 * @abstract Refer to enum `MSWebView2ProcessFailedReason`.
 */
@property(nonatomic, readonly) MSWebView2ProcessFailedReason reason;

/**
 * @abstract Description of runtime process.
 */
@property(nonatomic, readonly, copy) NSString* processDescription;

/**
 * @abstract Frame info when a Out-of-Process iframes(OOPIFs) render process
 * crash occurs.
 */
@property(nonatomic, readonly, copy, nullable)
    NSArray<MSWebView2FrameInfo*>* frameInfosForFailedProcess;

@end

NS_ASSUME_NONNULL_END
