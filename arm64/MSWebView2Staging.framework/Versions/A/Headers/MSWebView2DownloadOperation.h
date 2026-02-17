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

/**
 * The state of the download. A download can be in progress, interrupted, or
 * completed.
 */
typedef NS_ENUM(NSInteger, MSWebView2DownloadState) {
  /**
   * The download is in progress.
   */
  MSWebView2DownloadStateInProgress,
  /**
   * The connection with the file host was broken. The `interruptReason`
   * property can be accessed from `MSWebView2DownloadOperation`. See
   * `MSWebView2DownloadInterruptReason` for descriptions of kinds of
   * interrupt reasons. Host can check whether an interrupted download can be
   * resumed with the `canResume` property on the `MSWebView2DownloadOperation`.
   * Once resumed, a download is in the `MSWebView2DownloadStateInProgress`
   * state.
   */
  MSWebView2DownloadStateInterrupted,
  /**
   * The download completed successfully.
   */
  MSWebView2DownloadStateCompleted,
};

/**
 * The reason why connection with file host was broken.
 */
typedef NS_ENUM(NSInteger, MSWebView2DownloadInterruptReason) {
  /**
   * No error occurred.
   */
  MSWebView2DownloadInterruptReasonNone,
  /**
   * Generic file error.
   */
  MSWebView2DownloadInterruptReasonFileFailed,
  /**
   * Access denied due to security restrictions.
   */
  MSWebView2DownloadInterruptReasonFileAccessDenied,
  /**
   * Disk full. User should free some space or choose a different location to
   * store the file.
   */
  MSWebView2DownloadInterruptReasonFileNoSpace,
  /**
   * Result file path with file name is too long.
   */
  MSWebView2DownloadInterruptReasonFileNameTooLong,
  /**
   * File is too large for file system.
   */
  MSWebView2DownloadInterruptReasonFileTooLarge,
  /**
   * Microsoft Defender Smartscreen detected a virus in the file.
   */
  MSWebView2DownloadInterruptReasonFileMalicious,
  /**
   * File was in use, too many files opened, or out of memory.
   */
  MSWebView2DownloadInterruptReasonFileTransientError,
  /**
   * File blocked by local policy.
   */
  MSWebView2DownloadInterruptReasonFileBlockedByPolicy,
  /**
   * Security check failed unexpectedly. Microsoft Defender SmartScreen could
   * not scan this file.
   */
  MSWebView2DownloadInterruptReasonFileSecurityCheckFailed,
  /**
   * Seeking past the end of a file in opening a file, as part of resuming an
   * interrupted download. The file did not exist or was not as large as
   * expected. Partially downloaded file was truncated or deleted, and download
   * will be
   */
  MSWebView2DownloadInterruptReasonFileTooShort,
  /**
   * Partial file did not match the expected hash and was deleted. Download
   * will be restarted automatically.
   */
  MSWebView2DownloadInterruptReasonFileHashMismatch,
  /**
   * Generic network error. User can retry the download manually.
   */
  MSWebView2DownloadInterruptReasonNetworkFailed,
  /**
   * Network operation timed out.
   */
  MSWebView2DownloadInterruptReasonNetworkTimeout,
  /**
   * Network connection lost. User can retry the download manually.
   */
  MSWebView2DownloadInterruptReasonNetworkDisconnected,
  /**
   * Server has gone down. User can retry the download manually.
   */
  MSWebView2DownloadInterruptReasonNetworkServerDown,
  /**
   * Network request invalid because original or redirected URI is invalid, has
   * an unsupported scheme, or is disallowed by network policy.
   */
  MSWebView2DownloadInterruptReasonNetworkInvalidRequest,
  /**
   * Generic server error. User can retry the download manually.
   */
  MSWebView2DownloadInterruptReasonServerFailed,
  /**
   * Server does not support range requests.
   */
  MSWebView2DownloadInterruptReasonServerNoRange,
  /**
   * Server does not have the requested data.
   */
  MSWebView2DownloadInterruptReasonServerBadContent,
  /**
   * Server did not authorize access to resource.
   */
  MSWebView2DownloadInterruptReasonServerUnauthorized,
  /**
   * Server certificate problem.
   */
  MSWebView2DownloadInterruptReasonServerCertificateProblem,
  /**
   * Server access forbidden.
   */
  MSWebView2DownloadInterruptReasonServerForbidden,
  /**
   * Unexpected server response. Responding server may not be intended server.
   * User can retry the download manually.
   */
  MSWebView2DownloadInterruptReasonServerUnexpectedResponse,
  /**
   * Server sent fewer bytes than the content-length header. Content-length
   * header may be invalid or connection may have closed. Download is treated
   * as complete unless there are
   * [strong validators](https://tools.ietf.org/html/rfc7232#section-2) present
   * to interrupt the download.
   */
  MSWebView2DownloadInterruptReasonServerContentLengthMismatch,
  /**
   * Unexpected cross-origin redirect.
   */
  MSWebView2DownloadInterruptReasonServerCrossOriginRedirect,
  /**
   * User canceled the download.
   */
  MSWebView2DownloadInterruptReasonUserCanceled,
  /**
   * User shut down the WebView. Resuming downloads that were interrupted
   * during shutdown is not yet supported.
   */
  MSWebView2DownloadInterruptReasonUserShutdown,
  /**
   * User paused the download.
   */
  MSWebView2DownloadInterruptReasonUserPaused,
  /**
   * WebView crashed.
   */
  MSWebView2DownloadInterruptReasonDownloadProcessCrashed,
};

NS_ASSUME_NONNULL_BEGIN
/**
 * Represents a download operation. Gives access to the download's metadata
 * and supports a user canceling, pausing, or resuming the download.
 */
@interface MSWebView2DownloadOperation : NSObject

/**
 * @abstract The URI of the download.
 */
@property(nonatomic, copy, readonly) NSString* URI;

/**
 * @abstract The Content-Disposition header value from the download's HTTP
 * response. If none, the value is an empty string.
 */
@property(nonatomic, copy, readonly) NSString* contentDisposition;

/**
 * @abstract MIME type of the downloaded content.
 */
@property(nonatomic, copy, readonly) NSString* mimeType;

/**
 * @abstract The expected size of the download in total number of bytes based on
 * the HTTP Content-Length header. Returns -1 if the size is unknown.
 */
@property(nonatomic, readonly) int64_t totalBytesToReceive;

/**
 * @abstract The number of bytes that have been written to the download file.
 */
@property(nonatomic, readonly) int64_t bytesReceived;

/**
 * @abstract The estimated end time.
 */
@property(nonatomic, readonly) NSDate* estimatedEndTime;

/**
 * @abstract The absolute path to the download file, including file name. Host
 * can change this from `MSWebView2DownloadStartingInfo`.
 */
@property(nonatomic, readonly) NSString* resultFilePath;

/**
 * @abstract The state of the download. A download can be in progress,
 * interrupted, or completed. See `MSWebView2DownloadState` for descriptions of
 * states.
 */
@property(nonatomic, readonly) MSWebView2DownloadState state;

/**
 * @abstract The reason why connection with file host was broken.
 */
@property(nonatomic, readonly)
    MSWebView2DownloadInterruptReason interruptReason;

/**
 * @abstract Returns YES if an interrupted download can be resumed. Downloads
 * with the following interrupt reasons may automatically resume without you
 * calling any methods:
 * `MSWebView2DownloadInterruptReasonServerNoRange`,
 * `MSWebView2DownloadInterruptReasonFileHashMismatch`,
 * `MSWebView2DownloadInterruptReasonFileTooShort`.
 * In these cases download progress may be restarted with `bytesReceived`
 * reset to 0.
 */
@property(nonatomic, readonly) BOOL canResume;

/**
 * @abstract Cancel the download.
 */
- (void)cancel;

/**
 * @abstract Pause the download.
 */
- (void)pause;

/**
 * @abstract Resume the download.
 */
- (void)resume;

@end

NS_ASSUME_NONNULL_END
