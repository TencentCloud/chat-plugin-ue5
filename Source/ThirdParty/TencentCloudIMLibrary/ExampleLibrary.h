#if defined _WIN32 || defined _WIN64
#define EXAMPLELIBRARY_IMPORT __declspec(dllimport)
#elif defined __linux__
#define EXAMPLELIBRARY_IMPORT __attribute__((visibility("default")))
#else
#define EXAMPLELIBRARY_IMPORT
#endif

#include "V2TIMManager.h"
#include "V2TIMMessageManager.h"
#include "V2TIMFriendshipManager.h"
#include "V2TIMConversationManager.h"
#include "V2TIMGroupManager.h"
#include "V2TIMOfflinePushManager.h"
#include "V2TIMSignalingManager.h"

EXAMPLELIBRARY_IMPORT void ExampleLibraryFunction();
