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

#import "MSWebView2BasicAuthenticationRequestedInfo.h"
#import "MSWebView2BasicAuthenticationResponse.h"
#import "MSWebView2BrowserProcessExitedInfo.h"
#import "MSWebView2ContentLoadingInfo.h"
#import "MSWebView2ContextMenuItem.h"
#import "MSWebView2ContextMenuRequestedInfo.h"
#import "MSWebView2ContextMenuTarget.h"
#import "MSWebView2Controller.h"
#import "MSWebView2ControllerDelegate.h"
#import "MSWebView2ControllerOptions.h"
#import "MSWebView2Cookie.h"
#import "MSWebView2CookieManager.h"
#import "MSWebView2DOMContentLoadedInfo.h"
#import "MSWebView2DedicatedWorker.h"
#import "MSWebView2DedicatedWorkerCreatedInfo.h"
#import "MSWebView2DedicatedWorkerDelegate.h"
#import "MSWebView2DevToolsProtocolEventInfo.h"
#import "MSWebView2DownloadDelegate.h"
#import "MSWebView2DownloadOperation.h"
#import "MSWebView2DownloadStartingInfo.h"
#import "MSWebView2Environment.h"
#import "MSWebView2EnvironmentDelegate.h"
#import "MSWebView2EnvironmentOptions.h"
#import "MSWebView2Error.h"
#import "MSWebView2ExecuteScriptResult.h"
#import "MSWebView2Foundation.h"
#import "MSWebView2Frame.h"
#import "MSWebView2FrameDelegate.h"
#import "MSWebView2FrameInfo.h"
#import "MSWebView2FrameNavigationDelegate.h"
#import "MSWebView2FramePermissionDelegate.h"
#import "MSWebView2FrameWebMessageDelegate.h"
#import "MSWebView2HTTPHeader.h"
#import "MSWebView2HTTPHeaders.h"
#import "MSWebView2Loader.h"
#import "MSWebView2NavigationCompletedInfo.h"
#import "MSWebView2NavigationDelegate.h"
#import "MSWebView2NavigationStartingDecision.h"
#import "MSWebView2NavigationStartingInfo.h"
#import "MSWebView2NewWindowDelegate.h"
#import "MSWebView2NewWindowRequestedInfo.h"
#import "MSWebView2PermissionDelegate.h"
#import "MSWebView2PermissionRequestedInfo.h"
#import "MSWebView2ProcessFailedInfo.h"
#import "MSWebView2ProcessInfo.h"
#import "MSWebView2Profile.h"
#import "MSWebView2ProfileDelegate.h"
#import "MSWebView2ScriptDialogOpeningInfo.h"
#import "MSWebView2ServiceWorker.h"
#import "MSWebView2ServiceWorkerActivationInfo.h"
#import "MSWebView2ServiceWorkerDelegate.h"
#import "MSWebView2ServiceWorkerManager.h"
#import "MSWebView2ServiceWorkerManagerDelegate.h"
#import "MSWebView2ServiceWorkerRegistration.h"
#import "MSWebView2ServiceWorkerRegistrationDelegate.h"
#import "MSWebView2ServiceWorkerRegistrationInfo.h"
#import "MSWebView2Settings.h"
#import "MSWebView2SharedWorker.h"
#import "MSWebView2SharedWorkerCreatedInfo.h"
#import "MSWebView2SharedWorkerDelegate.h"
#import "MSWebView2SharedWorkerManager.h"
#import "MSWebView2SharedWorkerManagerDelegate.h"
#import "MSWebView2SourceChangedInfo.h"
#import "MSWebView2TextureStream.h"
#import "MSWebView2TextureStreamDelegate.h"
#import "MSWebView2WebMessageDelegate.h"
#import "MSWebView2WebMessageReceivedInfo.h"
#import "MSWebView2WebResourceDelegate.h"
#import "MSWebView2WebResourceRequest.h"
#import "MSWebView2WebResourceRequestedInfo.h"
#import "MSWebView2WebResourceResponse.h"
#import "MSWebView2WebView.h"
#import "MSWebView2WebViewDelegate.h"
#import "MSWebView2WindowFeatures.h"
