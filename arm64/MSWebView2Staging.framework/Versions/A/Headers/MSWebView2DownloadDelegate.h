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

#import "MSWebView2DownloadStartingInfo.h"

NS_ASSUME_NONNULL_BEGIN

@class MSWebView2WebView;
/**
 * Delegate for the download starting event.
 */
@protocol MSWebView2DownloadDelegate <NSObject>

@optional

/**
 * @abstract Add an event handler for the `DownloadStarting` event. This event
 * is raised when a download has begun, blocking the default download dialog,
 * but not blocking the progress of the download.
 *
 * @param webView The web view that initiates the download.
 * @param info Details about the download starting event.
 * @param decisionHandler The decision handler called to handle the download
 * starting event.
 *
 * @discussion The host can choose to cancel a download, change the result file
 * path, and hide the default download dialog. If the host chooses to cancel the
 * download, the download is not saved, no dialog is shown, and the state is
 * changed to <MSWebView2DownloadInterruptReason> with interrupt reason
 * `MSWebView2DownloadInterruptReasonUserCanceled`. Otherwise, the download is
 * saved to the default path after the event completes, and default download
 * dialog is shown if the host did not choose to hide it. The host can change
 * the visibility of the download dialog using the `handled`. If the event is
 * not handled, downloads complete normally with the default dialog shown.
 */
- (void)webView:(MSWebView2WebView*)webView
    downloadStarting:(MSWebView2DownloadStartingInfo*)info
     decisionHandler:(void (^)(NSString* _Nullable resultFilePath,
                               BOOL handled,
                               BOOL shouldCancel))decisionHandler;

/**
 * @abstract Add an event handler for the `bytesReceivedChanged` event. This
 * event is raised when the number of bytes received changes.
 *
 * @param webView The web view that initiates the download.
 * @param downloadOperation The download operation that is in progress.
 * @param bytesReceived The number of bytes received.
 *
 * @discussion The host can use this event to update the UI with the number of
 * bytes received.
 */

- (void)webView:(MSWebView2WebView*)webView
       downloadOperation:(MSWebView2DownloadOperation*)downloadOperation
    bytesReceivedChanged:(int64_t)bytesReceived;

/**
 * @abstract Add an event handler for the `stateChanged` event. This event is
 raised
 * when the download operation state changes.

 * @param webView The web view that initiates the download.
 * @param downloadOperation The download operation that is in progress.
 * @param state The state of the download operation.
 *
 * @discussion The host can use this event to update the UI with the download
 operation state.
 */
- (void)webView:(MSWebView2WebView*)webView
    downloadOperation:(MSWebView2DownloadOperation*)downloadOperation
         stateChanged:(MSWebView2DownloadState)state;

/**
 * @abstract Add an event handler for the `estimatedEndTimeChanged` event. This
 * event is raised when the estimated end time of the download operation
 * changes.
 *
 * @param webView The web view that initiates the download.
 * @param downloadOperation The download operation that is in progress.
 * @param estimatedEndTime The estimated end time of the download operation.
 *
 * @discussion The host can use this event to update the UI with the estimated
 * end time of the download operation.
 */
- (void)webView:(MSWebView2WebView*)webView
          downloadOperation:(MSWebView2DownloadOperation*)downloadOperation
    estimatedEndTimeChanged:(NSDate*)estimatedEndTime;

/**
 * @abstract Add an event handler for the `IsDefaultDownloadDialogOpenChanged`
 * event. This event is raised when the default download dialog's visibility
 * changes.
 *
 * @param webView The web view that initiates the download.
 *
 * @discussion The host can use this event to update the UI with the visibility
 * of the default download dialog.
 */
- (void)webViewIsDefaultDownloadDialogOpenChanged:(MSWebView2WebView*)webView;

@end

NS_ASSUME_NONNULL_END
