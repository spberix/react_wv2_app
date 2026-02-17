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

#import "MSWebView2Foundation.h"

NS_ASSUME_NONNULL_BEGIN

@class MSWebView2TextureStream;

/**
 * MSWebViewErrorDomain Indicates a texture stream error.
 */
MS_WEBVIEW2_EXTERN NSString* const MSWebView2TextureStreamErrorDomain;

/**
 * Constants used by NSError to indicate errors in the
 * MSWebView2TextureStreamErrorDomain.
 */
typedef NS_ENUM(NSInteger, MSWebView2TextureStreamErrorCode) {
  /**
   * Indicates that create a <MSWebView2TextrueStream> instance more than once
   * for a specific stream id.
   */
  MSWebView2TextureStreamErrorStreamIDAlreadyRegistered,

  /**
   * Indicates that call <MSWebView2TextureStream> method
   * `presentIOSurface:timestamp:error:` before `textureStreamStarted:` event or
   * after `textureStreamStopped:`. Or, 10 seconds passed before calling
   * <MSWebView2TextureStream> method `presentIOSurface:timestamp:error:` since
   * `textureStreamStartRequested:` event.
   */
  MSWebView2TextureStreamErrorNoVideoTrackRequested,

  /**
   * Indicates that the buffer presented is alreay in use for rendering.
   */
  MSWebView2TextureStreamErrorBufferInUse,

  /**
   * Indicates that the IOSurface data format is not NV12.
   */
  MSWebView2TextureStreamErrorVideoFormatNotSupported,
};

/**
 * Delegate for the texture stream.
 */
@protocol MSWebView2TextureStreamDelegate <NSObject>

@optional

/**
 * @abstract Tells the delegate that the texture stream has start requested.
 *
 * @param stream The texture stream instance.
 */
- (void)textureStreamStartRequested:(MSWebView2TextureStream*)stream;

/**
 * @abstract Tells the delegate that the texure stream has stop requested.
 *
 * @param stream The texture stream instance.
 * @discussion <MSWebView2TextureStream> method `stop` and
 * Javascript MediaTrack `stop()` can trigger this event.
 */
- (void)textureStreamStopped:(MSWebView2TextureStream*)stream;

/**
 * @abstract Tells the delegate that there is an error when presenting.
 *
 * @param stream The texture stream instance.
 * @param ioSurface The related ioSurface when error happens.
 * @param error The detail error information.
 */
- (void)textureStream:(MSWebView2TextureStream*)stream
            IOSurface:(IOSurfaceRef)ioSurface
        errorReceived:(NSError*)error;

/**
 * @abstract Tells the delegate that the IOSurface is released by renderer.
 *
 * @param stream The texture stream instance.
 * @param ioSurface The presented IOSurface which can be reused again.
 *
 * @discussion After this event, the IOSurface can be reused to present another
 * texture. If you are using IOSurface with CVPixelBufferPool, CVPixelBufferPool
 * will take care of reuse IOSurface, you can ignore this event.
 */
- (void)textureStream:(MSWebView2TextureStream*)stream
       IOSurfaceFreed:(IOSurfaceRef)ioSurface;

/**
 * @abstract Tells the delegate that received texture by Javascript.
 *
 * @param stream The texture stream instance.
 * @param ioSurface The renderer video frame buffer.
 * @param timestamp Timestamp of the web texture.
 *
 * @discussion There is no started event for web texture. Whenever texture is
 * sent from the Javascript, this method is triggered.
 *
 * Javascript can set timestamp with any value, but it is suggested to use same
 * value of its original video frame that is a parameter of
 * <MSWebView2TextrueStream> method `presentIOSurface:timestamp:error:` so that
 * the host is able to tell the receiving texture delta.
 *
 *  After consuming the IOSurface, should call <MSWebView2TextureStream> method
 *   `freeWebTextureIOSurface:` to let Javascript reuse IOSurface.
 */
- (void)webTextureStream:(MSWebView2TextureStream*)stream
       IOSurfaceReceived:(IOSurfaceRef)ioSurface
               timestamp:(unsigned long long)timestamp;

/**
 * @abstract Tells the delegate that web texture has stopped.
 * @param stream The texture stream instance.
 *
 * @discussion Javascript `unregisterTextureStream()` will trigger this event.
 */
- (void)webTextureStreamStopped:(MSWebView2TextureStream*)stream;

@end

NS_ASSUME_NONNULL_END
