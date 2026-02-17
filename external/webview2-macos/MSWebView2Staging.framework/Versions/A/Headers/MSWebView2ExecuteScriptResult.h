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
 * This interface represents a JavaScript exception.
 * If executeScriptWithResult property `succeeded` is NO,
 * you can use the result's Exception property to get the script exception.
 */
@interface MSWebView2ScriptException : NSObject

/**
 * @abstract The line number of the source where the exception occurred.
 *
 * @discussion In the JSON it is `exceptionDetail.lineNumber`.
 * Note that this position starts at 0.
 */
@property(nonatomic, readonly) NSUInteger lineNumber;

/**
 * @abstract The column number of the source where the exception occurred.
 *
 * @discussion In the JSON it is `exceptionDetail.columnNumber`.
 * Note that this position starts at 0.
 */
@property(nonatomic, readonly) NSUInteger columnNumber;

/**
 * @abstract The Name is the exception's class name.
 *
 * @discussion In the JSON it is `exceptionDetail.exception.className`.
 * This is the empty string if the exception doesn't have a class name.
 * This can happen if the script throws a non-Error object such as `throw
 * "abc";`
 */
@property(nonatomic, readonly, copy) NSString* name;

/**
 * @abstract The Message is the exception's message and potentially stack.
 *
 * @discussion In the JSON it is exceptionDetail.exception.description.
 * This is the empty string if the exception doesn't have a description.
 * This can happen if the script throws a non-Error object such as throw "abc";.
 */
@property(nonatomic, readonly, copy) NSString* message;

/**
 * @abstract This will return all details of the exception as a JSON string.
 *
 * @discussion In the case that script has thrown a non-Error object such as
 * `throw "abc";` or any other non-Error object, you can get object specific
 * properties.
 */
- (nullable NSString*)toJson;

@end

/**
 * This is the result for executeScriptWithResult.
 */
@interface MSWebView2ExecuteScriptResult : NSObject

/**
 * @abstract This property is true if executeScriptWithResult successfully
 * executed script with no unhandled exceptions and the result is available in
 * the resultAsJson property or via the tryGetResultAsString method.
 *
 * @discussion If it is NO then the script execution had an unhandled exception
 * which you can get via the exception property.
 */
@property(nonatomic, readonly) BOOL succeeded;

/**
 * @abstract A property that has no explicit return value returns undefined.
 *
 * @discussion If the script that was run throws an unhandled exception, then
 * the result is also "null". This method is applied asynchronously.
 * If the method is run before `ContentLoading`, the script will not be executed
 * and the string "null" will be returned.
 */
@property(nonatomic, readonly, copy) NSString* resultAsJson;

/**
 * @abstract If succeeded is NO, you can use this property to get the
 * unhandled exception thrown by script execution, otherwise it will return
 * 'nil'.
 */
@property(nullable, nonatomic, readonly, strong)
    MSWebView2ScriptException* exception;

/**
 * @abstract If succeeded is YES and the result of script execution is a
 * string.
 *
 * @return This method provides the value of the string result, and we will get
 * the `nil` var value when the js result is not string type.
 *
 * @discussion The difference with `resultAsJson` is that when result is
 * string type, the result returned by this method will have quotes less
 * than `resultAsJson`.
 */
- (nullable NSString*)tryGetResultAsString;

@end

NS_ASSUME_NONNULL_END
