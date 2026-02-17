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
 * Indicates the kind of context for which the context menu was
 * created. This enum will always represent the active element that caused the
 * context menu request. If there is a selection with multiple images, audio and
 * text, for example, the element that the end user right clicks on within this
 * selection will be the option represented by this enum.
 */
typedef NS_ENUM(NSInteger, MSWebView2ContextMenuTargetKind) {
  /**
   * Indicates that the context menu was created for the page without any
   * additional content.
   */
  MSWebView2ContextMenuTargetKindPage = 0,
  /**
   * Indicates that the context menu was created for an image element.
   */
  MSWebView2ContextMenuTargetKindImage,
  /**
   * Indicates that the context menu was created for selected text.
   */
  MSWebView2ContextMenuTargetKindSelectedText,
  /**
   * Indicates that the context menu was created for an audio element.
   */
  MSWebView2ContextMenuTargetKindAudio,
  /**
   * Indicates that the context menu was created for a video element.
   */
  MSWebView2ContextMenuTargetKindVideo
};

NS_ASSUME_NONNULL_BEGIN

/**
 * Info provided when a context menu is requested.
 */
@interface MSWebView2ContextMenuTarget : NSObject

/**
 * @abstract Indicate if the context menu was requested on the main frame and is
 * `NO` if invoked on another frame.
 */
@property(nonatomic, readonly) BOOL isRequestedForMainFrame;

/**
 * @abstract Indicate if the context menu is requested on a selection.
 */
@property(nonatomic, readonly) BOOL hasSelection;

/**
 * @abstract Indicate if the context menu is requested on an editable component.
 */
@property(nonatomic, readonly) BOOL editable;

/**
 * @abstract Indicate if the context menu is requested on an text element that
 * contains an anchor tag.
 */
@property(nonatomic, readonly) BOOL hasLinkText;

/**
 * @abstract Indicate if the context menu is requested on HTML containing an
 * anchor tag.
 */
@property(nonatomic, readonly) BOOL hasLinkURI;

/**
 * @abstract The page uri of the frame.
 */
@property(nonatomic, readonly, copy) NSString* pageURI;

/**
 * @abstract The uri of the frame. Will match the pageURI if
 * <isRequestedForMainFrame> is `YES`
 */
@property(nonatomic, readonly, copy) NSString* frameURI;

/**
 * @abstract The active source uri of element.
 */
@property(nonatomic, readonly, copy) NSString* sourceURI;

/**
 * @abstract URI of the link (if <hasLinkURI> is `YES`, otherwise empty
 * string).
 */
@property(nonatomic, readonly, copy) NSString* linkURI;

/**
 * @abstract Text of the link (if <hasLinkText> is `YES`, nil otherwise)
 */
@property(nonatomic, readonly, copy) NSString* linkText;

/**
 * @abstract The selected text (if <hasSelection> is `YES`, nil otherwise).
 */
@property(nonatomic, readonly, copy) NSString* selectionText;

/**
 * @abstract The kind of context that the user selected.
 */
@property(nonatomic, readonly) MSWebView2ContextMenuTargetKind kind;

@end

NS_ASSUME_NONNULL_END
