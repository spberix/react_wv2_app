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

NS_ASSUME_NONNULL_BEGIN

/**
 * Info for the `WebMessageReceived` event.
 */
@interface MSWebView2WebMessageReceivedInfo : NSObject

/**
 * @abstract The URI of the document that sent this web message.
 */
@property(nonatomic, copy, readonly) NSString* source;

/**
 * @abstract The message posted from the WebView content to the host converted
 * to a JSON string
 *
 * @discussion Use this property to communicate using JavaScript objects.
 * For example, the following postMessage runs result in the following
 * webMessageAsJSON values:
 * @textblock
 * postMessage({'a': 'b'}) -->  @"{\"a\": \"b\"}"
 * postMessage(1.2)        -->  @"1.2"
 * postMessage(true)       -->  @"true"
 * postMessage('example')  -->  @"\"example\""
 * @/textblock
 */
@property(nonatomic, copy, readonly) NSString* webMessageAsJSON;

/**
 * @abstract Try get the web message as string.
 *
 * @return If the message posted from the WebView content to the host is a
 * string type, return the value of that string. If the message
 * posted is some other kind of JavaScript type this method returns nil
 *
 * @discussion Run this operation to communicate using simple strings.
 * For example, the following postMessage runs result in the following
 * webMessageAsString values:
 * @textblock
 * postMessage({'a': 'b'}) -->  nil
 * postMessage(1.2)        -->  nil
 * postMessage(true)       -->  nil
 * postMessage('example')  -->  @"example"
 * @/textblock
 */
- (NSString*)tryGetWebMessageAsString;

@end

NS_ASSUME_NONNULL_END
