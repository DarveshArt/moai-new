// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#import "host.h"

#import <moai-ios-motion/MOAIMotionMgrIOS.h>

//================================================================//
// aku
//================================================================//

//----------------------------------------------------------------//
void AKUIosMotionAppFinalize () {
}

//----------------------------------------------------------------//
void AKUIosMotionAppInitialize () {
}

//----------------------------------------------------------------//
void AKUIosMotionContextInitialize () {

	REGISTER_LUA_CLASS ( MOAIMotionMgrIOS )
}
