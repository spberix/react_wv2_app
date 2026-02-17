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

#import "MSWebView2Foundation.h"

NS_ASSUME_NONNULL_BEGIN

/**
 * Allowed port range scope for port configuration.
 */
typedef NS_ENUM(NSInteger, MSWebView2AllowedPortRangeScope) {
  /**
   * Default - applies to all network components.
   */
  MSWebView2AllowedPortRangeScopeDefault = 0,

  /**
   * Applies to WebRTC network component only.
   */
  MSWebView2AllowedPortRangeScopeWebRTC = 1,
};

/**
 * Transport protocol kind for port configuration.
 */
typedef NS_ENUM(NSInteger, MSWebView2TransportProtocolKind) {
  /**
   * User Datagram Protocol.
   */
  MSWebView2TransportProtocolKindUDP = 0,

};

/**
 * Represents a port range configuration for network components.
 */
MS_WEBVIEW2_EXTERN
@interface MSWebView2PortRange : NSObject

/**
 * The minimum port number in the range.
 * Valid range: 1025-65535, or 0 to indicate unset.
 */
@property(nonatomic, readonly) NSInteger minPort;

/**
 * The maximum port number in the range.
 * Valid range: 1025-65535, or 0 to indicate unset.
 */
@property(nonatomic, readonly) NSInteger maxPort;

/**
 * Creates a port range with the specified minimum and maximum ports.
 *
 * @param minPort Minimum port (1025-65535) or 0 for unset.
 * @param maxPort Maximum port (1025-65535) or 0 for unset.
 * @return A new port range instance.
 */
- (instancetype)initWithMinPort:(NSInteger)minPort maxPort:(NSInteger)maxPort;

@end

/**
 * Configuration of WebView environment.
 */
MS_WEBVIEW2_EXTERN
@interface MSWebView2EnvironmentOptions : NSObject <NSCopying>

/**
 @abstract Determines whether to enable single sign on with Azure Active
 Directory (AAD) resources inside WebView using the logged in Company Portal
 account.

 @discussion This controls the
 `--enable-features=msSingleSignOnOSPrimaryAccountIsShared` command line switch
 passed to the browser process. Use the `allowSingleSignOnUsingOSPrimaryAccount`
 property to enable single sign on with the Company Portal account. This
 property defaults to `NO`.
 */
@property(nonatomic) BOOL allowSingleSignOnUsingOSPrimaryAccount;

/**
 * @abstract Additional browser arguments.
 */
@property(nullable, nonatomic, copy)
    NSArray<NSString*>* additionalBrowserArguments;

/**
 * @abstract Set the `Accept-Language` request header carried by WebView when
 * sending requests.
 *
 * @discussion The default language applies to the `Accept-Language` HTTP
 * header that WebView sends to websites. This option is ignored if language is
 * set in `additionalBrowserArguments`. The intended locale value is in the
 * format of BCP 47 Language Tags. More information can be found from [IETF
 * BCP47](https://www.ietf.org/rfc/bcp/bcp47.html).
 */
@property(nullable, nonatomic, copy) NSString* language;

/**
 * Sets the allowed port range restriction for the specified scope and protocol.
 *
 * @param portRange The port range to set.
 * @param scope The network component scope.
 * @param protocol The transport protocol.
 *
 * @discussion
 * This API enables WebView2 to operate within enterprise network or firewall
 * restrictions by limiting network communication to a defined port range.
 * It provides fine-grained control by allowing port restrictions to be applied
 * per network component scope, such as WebRTC.
 *
 * Currently, only WebRTC UDP Port Range restriction is supported.
 *
 * `portRange.minPort` and `portRange.maxPort` must be within the range
 * 1025-65535 (inclusive), or must both be the sentinel value 0. `minPort` must
 * be less than or equal to `maxPort`. If `minPort` equals `maxPort`, the range
 * represents a single port.
 *
 * A component-specific scope (e.g., WebRTC) always takes precedence over
 * Default. Default defines the port range restrictions for all components
 * without specific overrides. Passing {0, 0} for a component scope unsets its
 * specific range restriction and inherits from Default. If Default is set and a
 * specific scope is unset, that component inherits Default. Passing {1025,
 * 65535} for a component scope makes the port range unrestricted for that
 * component.
 *
 * | Scope State                    | Behaviour                            |
 * | ------------------------------ | ------------------------------------ |
 * | Only Default is set            | Default applies port range           |
 * |                                | restrictions to all components       |
 * | ------------------------------ | ------------------------------------ |
 * | Default and WebRTC are both    | WebRTC port range restrictions       |
 * | set                            | apply to WebRTC; Default applies     |
 * |                                | to others                            |
 * | ------------------------------ | ------------------------------------ |
 * | WebRTC only is set             | WebRTC applies port range            |
 * |                                | restrictions only to WebRTC;         |
 * |                                | others unrestricted                  |
 * | ------------------------------ | ------------------------------------ |
 * | Default set and WebRTC reset   | Default applies port range           |
 * | to {0,0}                       | restrictions to all and WebRTC       |
 * |                                | inherits Default                     |
 * | ------------------------------ | ------------------------------------ |
 * | Default set and WebRTC set to  | Default applies port range           |
 * | {1025,65535}                   | restrictions to all except WebRTC    |
 * |                                | which is unrestricted                |
 *
 * @throws NSInvalidArgumentException if parameters are invalid.
 */
- (void)setAllowedPortRange:(MSWebView2PortRange*)portRange
                   forScope:(MSWebView2AllowedPortRangeScope)scope
                   protocol:(MSWebView2TransportProtocolKind)protocol;

/**
 * Retrieves the effective allowed port range for the specified component scope
 * and protocol.
 *
 * @param scope The network component scope to query.
 * @param protocol The transport protocol to query.
 * @return The effective port range, or a range with minPort=0 and maxPort=0 if
 * none is configured.
 *
 * @discussion
 * Returns the effective port range previously set via
 * `setAllowedPortRange:forScope:protocol:`.
 *
 * Returns the port range to use for the specified
 * scope. If the scope is Default or if the specified scope is unset, then the
 * Default port range is returned. A range of {0, 0} means that no port range
 * has been set. Querying Default only returns Default; it does not aggregate
 * component-specific settings. If neither Default nor a component-specific
 * scope is set, the default {0, 0} (unset) is returned.
 *
 * | GetEffectiveAllowedPortRange Scope query         | Returned Range       |
 * | ------------------------------------------------ | -------------------- |
 * | Pass WebRTC when only Default is set             | Returns Default range|
 * | Pass WebRTC when WebRTC explicitly set           | Returns WebRTC range |
 * | Pass WebRTC when Default unset and WebRTC unset  | Returns {0, 0}       |
 * | Pass WebRTC when Default set and WebRTC reset to {0, 0} | Returns Default |
 * | Pass Default when only WebRTC set                | Returns {0, 0}       |
 *
 * @throws NSInvalidArgumentException if parameters are invalid.
 */
- (MSWebView2PortRange*)
    getEffectiveAllowedPortRangeForScope:(MSWebView2AllowedPortRangeScope)scope
                                protocol:
                                    (MSWebView2TransportProtocolKind)protocol;

@end

NS_ASSUME_NONNULL_END
