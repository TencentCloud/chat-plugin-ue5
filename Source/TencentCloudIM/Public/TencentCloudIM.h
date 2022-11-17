// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"
#include "V2TIMBuffer.h"
#include "V2TIMCallback.h"
#include "V2TIMCommon.h"
#include "V2TIMConversation.h"
#include "V2TIMConversationManager.h"
#include "V2TIMDefine.h"
#include "V2TIMErrorCode.h"
#include "V2TIMFriendship.h"
#include "V2TIMFriendshipManager.h"
#include "V2TIMGroup.h"
#include "V2TIMGroupManager.h"
#include "V2TIMListener.h"
#include "V2TIMManager.h"
#include "V2TIMMessage.h"
#include "V2TIMMessageManager.h"
#include "V2TIMOfflinePushManager.h"
#include "V2TIMSignalingManager.h"
#include "V2TIMSignaling.h"
#include "V2TIMString.h"

class FTencentCloudIMModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	/** Handle to the test dll we will load */
	void*	ExampleLibraryHandle;
};
