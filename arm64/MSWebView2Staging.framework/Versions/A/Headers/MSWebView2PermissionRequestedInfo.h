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

NS_ASSUME_NONNULL_BEGIN

/**
 * Indicates the kind of a permission request.
 */
typedef NS_ENUM(NSInteger, MSWebView2PermissionKind) {
  /**
   * Indicates an unknown permission.
   */
  MSWebView2PermissionKindUnknown,
  /**
   * Indicates permission to capture audio.
   */
  MSWebView2PermissionKindMicrophone,
  /**
   * Indicates permission to capture video.
   */
  MSWebView2PermissionKindCamera,
  /**
   * Indicates permission to access geolocation.
   */
  MSWebView2PermissionKindGeolocation,
  /**
   * Indicates permission to send web notifications. This permission request is
   * currently auto-rejected and no event is raised for it.
   */
  MSWebView2PermissionKindNotifications,
  /**
   * Indicates permission to access generic sensor.  Generic Sensor covering
   * ambient-light-sensor,
   */
  MSWebView2PermissionKindOtherSensors,
  /**
   * Indicates permission to read the system clipboard without a user gesture.
   */
  MSWebView2PermissionKindClipboardRead,
  /**
   * Indicates permission to automatically download multiple files. Permission
   * is requested when multiple downloads are triggered in quick succession.
   */
  MSWebView2PermissionKindMultipleAutomaticDownloads,
  /**
   * Indicates permission to read and write to files or folders on the device.
   * Permission is requested when developers use the [File System Access
   * API](https://developer.mozilla.org/en-US/docs/Web/API/File_System_Access_API)
   * to show the file or folder picker to the end user, and then request
   * "readwrite" permission for the user's selection.
   */
  MSWebView2PermissionKindFileReadWrite,
  /**
   * Indicates permission to play audio and video automatically on sites. This
   * permission affects the autoplay attribute and play method of the audio and
   * video HTML elements, and the start method of the Web Audio API. See the
   * [Autoplay guide for media and Web Audio
   * APIs](https://developer.mozilla.org/en-US/docs/Web/Media/Autoplay_guide)
   * for details.
   */
  MSWebView2PermissionKindAutoPlay,
  /**
   * Indicates permission to use fonts on the device. Permission is requested
   * when developers use the [Local Font Access
   * API](https://wicg.github.io/local-font-access/) to query the system fonts
   * available for styling web content.
   */
  MSWebView2PermissionKindLocalFonts,
  /**
   * Indicates permission to send and receive system exclusive messages to/from
   * MIDI (Musical Instrument Digital Interface) devices.
   */
  MSWebView2PermissionKindMIDISystemExclusiveMessages,
  /**
   * Indicates permission to open and place windows on the screen. Permission is
   * requested when developers use the [Multi-Screen Window Placement
   * API](https://www.w3.org/TR/window-placement/) to get screen details.
   */
  MSWebView2PermissionKindWindowManagement,
};

/**
 * Info for the MSWebView2PermissionDelegate.
 */
@interface MSWebView2PermissionRequestedInfo : NSObject

/**
 * @abstract Yes when the permission request was initiated through a user
 * gesture such as clicking an anchor tag with target.
 */
@property(nonatomic, readonly, getter=isUserInitiated) BOOL userInitiated;

/**
 * @abstract The kind of the permission that is requested.
 */
@property(nonatomic, assign, readonly) MSWebView2PermissionKind permissionKind;

/**
 * @abstract The origin of the web content that requests the permission.
 */
@property(nonatomic, copy, readonly) NSString* uri;

@end

NS_ASSUME_NONNULL_END
