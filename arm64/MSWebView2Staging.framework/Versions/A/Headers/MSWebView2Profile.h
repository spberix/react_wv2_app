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

#import "MSWebView2CookieManager.h"
#import "MSWebView2PermissionRequestedInfo.h"
#import "MSWebView2PermissionState.h"
#import "MSWebView2ProfileDelegate.h"
#import "MSWebView2ServiceWorkerManager.h"
#import "MSWebView2SharedWorkerManager.h"

NS_ASSUME_NONNULL_BEGIN

typedef NS_OPTIONS(NSUInteger, MSWebView2BrowsingDataKind) {

  /**
   * No need to clear data.
   */
  MSWebView2BrowsingDataKindNone = 0,

  /**
   * Specifies file systems data.
   */
  MSWebView2BrowsingDataKindFileSystems = 1 << 0,

  /**
   * Specifies data stored by the IndexedDB DOM feature.
   */
  MSWebView2BrowsingDataKindIndexedDB = 1 << 1,

  /**
   * Specifies data stored by the localStorage DOM API.
   */
  MSWebView2BrowsingDataKindLocalStorage = 1 << 2,

  /**
   * Specifies data stored by the Web SQL database DOM API.
   */
  MSWebView2BrowsingDataKindWebSQL = 1 << 3,

  /**
   * Specifies data stored by the CacheStorage DOM API.
   */
  MSWebView2BrowsingDataKindCacheStorage = 1 << 4,

  /**
   * Specifies DOM storage data, now and future. This browsing data kind is
   * inclusive of MSWebView2BrowsingDataKindFileSystems,
   * MSWebView2BrowsingDataKindIndexedDB,
   * MSWebView2BrowsingDataKindLocalStorage,
   * MSWebView2BrowsingDataKindWebSQL,
   * MSWebView2BrowsingDataKindServiceWorker,
   * MSWebView2BrowsingDataKindCacheStorage.
   * New DOM storage data types may be added to this data kind in the future.
   */
  MSWebView2BrowsingDataKindAllDOMStorage = 1 << 5,

  /**
   *Specifies HTTP cookies data.
   */
  MSWebView2BrowsingDataKindCookies = 1 << 6,

  /**
   * Specifies all site data, now and future. This browsing data kind
   * is inclusive of MSWebView2BrowsingDataKindAllDOMStorage and
   * MSWebView2BrowsingDataKindCookies. New site data types
   * may be added to this data kind in the future.
   */
  MSWebView2BrowsingDataKindAllSite = 1 << 7,

  /**
   * Specifies disk cache.
   */
  MSWebView2BrowsingDataKindDiskCache = 1 << 8,

  /**
   *  Specifies download history data.
   */
  MSWebView2BrowsingDataKindDownloadHistory = 1 << 9,

  /**
   * Specifies general autofill form data.
   * This excludes password information and includes information like:
   * names, street and email addresses, phone numbers, and arbitrary input.
   * This also includes payment data.
   */
  MSWebView2BrowsingDataKindGeneralAutofill = 1 << 10,

  /**
   *Specifies password autosave data.
   */
  MSWebView2BrowsingDataKindPasswordAutosave = 1 << 11,

  /**
   *Specifies browsing history data.
   */
  MSWebView2BrowsingDataKindBrowsingHistory = 1 << 12,

  /**
   *Specifies settings data.
   */
  MSWebView2BrowsingDataKindSettings = 1 << 13,

  /**
   * Specifies profile data that should be wiped to make it look like a new
   * profile. This does not delete account-scoped data like passwords but will
   * remove access to account-scoped data by signing the user out. Specifies
   * all profile data, now and future. New profile data types may be added to
   * this data kind in the future. This browsing data kind is inclusive of
   * MSWebView2BrowsingDataKindAllSite,
   * MSWebView2BrowsingDataKindDiskCache,
   * MSWebView2BrowsingDataKindDownloadHistory,
   * MSWebView2BrowsingDataKindGeneralAutofill,
   * MSWebView2BrowsingDataKindPasswordAutosave,
   * MSWebView2BrowsingDataKindBrowsingHistory, and
   * MSWebView2BrowsingDataKindSettings.
   */
  MSWebView2BrowsingDataKindAllProfile = 1 << 14,

  /**
   * Specifies service workers registered for an origin, and clear will result
   * termination and deregistration of them.
   */
  MSWebView2BrowsingDataKindServiceWorker = 1 << 15,

};

typedef NS_OPTIONS(NSInteger, MSWebView2PreferredColorScheme) {
  MSWebView2PreferredColorSchemeAuto = 0,
  MSWebView2PreferredColorSchemeLight,
  MSWebView2PreferredColorSchemeDark
};

/**
 * Provides a set of properties to configure a Profile object.
 */
@interface MSWebView2Profile : NSObject

- (instancetype)init NS_UNAVAILABLE;

/**
 * @abstract The name of profile.
 */
@property(nonatomic, copy, readonly) NSString* profileName;

/**
 * @abstract InPrivate mode is enabled or not.
 */
@property(nonatomic, readonly, getter=isInPrivateModeEnabled)
    BOOL inPrivateModeEnabled;

/**
 * @abstract Full path of profile directory.
 */
@property(nonatomic, copy, readonly) NSString* profilePath;

/**
 * @abstract The cookie manager associated with this profile instance.
 */
@property(nonatomic, readonly, strong) MSWebView2CookieManager* cookieManager;

/**
 * @abstract Get the shared worker manager to monitor shared worker creations
 * and interact with the shared workers associated with the current profile.
 */
@property(nonatomic, readonly)
    MSWebView2SharedWorkerManager* sharedWorkerManager;

/**
 * @abstract isPasswordAutosaveEnabled controls whether autosave for password
 * information is enabled.
 *
 * @discussion The `isPasswordAutosaveEnabled` property behaves
 * independently of the `isGeneralAutofillEnabled` property. When
 * `isPasswordAutosaveEnabled` is false, no new password data is saved and no
 * Save/Update Password prompts are displayed. However, if there was password
 * data already saved before disabling this setting, then that password
 * information is auto-populated, suggestions are shown and clicking on one will
 * populate the fields. When `isPasswordAutosaveEnabled` is true, password
 * information is auto-populated, suggestions are shown and clicking on one will
 * populate the fields, new data is saved, and a Save/Update Password prompt is
 * displayed. It will apply immediately after setting.
 * The default value is `NO`.
 * For the WebView2s with the same profile, their
 * `MSWebView2Profile.isPasswordAutosaveEnabled` are always in sync.
 */
@property(nonatomic, getter=isPasswordAutoSaveEnabled)
    BOOL passwordAutosaveEnabled;

/**
 * @abstract isGeneralAutofillEnabled controls whether autofill for information
 * like names, street and email addresses, phone numbers, and arbitrary input
 * is enabled.
 *
 * @discussion `isGeneralAutofillEnabled` excludes password and credit card
 * information. When `isGeneralAutofillEnabled` is false, no suggestions appear,
 * and no new information is saved. When `isGeneralAutofillEnabled` is true,
 * information is saved, suggestions appear and clicking on one will populate
 * the form fields. It will apply immediately after setting.
 * The default value is `YES`.
 * For the WebView2s with the same profile, their
 * `MSWebView2Profile.isGeneralAutofillEnabled` are always in sync.
 */
@property(nonatomic, getter=isGeneralAutofillEnabled)
    BOOL generalAutofillEnabled;

/**
 * @abstract preferredColorScheme controls the overall color scheme of the
 * WebView2s associated with this profile.
 *
 * @discussion This sets the color scheme for WebView2 UI like dialogs,
 * prompts, and context menus by setting the media feature
 * `prefers-color-scheme` for websites to respond to. The default value for this
 * is MSWebView2PreferredColorSchemeAuto, which will follow whatever theme the
 * OS is currently set to.
 */
@property(nonatomic) MSWebView2PreferredColorScheme preferredColorScheme;

/**
 * @abstract The default download path of the WebView2s associated with this
 * profile.
 *
 * @discussion The default download folder path is persisted in the user data
 * folder across sessions. The path parameter should be an absolute path to a
 * folder that the user and application can write to.
 * If the directory does not yet exist, it is created at the time of the next
 * download. If the host application does not have permission to create the
 * directory, then the user is prompted to provide a new path through the Save
 * As dialog. The user can override the default download folder path for a given
 * download by choosing a different path in the Save As dialog.
 * If `defaultDownloadFolderPath` does not have write permission or it is not an
 * absolute path, an exception will be thrown.
 *
 * @exception NSInvalidArgumentException Thrown if `defaultDownloadFolderPath`
 * does not have write permission or it is not an absolute path.
 */
@property(nonatomic, copy) NSString* defaultDownloadFolderPath;

/**
 * @abstract The profile delegate.
 */
@property(nullable, nonatomic, weak) id<MSWebView2ProfileDelegate> delegate;

/**
 * @abstract The service worker manager associated with this profile instance.
 *
 * @discussion This property is lazy-loaded. The service worker manager is
 * created when this property is first accessed.
 */
@property(nonatomic, readonly, strong)
    MSWebView2ServiceWorkerManager* serviceWorkerManager;

/**
 * @abstract Clear browsing data based on a data type.
 *
 * @param dataKinds The dataKinds being a mask of one or more
 * `MSWebView2BrowsingDataKind`. Specify the type to be cleared.
 * @param completionHandler A block to invoke when clear browsing
 * data completes or fails.
 *
 * @discussion The browsing data kinds that are supported are listed below.
 * These data kinds follow a hierarchical structure in which nested bullet
 * points are included in their parent bullet point's data kind.
 * Ex: All DOM storage is encompassed in all site data which is encompassed
 * in all profile data.
 * - All Profile
 *   - All Site Data
 *     - All DOM Storage: File Systems, Indexed DB, Local Storage, Web SQL,
 * Cache Storage
 *     - Cookies
 *   - Disk Cache
 *   - Download History
 *   - General Autofill
 *   - Password Autosave
 *   - Browsing History
 *   - Settings
 * The completed handler will be invoked when the browsing data has been cleared
 * and will indicate if the specified data was properly cleared. In the case in
 * which the operation is interrupted and the corresponding data is not fully
 * cleared. the handler will return `YES` and otherwise will return `NO`.
 * Because this is an asynchronous operation, code that is dependent on the
 * cleared data must be placed in the callback of this operation.
 * If the WebView object is closed before the clear browsing data operation
 * has completed, the handler will be released, but not invoked. In this case
 * the clear browsing data operation may or may not be completed.
 */
- (void)clearBrowsingData:(MSWebView2BrowsingDataKind)dataKinds
        completionHandler:(void (^_Nullable)(BOOL))completionHandler;

/**
 * @abstract Clears browsing data between `startTime` and `endTime` based on the
 * data type
 *
 * @param dataKinds The dataKinds being a mask of one or more
 * `MSWebView2BrowsingDataKind`. Specify the type to be cleared.
 * @param startTime The `startTime` parameter correspond to the number of
 * seconds since the UNIX epoch, the browsing data before `startTime` time will
 * not be cleared.
 * @param endTime The `endTime` parameter correspond to the number of
 * seconds since the UNIX epoch, the browsing data before `endTime` time will
 * not be cleared.
 * @param completionHandler A block to invoke when clear browsing
 * data completes or fails.
 *
 * @discussion The browsing data kinds that are supported are listed below.
 * These data kinds follow a hierarchical structure in which nested bullet
 * points are included in their parent bullet point's data kind.
 * Ex: All DOM storage is encompassed in all site data which is encompassed
 * in all profile data.
 * - All Profile
 *   - All Site Data
 *     - All DOM Storage: File Systems, Indexed DB, Local Storage, Web SQL,
 * Cache Storage
 *     - Cookies
 *   - Disk Cache
 *   - Download History
 *   - General Autofill
 *   - Password Autosave
 *   - Browsing History
 *   - Settings
 * The `startTime` and `endTime` parameters correspond to the number of
 * seconds since the UNIX epoch.
 * The completed handler will be invoked when the browsing data has been cleared
 * and will indicate if the specified data was properly cleared. In the case in
 * which the operation is interrupted and the corresponding data is not fully
 * cleared. the handler will return `YES` and otherwise will return `NO`.
 * Because this is an asynchronous operation, code that is dependent on the
 * cleared data must be placed in the callback of this operation.
 * If the WebView object is closed before the clear browsing data operation
 * has completed, the handler will be released, but not invoked. In this case
 * the clear browsing data operation may or may not be completed.
 */
- (void)clearBrowsingData:(MSWebView2BrowsingDataKind)dataKinds
                startTime:(NSDate*)startTime
                  endTime:(NSDate*)endTime
        completionHandler:(void (^_Nullable)(BOOL))completionHandler;

/**
 * @abstract Clear all browsing data
 *
 * @param completionHandler A block to invoke when clear browsing
 * data completes or fails.
 *
 * @discussion The completed handler will be invoked when the browsing data has
 * been cleared and will indicate if the specified data was properly cleared.
 * In the case in which the operation is interrupted and the corresponding data
 * is not fully cleared. the handler will return `YES` and otherwise will return
 * `NO`. Because this is an asynchronous operation, code that is dependent on
 * the cleared data must be placed in the callback of this operation.
 * If the WebView object is closed before the clear browsing data operation
 * has completed, the handler will be released, but not invoked. In this case
 * the clear browsing data operation may or may not be completed.
 */
- (void)clearAllBrowsingDataWithCompletionHandler:
    (void (^_Nullable)(BOOL))completionHandler;

/**
 * @abstract Clear data for a specific custom data partition.
 *
 * @param partitionID The partition ID to clear data for.
 * @param completionHandler A block to invoke when clear custom data
 * partition completes or fails.
 *
 * @discussion Clears all DOM storage and cookies in the custom data partition
 * identified by the `partitionID`. The handler will return `YES` if success
 * and otherwise will return `NO`.
 *
 * If no partition is found for the specified `partitionID`, the API succeeds
 * without doing anything. As DOM storage and cookies in the custom data
 * partition are also browsing data, they will also be cleared when
 * `clearBrowsingData:completionHandler:`,
 * `clearBrowsingData:startTime:endTime:completionHandler:` or
 * `clearAllBrowsingDataWithCompletionHandler:` is called and
 * the clearing condition is met.
 */
- (void)clearCustomDataPartition:(NSString*)partitionID
               completionHandler:(void (^_Nullable)(BOOL))completionHandler;

/**
 * @abstract Delete current profile.
 *
 * @discussion After the API is called, the profile will be marked for deletion.
 * The profile's local directory will be deleted at browser process exit. If it
 * fails to delete, like files are referenced by other processes, WebView
 * will try to delete the directory when future browser processes start until
 * success.
 *
 * The corresponding `MSWebView2WebView` will be closed and the event
 * `ProfileDeleted` will be raised. If a profile marked for deletion has not
 * been deleted yet, attempting to create a WebView using same profile name will
 * fail with error `MSWebView2ErrorProfileDeletePending`.
 */
- (void)deleteProfile;

/**
 * @abstract Sets permission state for the given permission kind and origin
 * asynchronously.
 *
 * @param state The permission state to set.
 * @param kind The given permission kind.
 * @param origin The origin that requests the permission.
 * @param completionHandler A block to invoke when set permission state
 * completes or fails.
 *
 * @discussion The change persists across sessions until it is changed by
 * another call to this method. Setting the state to
 * `MSWebView2PermissionStateDefault` will erase any state saved in the
 * profile and restore the default behavior.
 *
 * The origin should have a valid scheme and host (e.g.
 * "https://www.example.com"). Additional URI parts like path and fragment are
 * ignored. For example, "https://wwww.example.com/app1/index.html/" is treated
 * the same as "https://wwww.example.com". See the [MDN origin
 * definition](https://developer.mozilla.org/en-US/docs/Glossary/Origin) for
 * more details.
 *
 * After calling this API for any origin, `PermissionRequest` event will
 * continue to raise up except for:
 * 1. The permission state sets to deny for Camera and Microphone.
 * 2. The permission prompt is canceled 3x for Camera and Microphone.
 * Those are consistent with browser behavior.
 *
 * @textblock
 * [webview.profile setPermission:MSWebView2PermissionKindMicrophone
 *                         origin:@"https://www.bing.com"
 *                          state:MSWebView2PermissionStateAllow
 *              completionHandler:^(BOOL success) {
 *                if (success) {
 *                  [webview reload];
 *                }
 *              }];
 * @/textblock
 *
 * @exception NSInvalidArgumentException Thrown if `origin` is not a valid
 * scheme and host.
 */
- (void)setPermission:(MSWebView2PermissionKind)kind
               origin:(NSString*)origin
                state:(MSWebView2PermissionState)state
    completionHandler:(void (^_Nullable)(BOOL))completionHandler;

@end

NS_ASSUME_NONNULL_END
