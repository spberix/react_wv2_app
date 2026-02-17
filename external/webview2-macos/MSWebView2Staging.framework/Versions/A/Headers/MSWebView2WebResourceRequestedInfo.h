/*
 * Copyright (C) Microsoft Corporation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *   * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *   * The name of Microsoft Corporation, or the names of its
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

#import "MSWebView2WebResourceRequest.h"

NS_ASSUME_NONNULL_BEGIN

/**
 * Specifies the web resource request contexts.
 */
typedef NS_ENUM(NSInteger, MSWebView2WebResourceContext) {
  /**
   * Specifies all resources.
   */
  MSWebView2WebResourceContextAll,
  /**
   * Specifies a document resource.
   */
  MSWebView2WebResourceContextDocument,
  /**
   * Specifies a CSS resource.
   */
  MSWebView2WebResourceContextStyleSheet,
  /**
   * Specifies an image resource.
   */
  MSWebView2WebResourceContextImage,
  /**
   * Specifies another media resource such as a video.
   */
  MSWebView2WebResourceContextMedia,
  /**
   * Specifies a font resource.
   */
  MSWebView2WebResourceContextFont,
  /**
   * Specifies a script resource.
   */
  MSWebView2WebResourceContextScript,
  /**
   * Specifies an XML HTTP request.
   */
  MSWebView2WebResourceContextXMLHTTPRequest,
  /**
   * Specifies a Fetch API communication.
   */
  MSWebView2WebResourceContextFetch,
  /**
   * Specifies a TextTrack resource.
   */
  MSWebView2WebResourceContextTextTrack,
  /**
   * Specifies an EventSource API communication.
   */
  MSWebView2WebResourceContextEventSource,
  /**
   * Specifies a WebSocket API communication.
   */
  MSWebView2WebResourceContextWebSocket,
  /**
   * Specifies a Web App Manifest.
   */
  MSWebView2WebResourceContextManifest,
  /**
   * Specifies a Signed HTTP Exchange.
   */
  MSWebView2WebResourceContextSignedExchange,
  /**
   * Specifies a Ping request.
   */
  MSWebView2WebResourceContextPing,
  /**
   * Specifies a CSP Violation Report.
   */
  MSWebView2WebResourceContextCSPViolationReport,
  /**
   * Specifies an other resource.
   */
  MSWebView2WebResourceContextOther,
};

/**
 * Specifies the source of `WebResourceRequested` event.
 */
typedef NS_OPTIONS(uint32_t, MSWebView2WebResourceRequestSourceKinds) {
  /**
   * Indicates that none request source.
   */
  MSWebView2WebResourceRequestSourceKindsNone = 0,
  /**
   * Indicates that web resource is requested from main page including dedicated
   * workers, iframes and main script for shared workers.
   */
  MSWebView2WebResourceRequestSourceKindsDocument = 1 << 0,
  /**
   * Indicates that web resource is requested from shared worker.
   */
  MSWebView2WebResourceRequestSourceKindsSharedWorker = 1 << 1,
  /**
   * Indicates that web resource is requested from service worker.
   */
  MSWebView2WebResourceRequestSourceKindsServiceWorker = 1 << 2,
  /**
   * Indicates that web resource is requested from any supported source.
   */
  MSWebView2WebResourceRequestSourceKindsAll = UINT32_MAX
};

/**
 * Info for the `WebResourceRequested` event.
 */
@interface MSWebView2WebResourceRequestedInfo : NSObject

/**
 * @abstract The web resource request.
 */
@property(nonatomic, copy) MSWebView2WebResourceRequest* request;

/**
 * @abstract The web resource request context.
 */
@property(nonatomic, readonly) MSWebView2WebResourceContext resourceContext;

/**
 * @abstract The web resource requested source.
 */
@property(nonatomic, readonly)
    MSWebView2WebResourceRequestSourceKinds requestedSourceKind;

@end

NS_ASSUME_NONNULL_END
