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
 * Specifies <MSWebView2ContextMenuItem> kind
 */
typedef NS_ENUM(NSInteger, MSWebView2ContextMenuItemKind) {
  /**
   * Specifies a command menu item kind.
   */
  MSWebView2ContextMenuItemKindCommand = 0,
  /**
   * Specifies a check box menu item kind. `MSWebView2ContextMenuItem` objects
   * of this kind will need the `isChecked` property to determine current state
   * of the check box
   */
  MSWebView2ContextMenuItemKindCheckbox,
  /**
   * Specifies a radio button menu item kind. `MSWebView2ContextMenuItem`
   * objects of this kind will need the `isChecked` property to determine
   * current state of the radio button.
   */
  MSWebView2ContextMenuItemKindRadio,
  /**
   * Specifies a separator menu item kind. `MSWebView2ContextMenuItem` objects
   * of this kind are used to signal a visual separator with no functionality.
   */
  MSWebView2ContextMenuItemKindSeparator,
  /**
   * Specifies a sub menu menu item kind. `MSWebView2ContextMenuItem` objects of
   * this kind will contain a children which contains an array of
   * `MSWebView2ContextMenuItem` objects.
   */
  MSWebView2ContextMenuItemKindSubmenu,
};

NS_ASSUME_NONNULL_BEGIN

/**
 * Indicate a context menu item object.
 */
@interface MSWebView2ContextMenuItem : NSObject

/**
 * @abstract Use this to report the
 * `selectedCommandID` in `ContextMenuRequested` event.
 */
@property(nonatomic, readonly) int32_t commandID;

/**
 * @abstract Indicate if the menu item is enabled.
 */
@property(nonatomic) BOOL enabled;

/**
 * @abstract Must only be used for custom context
 * menu items that has a <MSWebView2ContextMenuItemKind> kind like
 * `MSWebView2ContextMenuItemKindCheckbox` or
 * `MSWebView2ContextMenuItemKindRadio`.
 */
@property(nonatomic) BOOL checked;

/**
 * @abstract The array of children menu items if the kind is Submenu.
 * If the kind is not submenu, it will be nil.
 */
@property(nonatomic, nullable, copy)
    NSArray<MSWebView2ContextMenuItem*>* children;

/**
 * @abstract The <MSWebView2ContextMenuItem> kind.
 */
@property(nonatomic, readonly) MSWebView2ContextMenuItemKind kind;

/**
 * @abstract The icon for the `MSWebView2ContextMenuItem` in the form of
 * NSImage.
 */
@property(nonatomic, readonly, strong, nullable) NSImage* icon;

/**
 * @abstract The unlocalized name for the `MSWebView2ContextMenuItem`.
 *
 * @discussion Use this to distinguish between context menu item types. This
 * will be the English label of the menu item in lower camel case. For example,
 * the "Save as" menu item will be "saveAs". Extension menu items will be
 * "extension", custom menu items will be "custom" and spellcheck items will be
 * "spellCheck".
 * Some example context menu item names are:
 * - "saveAs"
 * - "copyImage"
 * - "openLinkInNewWindow"
 * - "cut"
 * - "copy"
 * - "paste"
 */
@property(nonatomic, readonly, copy) NSString* name;

/**
 * @abstract The localized label for the `MSWebView2ContextMenuItem`. Will
 * contain an ampersand for characters to be used as keyboard accelerator.
 */
@property(nonatomic, readonly, copy) NSString* label;

/**
 * @abstract The localized keyboard shortcut for this
 * `MSWebView2ContextMenuItem`
 *
 * @discussion The localized keyboard shortcut for this
 * `MSWebView2ContextMenuItem`. It will be an empty string if there is no
 * keyboard shortcut. The text is intended to be displayed to the end user to
 * show the keyboard shortcut. For example `Command+Option+I` for the `Inspect`.
 */
@property(nonatomic, readonly, copy, nullable) NSString* shortcut;

/**
 * @abstract The handler for this `MSWebView2ContextMenuItem` and will be
 * invoked if the item is selected.
 */
@property(nonatomic, nullable, copy) void (^customItemSelectedHandler)();

@end

NS_ASSUME_NONNULL_END
