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

#import "MSWebView2Foundation.h"

/**
 * MSWebViewErrorDomain Indicates a Microsoft WebView2 error.
 */
MS_WEBVIEW2_EXTERN NSString* const MSWebView2ErrorDomain;

/**
 * Constants used by NSError to indicate errors in the MSWebView2ErrorDomain.
 */
typedef NS_ENUM(NSInteger, MSWebView2ErrorCode) {
  /**
   * Indicates that an operation aborted.
   */
  MSWebView2ErrorAbort = 1,
  /**
   * Indicates that create user data folder failed.
   */
  MSWebView2ErrorCreateUserDataFolderFailed,
  /**
   * Indicates that an unspecificed failure.
   */
  MSWebView2ErrorFailed,
  /**
   * Indicates that executable file is not found.
   */
  MSWebView2ErrorExecutableFileNotFound,
  /**
   * Indicates that argument is not valid.
   */
  MSWebView2ErrorInvalidArgument,
  /**
   * Indicates that the group or resource is not in the correct state to perform
   * the requested operation.
   */
  MSWebView2ErrorInvalidState,
  /**
   * Indicates that named platform channel is invalid.
   */
  MSWebView2ErrorInvalidNamedPlatformChannel,
  /**
   * Indicates that launch process failed
   */
  MSWebView2ErrorLaunchProcessFailed,
  /**
   * Indicates that client library entry point is not found.
   */
  MSWebView2ErrorNoClientEntryPoint,
  /**
   * Indicates that the error is catastrophic failure.
   */
  MSWebView2ErrorUnexpected,
  /**
   * Indicates that the invoked object has disconnected from its client.
   */
  MSWebView2ErrorRpcDisconnected,
  /**
   * Indicates that the WebView2 runtime is unavailable.
   */
  MSWebView2ErrorRuntimeUnavailable,
  /**
   * Indicates that the launched runtime process hangs for a long time than
   * expected.
   */
  MSWebView2ErrorRuntimeProcessHang,
  /**
   * Indicates that the profile has been marked as deleted.
   */
  MSWebView2ErrorProfileDeletePending,
};
