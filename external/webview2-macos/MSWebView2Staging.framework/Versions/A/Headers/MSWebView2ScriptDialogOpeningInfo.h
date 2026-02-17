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
 * Indicates the kind of the JavaScript dialog type.
 */
typedef NS_ENUM(NSInteger, MSWebView2ScriptDialogKind) {
  /**
   * Indicates that the dialog uses the `window.alert` JavaScript function.
   */
  MSWebView2ScriptDialogKindAlert = 0,
  /**
   * Indicates that the dialog uses the `window.confirm` JavaScript function.
   */
  MSWebView2ScriptDialogKindConfirm,
  /**
   * Indicates that the dialog uses the `window.prompt` JavaScript function.
   */
  MSWebView2ScriptDialogKindPrompt,
  /**
   * Indicates that the dialog uses the `beforeunload` JavaScript event.
   */
  MSWebView2ScriptDialogKindBeforeUnload,
};

/**
 * Info for the `ScriptDialogOpening` event.
 */
@interface MSWebView2ScriptDialogOpeningInfo : NSObject

/**
 * @abstract The URI of the page that requested the dialog box.
 */
@property(nonatomic, copy, readonly) NSString* URI;

/**
 * @abstract The kind of JavaScript dialog box.
 */
@property(nonatomic, readonly) MSWebView2ScriptDialogKind kind;

/**
 * @abstract The message of the dialog box. From JavaScript this is the first
 * parameter passed to `alert`, `confirm`, and `prompt` and is empty for
 * `beforeunload`.
 */
@property(nonatomic, copy, readonly) NSString* message;

/**
 * @abstract The second parameter passed to the JavaScript prompt dialog. The
 * result of the prompt JavaScript function uses this value as the default
 * value.
 */
@property(nonatomic, copy, readonly) NSString* defaultText;

@end

NS_ASSUME_NONNULL_END
