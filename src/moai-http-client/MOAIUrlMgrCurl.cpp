// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-http-client/MOAIHttpTaskCurl.h>
#include <moai-http-client/MOAIUrlMgrCurl.h>
#include <moai-http-client/MOAIHttpTaskCurlThread.h>

SUPPRESS_EMPTY_FILE_WARNING
#if MOAI_WITH_LIBCURL

//================================================================//
// MOAIUrlMgrCurl
//================================================================//

//----------------------------------------------------------------//
void MOAIUrlMgrCurl::AddHandle ( MOAIHttpTaskCurl& task ) {

	CURL* handle = task.mEasyHandle;
	if ( !handle ) return;
	
	task.LatchRetain ();
	
	mTasks.push_back(&task);
	
	MOAIHttpTaskCurlThread* thread = MOAIHttpTaskCurlThread::getInstance();
	thread->setParams(&task);
	thread->start();
//	MOAIHttpTaskCurlThread::deleteInstance();
}

//----------------------------------------------------------------//
MOAIUrlMgrCurl::MOAIUrlMgrCurl () {
}

//----------------------------------------------------------------//
MOAIUrlMgrCurl::~MOAIUrlMgrCurl () {
}

//----------------------------------------------------------------//
void MOAIUrlMgrCurl::Process () {
	if (mTasks.size() == 0)
		return;
	
	STLArray < MOAIHttpTaskCurl*>::iterator it = mTasks.begin();
	while(it != mTasks.end() && mTasks.size() > 0) {
		MOAIHttpTaskCurl *task = *it;
		if (!task->inProgress()) {
			task->CurlFinish();
			task->LatchRelease();
			mTasks.erase(it);
		} else {
			it++;
		}
	}
	
/*
	STLMap < CURL*, MOAIHttpTaskCurl* >& handleMap = this->mHandleMap;
	CURLM* multiHandle = this->mMultiHandle;

	if ( !this->mMore ) return;
	this->mMore = false;

	if ( !multiHandle ) return;
	
	// pump the multi handle
	int stillRunning;
	while ( CURLM_CALL_MULTI_PERFORM == curl_multi_perform ( multiHandle, &stillRunning ));

	int msgsInQueue;
	CURLMsg* msg;
	do {
		
		msg = curl_multi_info_read ( multiHandle, &msgsInQueue );
	
		if ( msg && ( msg->msg == CURLMSG_DONE )) {
		
			CURL* handle = msg->easy_handle;
			if ( handleMap.contains ( handle )) {
				MOAIHttpTaskCurl* task = handleMap [ handle ];
				handleMap.erase ( handle );
				
				task->CurlFinish ();
				task->LatchRelease ();
			}
		}
	}
	while ( msg );

	// bail if nothing left running
	if ( !stillRunning ) return;
	
	this->mMore = true;
*/
 }

#endif