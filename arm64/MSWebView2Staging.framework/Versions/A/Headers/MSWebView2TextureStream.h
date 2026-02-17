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
#import <IOSurface/IOSurface.h>

#import "MSWebView2TextureStreamDelegate.h"

NS_ASSUME_NONNULL_BEGIN

/**
 * WebView2TextureStream enables to send video data from host application to
 * web.
 * An exception will be raised if APIs are not called on UI thread or connection
 * is not built with runtime.
 */
@interface MSWebView2TextureStream : NSObject

- (instancetype)init NS_UNAVAILABLE;

/**
 * @abstract Adds an allowed URL origin for the given stream id.
 *
 * @param origin URL origin.
 * @param allowedInWebTexture Whether allowed the origin in WebTexture
 * @discussion The origin must be registered first in order for the stream
 * request to succeed. The stream requests could be made from any frame,
 * including iframes.
 *
 * The origin does not support wildcard, and it should have a scheme like
 * "https://". For example, "https://www.valid-host.com",
 * "http://www.valid-host.com" are valid origins, but "www.valid-host.com", or
 * "\*.valid-host.com" are not valid origins. JavaScript API `getTextureStream`
 * will fail unless the requesting frame's origin URL is added to the request
 * filter.
 *
 * @exception NSInvalidArgumentException Thrown if `origin` is not a valid URL
 * origin.
 */
- (void)addAllowedOrigin:(NSString*)origin
    andAllowedInWebTexture:(BOOL)allowedInWebTexture;

/**
 * @abstract Remove added origin which was added by
 * <addAllowedOrigin:andAllowedInWebTexture:>.
 *
 * @param origin URL origin.
 *
 * @exception NSInvalidArgumentException Thrown if `origin` is not a valid URL
 * origin.
 */
- (void)removeAllowedOrigin:(NSString*)origin;

/**
 * @abstract render a video frame.
 *
 * @param ioSurface The video frame related IOSurfaceRef.
 * @param timestamp The video frame related timestamp.
 * @param error A pointer to an error object. If an error occurs, this pointer
 * is set to an actual error object containing the error information. You may
 * specify nil for this parameter if you do not want the error information.
 *
 * @discussion This API should be called after <MSWebView2TextureStreamDelegate>
 * method `textureStreamStartRequested:` event.
 *
 * @exception NSGenericException Thrown if the video request is not started.
 */
- (void)presentIOSurface:(IOSurfaceRef)ioSurface
               timestamp:(unsigned long long)timestamp
                   error:(NSError* _Nullable* _Nullable)error;

/**
 * @abstract Stop streaming of the current stream id.
 *
 * @discussion Stop method will trigger <MSWebView2TextureStreamDelegate> method
 * `textureStreamStopped:` event. After stop streaming,
 * `presentIOSurface:timestamp:error:` will failed. The Javascript can restart
 * the stream with `getTextureStream()`.
 */
- (void)stop;

/**
 * @abstract Notifies renderer that the IOSurface can be reused.
 *
 * @param ioSurface the Javascript video frame buffer.
 * @discussion The IOSurface is from <MSWebView2TextureStreamDelegate> method
 *  `webTextureStream:IOSurfaceReceived:timestamp:`. After consume the IOSurface
 * call this API to let Javascript reuse the IOSurface.
 *
 * @exception NSGenericException Thrown if the video request is not started.
 */
- (void)freeWebTextureIOSurface:(IOSurfaceRef)ioSurface;

/**
 * @abstract The texture stream related streamId.
 */
@property(nonatomic, copy, readonly) NSString* streamID;

/**
 * @abstract The texture stream delegate.
 */
@property(nonatomic, weak) id<MSWebView2TextureStreamDelegate> delegate;

@end

NS_ASSUME_NONNULL_END
