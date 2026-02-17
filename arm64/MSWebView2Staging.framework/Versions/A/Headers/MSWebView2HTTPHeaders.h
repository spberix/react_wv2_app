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

#import "MSWebView2HTTPHeader.h"

NS_ASSUME_NONNULL_BEGIN

/**
 * Represent HTTP request and response headers.
 */
@interface MSWebView2HTTPHeaders : NSObject

/**
 * @abstract All HTTP header fields using an array of <MSWebView2HTTPHeader>.
 */
@property(nonatomic, copy, readonly) NSArray<MSWebView2HTTPHeader*>* allHeaders;

/**
 * @abstract Gets all header values matching the name.
 *
 * @param name The name of the header field.
 *
 * @return Returns an array of <MSWebView2HTTPHeaders> matching the name. If
 * `name` does not exist, the returned array is empty.
 */
- (NSArray<MSWebView2HTTPHeader*>*)getHeaders:(NSString*)name;

/**
 * @abstract Gets the header value matching the name.
 *
 * @param name The name of the header field.
 *
 * @return Header value matching the name.
 *
 * @discussion Returns the first header value if there are multiple header
 * values with the same name. If `name` does not exist, returns `nil`.
 */
- (nullable NSString*)getHeader:(NSString*)name;

/**
 * @abstract Removes all header values matching the name.
 *
 * @param name The name of the header field.
 *
 * @discussion Does nothing if `name` does not exist.
 */
- (void)removeHeaders:(NSString*)name;

/**
 * @abstract Verifies that the headers contain an entry that matches the header
 * name.
 *
 * @param name The name of the header field.
 *
 * @return Returns `YES` if contained, or returns `NO`.
 */
- (BOOL)contains:(NSString*)name;

/**
 * @abstract Adds or updates header that matches the name.
 *
 * @param name The name of the header field.
 * @param value The value of the header field.
 *
 * @discussion Updates header value if `name` already exists, or add a new
 * header field.
 *
 * The difference between `setHeader:withValue:` and <appendHeader:withValue:>
 * is when `name` already exists in <allHeaders>, `setHeader:withValue:` remove
 * the existing header fields and then appends a new value to the end of header
 * list. This is the same as calling `removeHeaders:` followed
 * `appendHeader:withValue:`.
 *
 * If the `value` is `nil`, it will call `removeHeaders:` to delete headers
 * matching the `name`.
 *
 * @exception NSInvalidArgumentException Thrown if `name` or `value` is invalid.
 */
- (void)setHeader:(NSString*)name withValue:(nullable NSString*)value;

/**
 * @abstract Appends header line with name and value.
 *
 * @param name The name of the header field.
 * @param value The value of the header field.
 *
 * @exception NSInvalidArgumentException Thrown if `name` or `value` is invalid.
 */
- (void)appendHeader:(NSString*)name withValue:(NSString*)value;

@end

NS_ASSUME_NONNULL_END
