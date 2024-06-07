// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

#include "V2TIMBuffer.h"
#include "V2TIMCallback.h"
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
#include "V2TIMSignaling.h"
#include "V2TIMSignalingManager.h"
#include "V2TIMString.h"
#include "V2TIMOfflinePushManager.h"


class TencentCloudChat : public IModuleInterface
{
private:
    void*	ImSDKHandle;
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	/**
     * 1.2 添加 SDK 监听
     */
    static void AddSDKListener(V2TIMSDKListener *listener);
	/**
     * 1.3 移除 SDK 监听
     */
    static void RemoveSDKListener(V2TIMSDKListener* listener);
	/**
     * 1.4 初始化 SDK
     *
     * @param sdkAppID 应用 ID，必填项，可以在[控制台](https://console.cloud.tencent.com/im)中获取
     * @param config   配置信息
     * @return true：成功；false：失败
     */
    static bool InitSDK(uint32_t sdkAppID, const V2TIMSDKConfig &config);
	/**
     * 1.5 反初始化 SDK
     *
     */
    static void UnInitSDK();
	/**
     * 1.6 获取 SDK 版本
     * @return 返回版本号，字符串表示，例如 5.0.10
     */
	static V2TIMString GetVersion();
	/**
     *  1.7 获取服务器当前时间
     *
     *  可用于信令离线推送场景下超时判断。
     *
     *  示例：
     *  信令邀请方：设置邀请的服务器时间 serverTime 。
     *  信令接收方：收到信令推送，根据 serverTime 判断信令是否超时 。
     *
     *  @return 服务器时间，单位 s
     */
    static int64_t GetServerTime();

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                               登录登出
    //
    /////////////////////////////////////////////////////////////////////////////////

    /**
     * 2.1 登录
     *
     * 登录需要设置用户名 userID 和用户签名 userSig，userSig 生成请参考 [UserSig 后台
     * API](https://cloud.tencent.com/document/product/269/32688)。
     *
     * @note 请注意如下特殊逻辑:
     * - 登陆时票据过期：Login 函数的回调会返回 ERR_USER_SIG_EXPIRED：6206 错误码，此时生成新的 userSig 重新登录。
     * - 在线时票据过期：用户在线期间也可能收到 V2TIMSDKListener::OnUserSigExpired 回调，此时也是需要您生成新的 userSig 并重新登录。
     * - 在线时被踢下线：用户在线情况下被踢，SDK 会通过 V2TIMSDKListener::OnKickedOffline 回调通知给您，此时可以 UI 提示用户，并再次调用 Login() 重新登录。
     * - 同平台多设备在线：该功能为IM旗舰版功能，购买[旗舰版套餐包](https://buy.cloud.tencent.com/avc?from=17487)后可使用，详见[价格说明](https://cloud.tencent.com/document/product/269/11673?from=17224#.E5.9F.BA.E7.A1.80.E6.9C.8D.E5.8A.A1.E8.AF.A6.E6.83.85)。
     */
    static void Login(const V2TIMString &userID, const V2TIMString &userSig,
                       V2TIMCallback *callback);

    /**
     *
     * 2.2 登出
     *
     * 退出登录，如果切换账号，需要 Logout 回调成功或者失败后才能再次 Login，否则 Login 可能会失败。
     */
    static void Logout(V2TIMCallback *callback);

    /**
     * 2.3 获取登录用户
     */
    static V2TIMString GetLoginUser();

    /**
     * 2.4 获取登录状态
     *
     * 如果用户已经处于已登录和登录中状态，请勿再频繁调用登录接口登录。
     *
     * @return 登录状态
     */
    static V2TIMLoginStatus GetLoginStatus();

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                         消息收发
    //
    /////////////////////////////////////////////////////////////////////////////////

    /**
     * 3.1 设置基本消息（文本消息和自定义消息）的事件监听器
     *
     * @note 图片消息、视频消息、语音消息等高级消息的监听，请参考: @ref V2TIMMessageManager::AddAdvancedMsgListener() 接口。
     *
     */
    static void AddSimpleMsgListener(V2TIMSimpleMsgListener *listener);

    /**
     * 3.2 移除基本消息（文本消息和自定义消息）的事件监听器
     */
    static void RemoveSimpleMsgListener(V2TIMSimpleMsgListener *listener);

    /**
     * 3.3 发送单聊普通文本消息（最大支持 12KB）
     *
     * 文本消息支持云端的脏词过滤，如果用户发送的消息中有敏感词，callback 回调将会返回 ERR_SVR_COMM_SENSITIVE_TEXT(80001) 错误码。
     *
     * @return 返回消息的唯一标识 ID
     */
    static V2TIMString SendC2CTextMessage(const V2TIMString &text, const V2TIMString &userID,
                                           V2TIMSendCallback *callback);

    /**
     * 3.4 发送单聊自定义（信令）消息（最大支持 12KB）
     *
     * 自定义消息本质就是一端二进制
     * buffer，您可以在其上自由组织自己的消息格式（常用于发送信令），但是自定义消息不支持云端敏感词过滤。
     *
     * @return 返回消息的唯一标识 ID
     * @note 该接口发送的消息默认不会推送，如果需要推送，请调用 @ref
     * V2TIMMessageManager::SendMessage 接口。
     */
    static V2TIMString SendC2CCustomMessage(const V2TIMBuffer &customData,
                                             const V2TIMString &userID,
                                             V2TIMSendCallback *callback);

    /**
     * 3.5 发送群聊普通文本消息（最大支持 12KB）
     *
     * @param priority 设置消息的优先级，我们没有办法所有消息都能 100%
     * 送达每一个用户，但高优先级的消息会有更高的送达成功率。
     *                 - @ref
     * V2TIM_PRIORITY_HIGH：云端会优先传输，适用于在群里发送重要消息，比如主播发送的文本消息等。
     *                 - @ref
     * V2TIM_PRIORITY_NORMAL：云端按默认优先级传输，适用于在群里发送非重要消息，比如观众发送的弹幕消息等。
     * @return 返回消息的唯一标识 ID
     */
    static V2TIMString SendGroupTextMessage(const V2TIMString &text, const V2TIMString &groupID,
                                             V2TIMMessagePriority priority,
                                             V2TIMSendCallback *callback);

    /**
     * 3.6 发送群聊自定义（信令）消息（最大支持 12KB）
     *
     * @param priority   设置消息的优先级，我们没有办法所有消息都能 100%
     * 送达每一个用户，但高优先级的消息会有更高的送达成功率。
     *                   - @ref
     * V2TIM_PRIORITY_HIGH：云端会优先传输，适用于在群里发送重要信令，比如连麦邀请，PK邀请、礼物赠送等关键性信令。
     *                   - @ref
     * V2TIM_PRIORITY_NORMAL：云端按默认优先级传输，适用于在群里发送非重要信令，比如观众的点赞提醒等等。
     * @return 返回消息的唯一标识 ID
     * @note 该接口发送的消息默认不会推送，如果需要推送，请调用 @ref
     * V2TIMMessageManager::SendMessage 接口。
     */
    static V2TIMString SendGroupCustomMessage(const V2TIMBuffer &customData,
                                               const V2TIMString &groupID,
                                               V2TIMMessagePriority priority,
                                               V2TIMSendCallback *callback);

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                        群相关操作
    //
    /////////////////////////////////////////////////////////////////////////////////

    /**
     * 4.1 设置群组监听器
     */
    static void AddGroupListener(V2TIMGroupListener *listener);

    /**
     * 4.2 设置群组监听器
     */
    static void RemoveGroupListener(V2TIMGroupListener *listener);

    /**
     * 4.3 创建群组
     *
     * @param groupType
     * 群类型，我们为您预定义好了几种常用的群类型，您也可以在控制台定义自己需要的群类型：
     *  - "Work"       ：工作群，成员上限 200 人，不支持由用户主动加入，需要他人邀请入群，适合用于类似微信中随意组建的工作群（对应老版本的 Private 群）。
     *  - "Public"     ：公开群，成员上限 2000 人，任何人都可以申请加群，但加群需群主或管理员审批，适合用于类似 QQ 中由群主管理的兴趣群。
     *  - "Meeting"    ：会议群，成员上限 6000 人，任何人都可以自由进出，且加群无需被审批，适合用于视频会议和在线培训等场景（对应老版本的 ChatRoom 群）。
     *  - "Community"  ：社群，成员上限 100000 人，任何人都可以自由进出，且加群无需被审批，适合用于知识分享和游戏交流等超大社区群聊场景。5.8 版本开始支持，需要您购买旗舰版套餐。
     *  - "AVChatRoom" ：直播群，人数无上限，任何人都可以自由进出，消息吞吐量大，适合用作直播场景中的高并发弹幕聊天室。
     *
     * @param groupID   自定义群组 ID，可以传空字符串，此时系统会自动分配 groupID，并通过 callback 返回。
     *                  "Community" 类型自定义群组 ID 必须以 "@TGS#_" 作为前缀。
     * @param groupName 群名称，不能为空字符串。
     *
     * @note 请注意如下特殊逻辑:
     *  - 不支持在同一个 SDKAPPID 下创建两个相同 groupID 的群。
     *  - 社群（Community）功能仅 5.8.1668 增强版及以上版本支持，需[购买旗舰版套餐包](https://buy.cloud.tencent.com/avc?from=17213)并[申请开通](https://cloud.tencent.com/document/product/269/3916?from=17215)后方可使用。
     *  - 直播群（AVChatRoom）：在进程重启或重新登录之后，如果想继续接收直播群的消息，请您调用 joinGroup 重新加入直播群。
     */
    static void CreateGroup(const V2TIMString &groupType, const V2TIMString &groupID,
                             const V2TIMString &groupName,
                             V2TIMValueCallback<V2TIMString> *callback);

    /**
     * 4.4 加入群组
     *
     *  @note 请注意如下特殊逻辑:
     *  - 工作群（Work）：不能主动入群，只能通过群成员调用 V2TIMGroupManager::InviteUserToGroup() 接口邀请入群。
     *  - 公开群（Public）：申请入群后，需要管理员审批，管理员在收到 V2TIMGroupListener::OnReceiveJoinApplication 回调后调用
     *  V2TIMGroupManager::GetGroupApplicationList() 接口处理加群请求。
     *  - 其他群：可以直接入群。
     *  - 直播群（AVChatRoom）：在进程重启或重新登录之后，如果想继续接收直播群的消息，请您调用 joinGroup 重新加入直播群。
     *  - 直播群（AVChatRoom）：直播群新成员可以查看入群前消息，该功能为 IM 旗舰版功能，[购买旗舰版套餐包](https://buy.cloud.tencent.com/avc?from=17484)后可使用，详见[价格说明](https://cloud.tencent.com/document/product/269/11673?from=17179#.E5.9F.BA.E7.A1.80.E6.9C.8D.E5.8A.A1.E8.AF.A6.E6.83.85)
     */
    static void JoinGroup(const V2TIMString &groupID, const V2TIMString &message,
                           V2TIMCallback *callback);

    /**
     * 4.5 退出群组
     *
     * @note 在公开群（Public）、会议（Meeting）和直播群（AVChatRoom）中，群主是不可以退群的，群主只能调用 DismissGroup 解散群组。
     */
    static void QuitGroup(const V2TIMString &groupID, V2TIMCallback *callback);

    /**
     * 4.6 解散群组
     * @note 请注意如下特殊逻辑:
     *  - Work：任何人都无法解散群组。
     *  - 其他群：群主可以解散群组。
     */
    static void DismissGroup(const V2TIMString &groupID, V2TIMCallback *callback);

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                        资料相关操作
    //
    /////////////////////////////////////////////////////////////////////////////////

    /**
     * 5.1 获取用户资料
     *
     *  @note 请注意:
     *   - 获取自己的资料，传入自己的 ID 即可。
     *   - userIDList 建议一次最大 100 个，因为数量过多可能会导致数据包太大被后台拒绝，后台限制数据包最大为 1MB。
     */
    static void GetUsersInfo(const V2TIMStringVector &userIDList,
                              V2TIMValueCallback<V2TIMUserFullInfoVector> *callback);

    /**
     * 5.2 修改个人资料
     */
    static void SetSelfInfo(const V2TIMUserFullInfo &info, V2TIMCallback *callback);

    /**
     *  5.3 查询用户状态，从 6.3 版本开始支持
     *
     *  @param userIDList 需要获取的用户 ID
     *
     *  @note 请注意：
     *  - 如果您想查询自己的自定义状态，您只需要传入自己的 userID 即可
     *  - 当您批量查询时，接口只会返回查询成功的用户状态信息；当所有用户均查询失败时，接口会报错
     *  - 查询其他用户状态为 IM 旗舰版功能，[购买旗舰版套餐包](https://buy.cloud.tencent.com/avc?from=17491)后可使用，详见[价格说明](https://cloud.tencent.com/document/product/269/11673?from=17472#.E5.9F.BA.E7.A1.80.E6.9C.8D.E5.8A.A1.E8.AF.A6.E6.83.85)。
     */
    static void GetUserStatus(const V2TIMStringVector &userIDList,
                               V2TIMValueCallback<V2TIMUserStatusVector> *callback);

    /**
     *  5.4 设置自己的状态，从 6.3 版本开始支持
     *
     *  @param status 待设置的自定义状态
     *
     *  @note 请注意，该接口只支持设置自己的自定义状态，即 V2TIMUserStatus.customStatus
     */
    static void SetSelfStatus(const V2TIMUserStatus &status, V2TIMCallback *callback);

    /**
     *  5.5 订阅用户状态，从 6.3 版本开始支持
     *
     *  @param userIDList 待订阅的用户 ID
     *
     *  @note 请注意
     *   - 当成功订阅用户状态后，当对方的状态（包含在线状态、自定义状态）发生变更后，您可以监听 OnUserStatusChanged 回调来感知
     *   - 如果您需要订阅好友列表的状态，您只需要在控制台上打开开关即可，无需调用该接口
     *   - 该接口不支持订阅自己，您可以通过监听 OnUserStatusChanged 回调来感知自身的自定义状态的变更
     *   - 订阅列表有个数限制，超过限制后，会自动淘汰最先订阅的用户
     *   - 该功能为 IM 旗舰版功能，[购买旗舰版套餐包](https://buy.cloud.tencent.com/avc?from=17491)后可使用，详见[价格说明](https://cloud.tencent.com/document/product/269/11673?from=17472#.E5.9F.BA.E7.A1.80.E6.9C.8D.E5.8A.A1.E8.AF.A6.E6.83.85)。
     */
    static void SubscribeUserStatus(const V2TIMStringVector &userIDList, V2TIMCallback *callback);

    /**
     *  5.6 取消订阅用户状态，从 6.3 版本开始支持
     *
     *  @note
     *   - 当 userIDList 为空时，取消当前所有的订阅
     *   - 该功能为 IM 旗舰版功能，[购买旗舰版套餐包](https://buy.cloud.tencent.com/avc?from=17491)后可使用，详见[价格说明](https://cloud.tencent.com/document/product/269/11673?from=17472#.E5.9F.BA.E7.A1.80.E6.9C.8D.E5.8A.A1.E8.AF.A6.E6.83.85)。
     */
    static void UnsubscribeUserStatus(const V2TIMStringVector &userIDList, V2TIMCallback *callback);

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                        高级功能
    //
    /////////////////////////////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                         监听 - 高级（图片、语音、视频等）消息
    //
    /////////////////////////////////////////////////////////////////////////////////

    /**
     * 1.1 添加高级消息的事件监听器
     */
    static void AddAdvancedMsgListener(V2TIMAdvancedMsgListener *listener);

    /**
     * 1.2 移除高级消息监听器
     */
    static void RemoveAdvancedMsgListener(V2TIMAdvancedMsgListener *listener);

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                         创建 - 高级（图片、语音、视频等）消息
    //
    /////////////////////////////////////////////////////////////////////////////////

    /**
     * 2.1 创建文本消息
     */
    static V2TIMMessage CreateTextMessage(const V2TIMString &text);

    /**
     * 2.2 创建文本消息，并且可以附带 @ 提醒功能（该接口已弃用，推荐使用 CreateAtSignedGroupMessage 接口）
     *
     *  提醒消息仅适用于在群组中发送的消息
     *
     *  @param  atUserList 需要 @ 的用户列表，如果需要 @ALL，请传入 kImSDK_MesssageAtALL 常量字符串。
     *  举个例子，假设该条文本消息希望@提醒 denny 和 lucy 两个用户，同时又希望 @所有人，atUserList
     * 传 {"denny","lucy",kImSDK_MesssageAtALL}
     *
     *  @note atUserList 使用注意事项
     *  - 默认情况下，最多支持 @ 30个用户，超过限制后，消息会发送失败。
     *  - atUserList 的总数不能超过默认最大数，包括 @ALL。
     *  - 直播群（AVChatRoom）不支持发送 @ 消息。
     */
    static V2TIMMessage CreateTextAtMessage(const V2TIMString &text,
                                             const V2TIMStringVector &atUserList);

    /**
     * 2.3 创建自定义消息
     */
    static V2TIMMessage CreateCustomMessage(const V2TIMBuffer &data);

    /**
     * 2.4 创建自定义消息
     *
     * @param description 自定义消息描述信息，做离线Push时文本展示。
     * @param extension 离线 Push 时扩展字段信息。
     */
    static V2TIMMessage CreateCustomMessage(const V2TIMBuffer &data,
                                             const V2TIMString &description,
                                             const V2TIMString &extension);

    /**
     * 2.5 创建图片消息（图片最大支持 28 MB）
     */
    static V2TIMMessage CreateImageMessage(const V2TIMString &imagePath);

    /**
     * 2.6 创建语音消息（语音最大支持 28 MB）
     *
     * @param duration  语音时长，单位 s
     */
    static V2TIMMessage CreateSoundMessage(const V2TIMString &soundPath, uint32_t duration);

    /**
     * 2.7 创建视频消息（视频最大支持 100 MB）
     *
     * @param type             视频类型，如 mp4 mov 等
     * @param duration         视频时长，单位 s
     * @param snapshotPath     视频封面图片路径
     */
    static V2TIMMessage CreateVideoMessage(const V2TIMString &videoFilePath,
                                            const V2TIMString &type, uint32_t duration,
                                            const V2TIMString &snapshotPath);

    /**
     * 2.8 创建文件消息（文件最大支持 100 MB）
     */
    static V2TIMMessage CreateFileMessage(const V2TIMString &filePath,
                                           const V2TIMString &fileName);

    /**
     * 2.9 创建地理位置消息
     */
    static V2TIMMessage CreateLocationMessage(const V2TIMString &desc, double longitude,
                                               double latitude);

    /**
     * 2.10 创建表情消息
     *
     * SDK 并不提供表情包，如果开发者有表情包，可使用 index 存储表情在表情包中的索引，
     * 或者直接使用 data 存储表情二进制信息以及字符串 key，都由用户自定义，SDK 内部只做透传。
     *
     * @param index 表情索引
     * @param data  自定义数据
     */
    static V2TIMMessage CreateFaceMessage(uint32_t index, const V2TIMBuffer &data);

    /**
     * 2.11 创建合并消息（5.2.210 及以上版本支持）
     *
     * <p> 我们在收到一条合并消息的时候，通常会在聊天界面这样显示：
     * <p> |vinson 和 lynx 的聊天记录                         |        -- title         （标题）
     * <p> |vinson：新版本 SDK 计划什么时候上线呢？             |        -- abstract1 （摘要信息1）
     * <p> |lynx：计划下周一，具体时间要看下这两天的系统测试情况..|        -- abstract2 （摘要信息2）
     * <p> |vinson：好的.                                     |        -- abstract3 （摘要信息3）
     * <p> 聊天界面通常只会展示合并消息的标题和摘要信息，完整的转发消息列表，需要用户主动点击转发消息 UI  后再获取。
     *
     * <p> 多条被转发的消息可以被创建成一条合并消息 V2TIMMessage，然后调用 SendMessage 接口发送，实现步骤如下： 
     * <p> 1. 调用 CreateMergerMessage 创建一条合并消息 V2TIMMessage。
     * <p> 2. 调用 SendMessage 发送转发消息 V2TIMMessage。
     *
     * <p> 收到合并消息解析步骤：
     * <p> 1. 通过 V2TIMMessage 获取 mergerElem。
     * <p> 2. 通过 mergerElem 获取 title 和 abstractList UI 展示。
     * <p> 3. 当用户点击摘要信息 UI 的时候，调用 DownloadMessageList 接口获取转发消息列表。
     *
     *
     * @param messageList 消息列表（最大支持 300 条，消息对象必须是 V2TIM_MSG_STATUS_SEND_SUCC 状态，消息类型不能为 V2TIMGroupTipsElem）
     * @param title 合并消息的来源，比如 "vinson 和 lynx 的聊天记录"、"xxx 群聊的聊天记录"。
     * @param abstractList 合并消息的摘要列表(最大支持 5 条摘要，每条摘要的最大长度不超过 100 个字符),不同的消息类型可以设置不同的摘要信息，比如:
     * 文本消息可以设置为：sender：text，图片消息可以设置为：sender：[图片]，文件消息可以设置为：sender：[文件]。
     * @param compatibleText 合并消息兼容文本，低版本 SDK 如果不支持合并消息，默认会收到一条文本消息，文本消息的内容为 compatibleText， 
     * 该参数不能为空字符串。
     */
    static V2TIMMessage CreateMergerMessage(const V2TIMMessageVector &messageList,
                                             const V2TIMString &title,
                                             const V2TIMStringVector &abstractList,
                                             const V2TIMString &compatibleText);

    /**
     * 2.12 创建转发消息（5.2.210 及以上版本支持）
     *
     * 如果需要转发一条消息，不能直接调用 SendMessage 接口发送原消息，需要先 CreateForwardMessage
     * 创建一条转发消息再发送。
     *
     * @param message 待转发的消息对象，消息状态必须为 V2TIM_MSG_STATUS_SEND_SUCC，消息类型不能为
     * V2TIMGroupTipsElem。
     * @return 转发消息对象，elem 内容和原消息完全一致。
     */
    static V2TIMMessage CreateForwardMessage(const V2TIMMessage &message);

    /**
     *  2.13 创建定向群消息（6.0 及以上版本支持）
     *
     *  如果您需要在群内给指定群成员列表发消息，可以创建一条定向群消息，定向群消息只有指定群成员才能收到。
     *
     *  @param message 原始消息对象
     *  @param receiverList 消息接收者列表
     *  @return 定向群消息对象
     *
     * @note 请注意：
     * - 原始消息对象不支持群 @ 消息。
     * - 消息接收者列表最大支持 50 个。
     * - 社群（Community）和直播群（AVChatRoom）不支持发送定向群消息。
     * - 定向群消息默认不计入群会话的未读计数。
     */
    static V2TIMMessage CreateTargetedGroupMessage(const V2TIMMessage &message, const V2TIMStringVector &receiverList);
    
    /**
     *  2.14 创建带 @ 标记的群消息（7.0 及以上版本支持）
     *
     *  如果您需要发送的群消息附带 @ 提醒功能，可以创建一条带 @ 标记的群消息。
     *
     *  @param message 原始消息对象
     *  @param atUserList 需要 @ 的用户列表，如果需要 @ALL，请传入 kImSDK_MesssageAtALL 常量字符串。
     *  举个例子，假设该条消息希望@提醒 denny 和 lucy 两个用户，同时又希望@所有人，atUserList 传 @[@"denny",@"lucy",kImSDK_MesssageAtALL]
     *  @return 群 @ 消息对象
     *
     *  @note atUserList 使用注意事项
     *  - 默认情况下，最多支持 @ 30个用户，超过限制后，消息会发送失败。
     *  - atUserList 的总数不能超过默认最大数，包括 @ALL。
     *  - 直播群（AVChatRoom）不支持发送 @ 消息。
     */
    static V2TIMMessage CreateAtSignedGroupMessage(const V2TIMMessage &message, const V2TIMStringVector &atUserList);


    /////////////////////////////////////////////////////////////////////////////////
    //
    //                         发送 - 高级（图片、语音、视频等）消息
    //
    /////////////////////////////////////////////////////////////////////////////////

    /**
     * 3.1 发送高级消息（高级版本：可以指定优先级，推送信息等特性）
     *
     * @param message           待发送的消息对象，需要通过对应的 CreateXXXMessage 接口进行创建。
     * @param receiver          消息接收者的 userID, 如果是发送 C2C 单聊消息，只需要指定 receiver
     * 即可。
     * @param groupID           目标群组 ID，如果是发送群聊消息，只需要指定 groupID 即可。
     * @param priority
     * 消息优先级，仅针对群聊消息有效。请把重要消息设置为高优先级（比如红包、礼物消息），高频且不重要的消息设置为低优先级（比如点赞消息）。
     * @param onlineUserOnly    是否只有在线用户才能收到，如果设置为 true
     * ，接收方历史消息拉取不到，常被用于实现“对方正在输入”或群组里的非重要提示等弱提示功能，该字段不支持
     * AVChatRoom。
     * @param offlinePushInfo   离线推送时携带的标题和内容。
     * @return                  消息唯一标识
     * @note
     *  - 设置 offlinePushInfo 字段，需要先在 V2TIMOfflinePushManager
     * 开启推送，推送开启后，除了自定义消息，其他消息默认都会推送。
     *  - 如果自定义消息也需要推送，请设置 offlinePushInfo 的 desc
     * 字段，设置成功后，推送的时候会默认展示 desc 信息。
     *  - AVChatRoom 群聊不支持 onlineUserOnly 字段，如果是 AVChatRoom 请将该字段设置为 false。
     *  - 如果设置 onlineUserOnly 为 true 时，该消息为在线消息且不会被计入未读计数。
     */
    static V2TIMString SendMessage(V2TIMMessage &message, const V2TIMString &receiver,
                                    const V2TIMString &groupID, V2TIMMessagePriority priority,
                                    bool onlineUserOnly,
                                    const V2TIMOfflinePushInfo &offlinePushInfo,
                                    V2TIMSendCallback *callback);

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                         接收 - 设置消息的接口选项（接收|接收但不提醒|不接收）
    //
    /////////////////////////////////////////////////////////////////////////////////

    /**
     *  4.1 设置针对某个用户的 C2C 消息接收选项（支持批量设置）
     * <p>5.3.425 及以上版本支持
     *
     *  @note 请注意:
     *  - 该接口支持批量设置，您可以通过参数 userIDList 设置一批用户，但一次最大允许设置 30 个用户。
     *  - 该接口调用频率被限制为1秒内最多调用5次。
     *
     *
     *  @param opt    三种类型的消息接收选项：
     *                V2TIMMessage.V2TIM_RECEIVE_MESSAGE：在线正常接收消息，离线时会有厂商的离线推送通知
     *                V2TIMMessage.V2TIM_NOT_RECEIVE_MESSAGE：不会接收到消息
     *                V2TIMMessage.V2TIM_RECEIVE_NOT_NOTIFY_MESSAGE：在线正常接收消息，离线不会有推送通知
     *
     */
    static void SetC2CReceiveMessageOpt(const V2TIMStringVector &userIDList,
                                         V2TIMReceiveMessageOpt opt, V2TIMCallback *callback);

    /**
     *  4.2 查询针对某个用户的 C2C 消息接收选项
     *  <p>5.3.425 及以上版本支持
     */
    static void GetC2CReceiveMessageOpt(
        const V2TIMStringVector &userIDList,
        V2TIMValueCallback<V2TIMReceiveMessageOptInfoVector> *callback);

    /**
     *  4.3 设置群消息的接收选项
     *
     * @param opt      三种类型的消息接收选项：
     *                 V2TIMMessage.V2TIM_RECEIVE_MESSAGE：在线正常接收消息，离线时会有厂商的离线推送通知
     *                 V2TIMMessage.V2TIM_NOT_RECEIVE_MESSAGE：不会接收到群消息
     *                 V2TIMMessage.V2TIM_RECEIVE_NOT_NOTIFY_MESSAGE：在线正常接收消息，离线不会有推送通知
     */
    static void SetGroupReceiveMessageOpt(const V2TIMString &groupID, V2TIMReceiveMessageOpt opt,
                                           V2TIMCallback *callback);

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                         获取历史消息、撤回、删除、标记已读等高级接口
    //
    /////////////////////////////////////////////////////////////////////////////////

    /**
     * 5.1 获取历史消息高级接口
     *
     * @param option 拉取消息选项设置，可以设置从云端、本地拉取更老或更新的消息
     *
     * @note 请注意：
     * - 如果设置为拉取云端消息，当 SDK 检测到没有网络，默认会直接返回本地数据
     * -
     * 只有会议群（Meeting）才能拉取到进群前的历史消息，直播群（AVChatRoom）消息不存漫游和本地数据库，调用这个接口无效
     */
    static void GetHistoryMessageList(const V2TIMMessageListGetOption &option,
                                       V2TIMValueCallback<V2TIMMessageVector> *callback);

    /**
     * 5.2 撤回消息
     *
     * @note 请注意：
     *  - 撤回消息的时间限制默认 2 minutes，超过 2 minutes 的消息不能撤回，您也可以在 [控制台](https://console.cloud.tencent.com/im)（功能配置 -> 登录与消息 ->
     * 消息撤回设置）自定义撤回时间限制。
     *  - 仅支持单聊和群组中发送的普通消息，无法撤销 onlineUserOnly 为 true 即仅在线用户才能收到的消息，也无法撤销直播群（AVChatRoom）中的消息。
     *  - 如果发送方撤回消息，已经收到消息的一方会收到 V2TIMAdvancedMsgListener::OnRecvMessageRevoked 回调。
     */
    static void RevokeMessage(const V2TIMMessage &message, V2TIMCallback *callback);

    /**
     *  5.3 消息变更
     *
     *  @note 请注意：
     *  - 如果消息修改成功，自己和对端用户（C2C）或群组成员（Group）都会收到 OnRecvMessageModified 回调。
     *  - 如果在修改消息过程中，消息已经被其他人修改，callback 会返回 ERR_SDK_MSG_MODIFY_CONFLICT 错误。
     *  - 消息无论修改成功或则失败，callback 都会返回最新的消息对象。
     */
    static void ModifyMessage(const V2TIMMessage &message, V2TIMCompleteCallback<V2TIMMessage> *callback);

    /**
     * 5.4 标记单聊会话已读
     * @note 请注意：
     *  - 该接口调用成功后，自己的未读数会清 0，对端用户会收到 OnRecvC2CReadReceipt 回调，回调里面会携带标记会话已读的时间。
     *  - 从 5.8 版本开始，当 userID 为 nil 时，标记所有单聊会话为已读状态。
     */
    static void MarkC2CMessageAsRead(const V2TIMString &userID, V2TIMCallback *callback);

    /**
     * 5.5 标记群组会话已读
      *  @note 请注意：
      *  - 该接口调用成功后，自己的未读数会清 0。
      *  - 从 5.8 版本开始，当 groupID 为 nil 时，标记所有群组会话为已读状态。
     */
    static void MarkGroupMessageAsRead(const V2TIMString &groupID, V2TIMCallback *callback);

    /**
     * 5.6 标记所有会话为已读 （5.8 及其以上版本支持）
     */
    static void MarkAllMessageAsRead(V2TIMCallback *callback);

    /**
     * 5.7 删除本地及云端的消息
     *
     *  @note 该接口删除本地及云端的消息，且无法恢复。需要注意的是：
     *  - 一次最多只能删除 30 条消息
     *  - 要删除的消息必须属于同一会话
     *  - 一秒钟最多只能调用一次该接口
     *  -
     * 如果该账号在其他设备上拉取过这些消息，那么调用该接口删除后，这些消息仍然会保存在那些设备上，即删除消息不支持多端同步。
     */
    static void DeleteMessages(const V2TIMMessageVector &messages, V2TIMCallback *callback);

    /**
     * 5.8 清空单聊本地及云端的消息（不删除会话）
     * <p> 5.4.666 及以上版本支持
     *
     * @note 请注意：
     * - 会话内的消息在本地删除的同时，在服务器也会同步删除。
     *
     */
    static void ClearC2CHistoryMessage(const V2TIMString &userID, V2TIMCallback *callback);

    /**
     * 5.9 清空群聊本地及云端的消息（不删除会话）
     * <p> 5.4.666 及以上版本支持
     *
     * @note 请注意：
     * - 会话内的消息在本地删除的同时，在服务器也会同步删除。
     */
    static void ClearGroupHistoryMessage(const V2TIMString &groupID, V2TIMCallback *callback);

    /**
     * 5.10 向群组消息列表中添加一条消息
     *
     * 该接口主要用于满足向群组聊天会话中插入一些提示性消息的需求，比如“您已经退出该群”，这类消息有展示
     * 在聊天消息区的需求，但并没有发送给其他人的必要。
     * 所以 InsertGroupMessageToLocalStorage() 相当于一个被禁用了网络发送能力的 SendMessage() 接口。
     *
     * @return msgID 消息唯一标识
     *
     * @note 通过该接口 save 的消息只存本地，程序卸载后会丢失。
     */
    static V2TIMString InsertGroupMessageToLocalStorage(
        V2TIMMessage &message, const V2TIMString &groupID, const V2TIMString &sender,
        V2TIMValueCallback<V2TIMMessage> *callback);

    /**
     *  5.11 向C2C消息列表中添加一条消息
     *
     *  该接口主要用于满足向C2C聊天会话中插入一些提示性消息的需求，比如“您已成功发送消息”，这类消息有展示
     *  在聊天消息去的需求，但并没有发送给对方的必要。
     *  所以 InsertC2CMessageToLocalStorage()相当于一个被禁用了网络发送能力的 SendMessage() 接口。
     *
     *  @return msgID 消息唯一表示
     *  @note 通过该接口 save 的消息只存本地，程序卸载后会丢失。
     */
    static V2TIMString InsertC2CMessageToLocalStorage(
        V2TIMMessage &message, const V2TIMString &userID, const V2TIMString &sender,
        V2TIMValueCallback<V2TIMMessage> *callback);

    /**
     * 5.12 根据 messageID 查询指定会话中的本地消息
     * @param messageIDList 消息 ID 列表
     */
    static void FindMessages(const V2TIMStringVector &messageIDList,
                              V2TIMValueCallback<V2TIMMessageVector> *callback);

    /**
     * 5.13 搜索本地消息（5.4.666 及以上版本支持，需要您购买旗舰版套餐）
     * @param searchParam 消息搜索参数，详见 V2TIMMessageSearchParam 的定义
     * @note 该功能为 IM 旗舰版功能，[购买旗舰版套餐包](https://buy.cloud.tencent.com/avc?from=17474)后可使用，详见[价格说明](https://cloud.tencent.com/document/product/269/11673?from=17176#.E5.9F.BA.E7.A1.80.E6.9C.8D.E5.8A.A1.E8.AF.A6.E6.83.85)
     */
    static void SearchLocalMessages(const V2TIMMessageSearchParam &searchParam,
                                     V2TIMValueCallback<V2TIMMessageSearchResult> *callback);

    /**
     *  5.14 发送消息已读回执（6.1 及其以上版本支持）
     * 
     * @note 请注意：
     * - 该功能为旗舰版功能，[购买旗舰版套餐包](https://buy.cloud.tencent.com/avc?from=17485)后可使用，详见[价格说明](https://cloud.tencent.com/document/product/269/11673?from=17221#.E5.9F.BA.E7.A1.80.E6.9C.8D.E5.8A.A1.E8.AF.A6.E6.83.85)。
     * - 向群消息发送已读回执，需要您先到控制台打开对应的开关，详情参考文档 [群消息已读回执](https://cloud.tencent.com/document/product/269/75343#.E8.AE.BE.E7.BD.AE.E6.94.AF.E6.8C.81.E5.B7.B2.E8.AF.BB.E5.9B.9E.E6.89.A7.E7.9A.84.E7.BE.A4.E7.B1.BB.E5.9E.8B) 。
     * - messageList 里的消息必须在同一个会话中。
     * - 该接口调用成功后，会话未读数不会变化，消息发送者会收到 onRecvMessageReadReceipts 回调，回调里面会携带消息的最新已读信息。
     */
    static void SendMessageReadReceipts(const V2TIMMessageVector &messageList, V2TIMCallback *callback);

    /**
     *  5.15 获取消息已读回执（6.1 及其以上版本支持）
     * @param messageList 消息列表
     *
     * @note 请注意：
     * - 该功能为旗舰版功能，[购买旗舰版套餐包](https://buy.cloud.tencent.com/avc?from=17485)后可使用，详见[价格说明](https://cloud.tencent.com/document/product/269/11673?from=17221#.E5.9F.BA.E7.A1.80.E6.9C.8D.E5.8A.A1.E8.AF.A6.E6.83.85)。
     * - 获取群消息已读回执，需要您先到控制台打开对应的开关，详情参考文档 [群消息已读回执](https://cloud.tencent.com/document/product/269/75343#.E8.AE.BE.E7.BD.AE.E6.94.AF.E6.8C.81.E5.B7.B2.E8.AF.BB.E5.9B.9E.E6.89.A7.E7.9A.84.E7.BE.A4.E7.B1.BB.E5.9E.8B) 。
     * - messageList 里的消息必须在同一个会话中。
     */
    static void GetMessageReadReceipts(const V2TIMMessageVector &messageList, V2TIMValueCallback<V2TIMMessageReceiptVector> *callback);

    /**
     * 5.16 获取群消息已读群成员列表（6.1 及其以上版本支持）
     * @param message 群消息
     * @param filter  指定拉取已读或未读群成员列表。
     * @param nextSeq 分页拉取的游标，第一次默认取传 0，后续分页拉取时，传上一次分页拉取成功回调里的 nextSeq。
     * @param count   分页拉取的个数，最多支持 100 个。
     *
     * @note 请注意：
     * - 该功能为旗舰版功能，[购买旗舰版套餐包](https://buy.cloud.tencent.com/avc?from=17485)后可使用，详见[价格说明](https://cloud.tencent.com/document/product/269/11673?from=17221#.E5.9F.BA.E7.A1.80.E6.9C.8D.E5.8A.A1.E8.AF.A6.E6.83.85)。
     * - 使用该功能之前，请您先到控制台打开对应的开关，详情参考文档 [群消息已读回执](https://cloud.tencent.com/document/product/269/75343#.E8.AE.BE.E7.BD.AE.E6.94.AF.E6.8C.81.E5.B7.B2.E8.AF.BB.E5.9B.9E.E6.89.A7.E7.9A.84.E7.BE.A4.E7.B1.BB.E5.9E.8B) 。
     */
    static void GetGroupMessageReadMemberList(const V2TIMMessage &message, V2TIMGroupMessageReadMembersFilter filter, uint64_t nextSeq, uint32_t count, V2TIMValueCallback<V2TIMGroupMessageReadMemberList> *callback);

    /**
     * 5.17 设置消息扩展（6.7 及其以上版本支持，需要您购买旗舰版套餐）
     * @param message 消息对象，消息需满足三个条件：1、消息发送前需设置 supportMessageExtension 为 true，2、消息必须是发送成功的状态，3、消息不能是社群（Community）和直播群（AVChatRoom）消息。
     * @param extensions 扩展信息，如果扩展 key 已经存在，则修改扩展的 value 信息，如果扩展 key 不存在，则新增扩展。
     *
     * @note
     * - 扩展 key 最大支持 100 字节，扩展 value 最大支持 1KB，单次最多支持设置 20 个扩展，单条消息最多可设置 300 个扩展。
     * - 当多个用户同时设置或删除同一个扩展 key 时，只有第一个用户可以执行成功，其它用户会收到 23001 错误码和最新的扩展信息，在收到错误码和扩展信息后，请按需重新发起设置操作。
     * - 我们强烈建议不同的用户设置不同的扩展 key，这样大部分场景都不会冲突，比如投票、接龙、问卷调查，都可以把自己的 userID 作为扩展 key。
     */
    static void SetMessageExtensions(const V2TIMMessage &message, const V2TIMMessageExtensionVector &extensions, V2TIMValueCallback<V2TIMMessageExtensionResultVector> *callback);

    /**
     * 5.18 获取消息扩展（6.7 及其以上版本支持，需要您购买旗舰版套餐）
     */
    static void GetMessageExtensions(const V2TIMMessage &message, V2TIMValueCallback<V2TIMMessageExtensionVector> *callback);

    /**
     * 5.19 删除消息扩展（6.7 及其以上版本支持，需要您购买旗舰版套餐）
     * @param keys 消息扩展 key 列表, 单次最大支持删除 20 个消息扩展，如果设置为 nil ，表示删除所有消息扩展
     *
     * @note
     * - 当多个用户同时设置或删除同一个扩展 key 时，只有第一个用户可以执行成功，其它用户会收到 23001 错误码和最新的扩展信息，在收到错误码和扩展信息后，请按需重新发起删除操作。
     */
    static void DeleteMessageExtensions(const V2TIMMessage &message, const V2TIMStringVector &keys, V2TIMValueCallback<V2TIMMessageExtensionResultVector> *callback);

    /**
     *  5.20 翻译文本消息
     *
     *  @param sourceTextList 待翻译文本数组。
     *  @param sourceLanguage 源语言。可以设置为特定语言或 ”auto“。“auto“ 表示自动识别源语言。传空默认为 ”auto“。
     *  @param targetLanguage 目标语言。支持的目标语言有多种，例如：英语-“en“，简体中文-”zh“，法语-”fr“，德语-”de“等。
     *  @param callback 翻译结果回调。其中 result 的 key 为待翻译文本, value 为翻译后文本。
     */
    static void TranslateText(const V2TIMStringVector &sourceTextList,
                               const V2TIMString &sourceLanguage, const V2TIMString &targetLanguage,
                               V2TIMValueCallback<V2TIMStringToV2TIMStringMap> *callback);

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                         群相关的高级接口
    //
    /////////////////////////////////////////////////////////////////////////////////

    /**
     * 1.1 创建自定义群组（高级版本：可以指定初始的群成员）
     *
     * @param info       自定义群组信息，可以设置 groupID | groupType | groupName | notification |
     * introduction | faceURL 字段
     * @param memberList 指定初始的群成员（直播群 AVChatRoom 不支持指定初始群成员，memberList
     * 请传一个大小为 0 的 V2TIMCreateGroupMemberInfoVector）
     *
     * @note 其他限制请参考 @ref V2TIMManager.h -> CreateGroup 注释
     */
    static void CreateGroup(const V2TIMGroupInfo& info,
                             const V2TIMCreateGroupMemberInfoVector& memberList,
                             V2TIMValueCallback<V2TIMString>* callback);

    /**
     * 1.2 获取当前用户已经加入的群列表
     *
     * @note
     * - 直播群(AVChatRoom) 不支持该 API。
     * - 该接口有频限检测，SDK 限制调用频率为1 秒 10 次，超过限制后会报
     * ERR_SDK_COMM_API_CALL_FREQUENCY_LIMIT （7008）错误
     */
    static void GetJoinedGroupList(V2TIMValueCallback<V2TIMGroupInfoVector>* callback);

    /////////////////////////////////////////////////////////////////////////////////
    //                         群资料和高级设置项
    /////////////////////////////////////////////////////////////////////////////////

    /**
     *  2.1 拉取群资料
     *
     *  @param groupIDList 群组 ID 列表
     */
    static void GetGroupsInfo(const V2TIMStringVector& groupIDList,
                               V2TIMValueCallback<V2TIMGroupInfoResultVector>* callback);

    /**
     * 2.2 搜索群资料（5.4.666 及以上版本支持）
     *
     * @param searchParam 搜索参数
     * @note 该功能为 IM 旗舰版功能，[购买旗舰版套餐包](https://buy.cloud.tencent.com/avc?from=17474)后可使用，详见[价格说明](https://cloud.tencent.com/document/product/269/11673?from=17176#.E5.9F.BA.E7.A1.80.E6.9C.8D.E5.8A.A1.E8.AF.A6.E6.83.85)
     */
    static void SearchGroups(const V2TIMGroupSearchParam& searchParam,
                              V2TIMValueCallback<V2TIMGroupInfoVector>* callback);

    /**
     * 2.3 修改群资料
     */
    static void SetGroupInfo(const V2TIMGroupInfo& info, V2TIMCallback* callback);

    /**
     * 2.4 初始化群属性，会清空原有的群属性列表
     *
     * @note
     * attributes 的使用限制如下：
     *  - 6.7 及其以前版本，只支持 AVChatRoom 直播群；
     *  - 从 6.8 版本开始，同时支持 AVChatRoom、Public、Meeting、Work 四种群类型；
     *  - 从 7.0 版本开始，除了话题外，群属性支持所有的群类型；
     *  - key 最多支持 16 个，长度限制为 32 字节；
     *  - value 长度限制为 4k；
     *  - 总的 attributes（包括 key 和 value）限制为 16k；
     *  - initGroupAttributes、setGroupAttributes、deleteGroupAttributes 接口合并计算， SDK 限制为 5 秒 10 次，超过后回调 8511 错误码；后台限制 1 秒 5 次，超过后返回 10049 错误码；
     *  - getGroupAttributes 接口 SDK 限制 5 秒 20 次；
     *  - 从 5.6 版本开始，当每次APP启动后初次修改群属性时，请您先调用 getGroupAttributes 拉取到最新的群属性之后，再发起修改操作；
     *  - 从 5.6 版本开始，当多个用户同时修改同一个群属性时，只有第一个用户可以执行成功，其它用户会收到 10056 错误码；收到这个错误码之后，请您调用 getGroupAttributes 把本地保存的群属性更新到最新之后，再发起修改操作。
     */
    static void InitGroupAttributes(const V2TIMString& groupID,
                                     const V2TIMGroupAttributeMap& attributes,
                                     V2TIMCallback* callback);

    /**
     * 2.5 设置群属性。已有该群属性则更新其 value 值，没有该群属性则添加该属性。
     * @note
     *   - 6.7 及其以前版本，只支持 AVChatRoom 直播群；
     *   - 从 6.8 版本开始，同时支持 AVChatRoom、Public、Meeting、Work 四种群类型；
     *   - 从 7.0 版本开始，除了话题外，群属性支持所有的群类型。
     */
    static void SetGroupAttributes(const V2TIMString& groupID,
                                    const V2TIMGroupAttributeMap& attributes,
                                    V2TIMCallback* callback);

    /**
     * 2.6 删除指定群属性，keys 传大小为 0 的 V2TIMStringVector 则清空所有群属性。
     * @note
     *   - 6.7 及其以前版本，只支持 AVChatRoom 直播群；
     *   - 从 6.8 版本开始，同时支持 AVChatRoom、Public、Meeting、Work 四种群类型；
     *   - 从 7.0 版本开始，除了话题外，群属性支持所有的群类型。
     */
    static void DeleteGroupAttributes(const V2TIMString& groupID, const V2TIMStringVector& keys,
                                       V2TIMCallback* callback);

    /**
     * 2.7 获取指定群属性，keys 传 keys 传大小为 0 的 V2TIMStringVector 则获取所有群属性。
     * @note
     *   - 6.7 及其以前版本，只支持 AVChatRoom 直播群；
     *   - 从 6.8 版本开始，同时支持 AVChatRoom、Public、Meeting、Work 四种群类型；
     *   - 从 7.0 版本开始，除了话题外，群属性支持所有的群类型。
     */
    static void GetGroupAttributes(const V2TIMString& groupID, const V2TIMStringVector& keys,
                                    V2TIMValueCallback<V2TIMGroupAttributeMap>* callback);

    /**
     * 2.8 获取指定群在线人数
     *
     * @note 请注意：
     *   - 目前只支持：直播群（AVChatRoom）。
     *   - 该接口有频限检测，SDK 限制调用频率为60秒1次。
     */
    static void GetGroupOnlineMemberCount(const V2TIMString& groupID,
                                           V2TIMValueCallback<uint32_t>* callback);

    /**
     * 2.9 设置群计数器（7.0 及其以上版本支持）
     *
     * @note
     *  - 该计数器的 key 如果存在，则直接更新计数器的 value 值；如果不存在，则添加该计数器的 key-value；
     *  - 当群计数器设置成功后，在 succ 回调中会返回最终成功设置的群计数器信息；
     *  - 除了社群和话题，群计数器支持所有的群组类型。
     */
    static void SetGroupCounters(const V2TIMString& groupID, const V2TIMStringToInt64Map& counters,
                                  V2TIMValueCallback<V2TIMStringToInt64Map>* callback);

    /**
     * 2.10 获取群计数器（7.0 及其以上版本支持）
     *
     * @note
     *  - 如果 keys 为空，则表示获取群内的所有计数器；
     *  - 除了社群和话题，群计数器支持所有的群组类型。
     */
    static void GetGroupCounters(const V2TIMString& groupID, const V2TIMStringVector& keys,
                                  V2TIMValueCallback<V2TIMStringToInt64Map>* callback);

    /**
     * 2.11 递增群计数器（7.0 及其以上版本支持）
     *
     * @param groupID 群 ID
     * @param key 群计数器的 key
     * @param value 群计数器的递增的变化量，计数器 key 对应的 value 变更方式为： new_value = old_value + value
     * @param callback 如果成功，会返回当前计数器做完递增操作后的 value
     *
     * @note
     *  - 该计数器的 key 如果存在，则直接在当前值的基础上根据传入的 value 作递增操作；反之，添加 key，并在默认值为 0 的基础上根据传入的 value 作递增操作；
     *  - 除了社群和话题，群计数器支持所有的群组类型。
     */
    static void IncreaseGroupCounter(const V2TIMString& groupID,
                                      const V2TIMString& key, int64_t value,
                                      V2TIMValueCallback<V2TIMStringToInt64Map>* callback);

    /**
     * 2.12 递减群计数器（7.0 及其以上版本支持）
     *
     * @param groupID 群 ID
     * @param key 群计数器的 key
     * @param value 群计数器的递减的变化量，计数器 key 对应的 value 变更方式为： new_value = old_value - value
     * @param callback 如果成功，会返回当前计数器做完递减操作后的 value
     *
     * @note
     *  - 该计数器的 key 如果存在，则直接在当前值的基础上根据传入的 value 作递减操作；反之，添加 key，并在默认值为 0 的基础上根据传入的 value 作递减操作
     *  - 除了社群和话题，群计数器支持所有的群组类型。
     */
    static void DecreaseGroupCounter(const V2TIMString& groupID,
                                      const V2TIMString& key, int64_t value,
                                      V2TIMValueCallback<V2TIMStringToInt64Map>* callback);

    /////////////////////////////////////////////////////////////////////////////////
    //                         群成员管理
    /////////////////////////////////////////////////////////////////////////////////

    /**
     * 3.1 获取群成员列表
     *
     * @param filter   指定群成员类型
     * @param nextSeq  分页拉取标志，第一次拉取填0，回调成功如果 nextSeq
     *                 不为零，需要分页，传入再次拉取，直至为0。
     *
     *  @note
     *  - 普通群（工作群、会议群、公开群、社群）的限制：
     *  1. filter 只能设置为 V2TIMGroupMemberFilter 定义的数值，SDK 会返回指定角色的成员。
     *
     *  - 直播群（AVChatRoom）的限制：
     *  1. 如果设置 filter 为 V2TIMGroupMemberFilter 定义的数值，SDK 返回全部成员。
     *    返回的人数规则为：旗舰版支持拉取最近入群群成员最多 1000 人，新进来的成员排在前面（6.3 及以上版本支持，
     *    需要先在 [控制台](https://console.cloud.tencent.com/im) 开启开关；
     *    非旗舰版支持拉取最近入群群成员最多 31 人，新进来的 成员排在前面。
     *  2. 如果设置 filter 为群成员自定义标记，旗舰版支持拉取指定标记的成员列表。
     *    标记群成员的设置请参考 MarkGroupMemberList API。
     *  3. 程序重启后，请重新加入群组，否则拉取群成员会报 10007 错误码。
     *  4. 群成员资料信息仅支持 userID | nickName | faceURL | role 字段。
     */
    static void GetGroupMemberList(const V2TIMString& groupID, uint32_t filter,
                                    uint64_t nextSeq,
                                    V2TIMValueCallback<V2TIMGroupMemberInfoResult>* callback);

    /**
     * 3.2 获取指定的群成员资料
     */
    static void GetGroupMembersInfo(
        const V2TIMString& groupID, V2TIMStringVector memberList,
        V2TIMValueCallback<V2TIMGroupMemberFullInfoVector>* callback);

    /**
     * 3.3 搜索群成员（5.4.666 及以上版本支持）
     *
     * @param param 搜索参数
     * @note 该功能为 IM 旗舰版功能，[购买旗舰版套餐包](https://buy.cloud.tencent.com/avc?from=17474)后可使用，详见[价格说明](https://cloud.tencent.com/document/product/269/11673?from=17176#.E5.9F.BA.E7.A1.80.E6.9C.8D.E5.8A.A1.E8.AF.A6.E6.83.85)
     */
    static void SearchGroupMembers(
        const V2TIMGroupMemberSearchParam& param,
        V2TIMValueCallback<V2TIMGroupSearchGroupMembersMap>* callback);

    /**
     * 3.4 修改指定的群成员资料
     */
    static void SetGroupMemberInfo(const V2TIMString& groupID,
                                    const V2TIMGroupMemberFullInfo& info,
                                    V2TIMCallback* callback);

    /**
     * 3.5 禁言（只有管理员或群主能够调用）
     *
     * @param seconds 禁言时间长度，单位秒，表示调用该接口成功后多少秒内不允许被禁言用户再发言。
     */
    static void MuteGroupMember(const V2TIMString& groupID, const V2TIMString& userID,
                                 uint32_t seconds,
                                 V2TIMCallback* callback);

    /**
     * 3.6 邀请他人入群
     *
     * @note 请注意不同类型的群有如下限制：
     * - 工作群（Work）：群里的任何人都可以邀请其他人进群。
     * - 会议群（Meeting）和公开群（Public）：只有通过rest api 使用 App
     * 管理员身份才可以邀请其他人进群。
     * - 直播群（AVChatRoom）：不支持此功能。
     */
    static void InviteUserToGroup(
        const V2TIMString& groupID, const V2TIMStringVector& userList,
        V2TIMValueCallback<V2TIMGroupMemberOperationResultVector>* callback);

    /**
     * 3.7 踢人（直播群踢人从 6.6 版本开始支持，需要您购买旗舰版套餐）
     *
     * @note 请注意不同类型的群有如下限制：
     * - 工作群（Work）：只有群主或 APP 管理员可以踢人。
     * - 公开群（Public）、会议群（Meeting）：群主、管理员和 APP 管理员可以踢人
     * - 直播群（AVChatRoom）：6.6 之前版本只支持禁言（MuteGroupMember），不支持踢人（KickGroupMember）。6.6 及以上版本支持禁言和踢人。
     */
    static void KickGroupMember(
        const V2TIMString& groupID, const V2TIMStringVector& memberList, const V2TIMString& reason,
        V2TIMValueCallback<V2TIMGroupMemberOperationResultVector>* callback);

    /**
     * 3.8 切换群成员的角色。
     *
     * @note 请注意不同类型的群有如下限制：
     *  -
     * 公开群（Public）和会议群（Meeting）：只有群主才能对群成员进行普通成员和管理员之间的角色切换。
     *  - 其他群不支持设置群成员角色。
     *  - 转让群组请调用 @ref TransferGroupOwner 接口。
     *  - 会议群（Meeting）切换群成员角色之后，不会有 OnGrantAdministrator 和 OnRevokeAdministrator
     * 通知回调
     *  -
     * 切换的角色支持普通群成员（V2TIM_GROUP_MEMBER_ROLE_MEMBER）和管理员（V2TIM_GROUP_MEMBER_ROLE_ADMIN）
     */
    static void SetGroupMemberRole(const V2TIMString& groupID, const V2TIMString& userID,
        uint32_t role, V2TIMCallback* callback);

    /**
     *  3.9 标记群成员(从 6.6 版本开始支持，需要您购买旗舰版套餐)
     *
     *  @param groupID 群 ID。
     *  @param memberList 群成员 ID 列表。
     *  @param markType 标记类型。数字类型，大于等于 1000，您可以自定义，一个直播群里最多允许定义 10 个标记。
     *  @param enableMark true 表示添加标记，false 表示移除标记。
     *
     *  @note 请注意
     *  - 仅支持直播群。
     *  - 只有群主才有权限标记群组中其他人。
     */
    static void MarkGroupMemberList(const V2TIMString& groupID,
        const V2TIMStringVector& memberList, uint32_t markType,
        bool enableMark, V2TIMCallback* callback);

    /**
     * 3.10 转让群主
     *
     * @note 请注意不同类型的群有如下限制：
     *  - 普通类型的群（Work、Public、Meeting）：只有群主才有权限进行群转让操作。
     *  - 直播群（AVChatRoom）：不支持转让群主。
     */
    static void TransferGroupOwner(const V2TIMString& groupID, const V2TIMString& userID,
                                    V2TIMCallback* callback);

    /////////////////////////////////////////////////////////////////////////////////
    //                         加群申请
    /////////////////////////////////////////////////////////////////////////////////

    /**
     * 4.1 获取加群申请列表
     * @note 最多支持50个
     */
    static void GetGroupApplicationList(
        V2TIMValueCallback<V2TIMGroupApplicationResult>* callback);

    /**
     * 4.2 同意某一条加群申请
     */
    static void AcceptGroupApplication(const V2TIMGroupApplication& application,
                                        const V2TIMString& reason, V2TIMCallback* callback);

    /**
     * 4.3 拒绝某一条加群申请
     */
    static void RefuseGroupApplication(const V2TIMGroupApplication& application,
                                        const V2TIMString& reason, V2TIMCallback* callback);

    /**
     * 4.4 标记申请列表为已读
     */
    static void SetGroupApplicationRead(V2TIMCallback* callback);

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                         社群-话题
    //
    /////////////////////////////////////////////////////////////////////////////////

    /**
     * 5.1 获取当前用户已经加入的支持话题的社群列表
     */
    static void GetJoinedCommunityList(V2TIMValueCallback<V2TIMGroupInfoVector> *callback);

    /**
     * 5.2 创建话题
     */
    static void CreateTopicInCommunity(const V2TIMString &groupID, const V2TIMTopicInfo &topicInfo,
        V2TIMValueCallback<V2TIMString> *callback);

    /**
     * 5.3 删除话题
     */
    static void DeleteTopicFromCommunity(const V2TIMString &groupID,
        const V2TIMStringVector &topicIDList,
        V2TIMValueCallback<V2TIMTopicOperationResultVector> *callback);

    /**
     * 5.4 修改话题信息
     */
    static void SetTopicInfo(const V2TIMTopicInfo &topicInfo, V2TIMCallback *callback);

    /**
     * 5.5 获取话题列表。
     * @note: topicIDList 传空时，获取此社群下的所有话题列表
     */
    static void GetTopicInfoList(const V2TIMString &groupID, const V2TIMStringVector &topicIDList,
        V2TIMValueCallback<V2TIMTopicInfoResultVector> *callback);

    /**
     * 1.1 添加会话监听器
     */
    static void AddConversationListener(V2TIMConversationListener* listener);

    /**
     * 1.2 移除会话监听器
     */
    static void RemoveConversationListener(V2TIMConversationListener* listener);

    /**
     * 1.3 获取会话列表
     *
     * - 一个会话对应一个聊天窗口，比如跟一个好友的 1v1 聊天，或者一个聊天群，都是一个会话。
     * -
     * 由于历史的会话数量可能很多，所以该接口希望您采用分页查询的方式进行调用，每次分页拉取的个数建议为
     * 100 个。
     * - 该接口拉取的是本地缓存的会话，如果服务器会话有更新，SDK 内部会自动同步，然后在 @ref
     * V2TIMConversationListener 回调告知客户。
     * - 如果会话全部拉取完毕，成功回调里面 V2TIMConversationResult 中的 isFinished 获取字段值为
     * true。
     *
     * @note 会话排序规则
     * - 5.5.892 及以后版本, 该接口获取的会话列表默认已经按照会话 orderKey 做了排序，orderKey
     * 值越大，代表该会话排序越靠前。
     * - 5.5.892 以前版本，该接口获取的会话列表默认已经按照会话 lastMessage -> timestamp
     * 做了排序，timestamp 越大，会话越靠前。
     *
     * @param nextSeq   分页拉取的游标，第一次默认取传 0，后续分页拉传上一次分页拉取成功回调里的
     * nextSeq
     * @param count    分页拉取的个数，一次分页拉取不宜太多，会影响拉取的速度，建议每次拉取 100
     * 个会话
     */
    static void GetConversationList(uint64_t nextSeq, uint32_t count,
                                     V2TIMValueCallback<V2TIMConversationResult>* callback);

    /**
     * 1.4 获取单个会话
     *
     * @param conversationID  会话唯一 ID，C2C 单聊组成方式为: "c2c_userID"：
     * 群聊组成方式为: "group_groupID")
     */
    static void GetConversation(const V2TIMString& conversationID,
                                 V2TIMValueCallback<V2TIMConversation>* callback);

    /**
     * 1.5 获取指定会话列表
     *
     * @param conversationIDList 会话唯一 ID，C2C 单聊组成方式为: "c2c_userID"：
     * 群聊组成方式为: "group_groupID")
     */
    static void GetConversationList(const V2TIMStringVector& conversationIDList,
                                     V2TIMValueCallback<V2TIMConversationVector>* callback);

    /**
     *  1.6 获取会话列表高级接口（从 6.5 版本开始支持）
     *
     * @param filter 会话 filter
     * @param nextSeq 分页拉取的游标
     * @param count 分页拉取的个数
     *
     */
    static void GetConversationListByFilter(const V2TIMConversationListFilter &filter,
                                             uint64_t nextSeq, uint32_t count,
                                             V2TIMValueCallback<V2TIMConversationResult>* callback);

    /**
     * 1.7 删除会话
     *
     * @param conversationID 会话唯一 ID，C2C 单聊组成方式为: "c2c_userID"：
     * 群聊组成方式为: "group_groupID")
     *
     * @note 请注意:
     * - 删除会话会在本地删除的同时，在服务器也会同步删除。
     * - 会话内的消息在本地删除的同时，在服务器也会同步删除。
     */
    static void DeleteConversation(const V2TIMString& conversationID, V2TIMCallback* callback);

    /**
     * 1.8 设置会话草稿
     *
     * @param conversationID 会话唯一 ID，C2C 会话唯一 ID，C2C 单聊组成方式为: "c2c_userID"：
     * 群聊组成方式为: "group_groupID")
     *
     * 只在本地保存，不会存储 Server，不能多端同步，程序卸载重装会失效。
     *
     * @param draftText 草稿内容, 空字符串表示 则表示取消草稿
     */
    static void SetConversationDraft(const V2TIMString& conversationID,
                                      const V2TIMString& draftText, V2TIMCallback* callback);

    /**
     * 1.9 设置会话自定义数据（从 6.5 版本开始支持）
     *
     * @param customData 自定义数据，最大支持 256 bytes
     */
    static void SetConversationCustomData(const V2TIMStringVector &conversationIDList, const V2TIMBuffer &customData,
                                           V2TIMValueCallback<V2TIMConversationOperationResultVector>* callback);

    /**
     * 1.10 设置会话置顶（5.3.425 及以上版本支持）
     *
     * @param conversationID 会话唯一 ID，C2C 单聊组成方式为: "c2c_userID"：
     * 群聊组成方式为: "group_groupID")
     *
     * @param isPinned 是否置顶
     */
    static void PinConversation(const V2TIMString& conversationID, bool isPinned,
                                 V2TIMCallback* callback);

    /**
     * 1.11 标记会话（从 6.5 版本开始支持，需要您购买旗舰版套餐）
     *
     * @param conversationIDList 会话列表
     * @param markType 会话标记类型，取值详见 @V2TIMConversationMarkType。
     * @param enableMark YES：设置标记 NO：取消标记
     *
     * 如果已有标记不能满足您的需求，您可以自定义扩展标记，扩展标记需要满足以下两个条件：
     * 1、扩展标记值不能和 V2TIMConversationMarkType 已有的标记值冲突
     * 2、扩展标记值必须是 0x1LL << n 的位移值（32 <= n < 64，即 n 必须大于等于 32 并且小于 64），比如扩展标记值 0x1LL << 32 表示 "Windows 在线"
     *
     * @note:扩展标记值不能设置为 0x1 << 32，要设置为 0x1LL << 32，明确告诉编译器是 64 位的整型常量。
     *
     */
    static void MarkConversation(const V2TIMStringVector &conversationIDList, uint64_t markType, bool enableMark,
                                  V2TIMValueCallback<V2TIMConversationOperationResultVector>* callback);

    /**
     * 1.12 获取全部会话的未读总数（5.3.425 及以上版本支持）
     * @note
     *  - 调用该接口以后，任意会话的未读数发生变化时，SDK 都会给您抛 OnTotalUnreadMessageCountChanged 回调。
     *  - 未读总数会减去设置为免打扰的会话的未读数，即消息接收选项设置为
     *  V2TIM_NOT_RECEIVE_MESSAGE 或 V2TIM_RECEIVE_NOT_NOTIFY_MESSAGE 的会话。
     */
    static void GetTotalUnreadMessageCount(V2TIMValueCallback<uint64_t>* callback);

    /**
     * 1.13 获取按会话 filter 过滤的未读总数（7.0 及以上版本支持）
     *
     * @param filter 会话 filter
     *
     * @note
     *  - 未读总数会减去设置为免打扰的会话的未读数，即消息接收选项设置为
     *  V2TIM_NOT_RECEIVE_MESSAGE 或 V2TIM_RECEIVE_NOT_NOTIFY_MESSAGE 的会话。
     */
    static void GetUnreadMessageCountByFilter(const V2TIMConversationListFilter &filter,
        V2TIMValueCallback<uint64_t>* callback);

    /**
     *  1.14 注册监听指定 filter 的会话未读总数变化（7.0 及以上版本支持）
     *
     * @param filter 会话 filter
     *
     * @note
     *  - 当您调用这个接口以后，该 filter 下的未读数发生变化时，SDK 会给您抛 OnUnreadMessageCountChangedByFilter 回调。
     */
    static void SubscribeUnreadMessageCountByFilter(const V2TIMConversationListFilter &filter);

    /**
     *  1.15 取消监听指定 filter 的会话未读总数变化（7.0 及以上版本支持）
     *
     * @param filter 会话 filter
     *
     */
    static void UnsubscribeUnreadMessageCountByFilter(const V2TIMConversationListFilter &filter);

    /////////////////////////////////////////////////////////////////////////////////
    //
    //               会话分组相关接口 （从 6.5 版本开始支持，需要您购买旗舰版套餐）
    //
    /////////////////////////////////////////////////////////////////////////////////

    /**
     * 2.1 创建会话分组
     *
     * @param groupName 分组名（必填参数，长度要 > 0，最大支持 32 bytes）
     * @param conversationIDList 会话 ID 列表（必填参数，不能为空）
     *
     * @note 会话分组最大支持 20 个，不再使用的分组请及时删除
     */
    static void CreateConversationGroup(const V2TIMString &groupName, const V2TIMStringVector &conversationIDList,
                                         V2TIMValueCallback<V2TIMConversationOperationResultVector>* callback);

    /**
     * 2.2 获取会话分组列表
     */
    static void GetConversationGroupList(V2TIMValueCallback<V2TIMStringVector>* callback);

    /**
     * 2.3 删除会话分组
     */
    static void DeleteConversationGroup(const V2TIMString &groupName, V2TIMCallback* callback);

    /**
     * 2.4 重命名会话分组
     *
     */
    static void RenameConversationGroup(const V2TIMString &oldName, const V2TIMString &newName,
                                         V2TIMCallback* callback);

    /**
     * 2.5 添加会话到一个会话分组
     */
    static void AddConversationsToGroup(const V2TIMString &groupName, const V2TIMStringVector &conversationIDList,
                                         V2TIMValueCallback<V2TIMConversationOperationResultVector>* callback);

    /**
     * 2.6 从一个会话分组中删除会话
     */
    static void DeleteConversationsFromGroup(const V2TIMString &groupName, const V2TIMStringVector &conversationIDList,
                                              V2TIMValueCallback<V2TIMConversationOperationResultVector>* callback);

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                         关系链和用户资料监听器
    //
    /////////////////////////////////////////////////////////////////////////////////

    /**
     * 1.1 添加关系链监听器
     */
    static void AddFriendListener(V2TIMFriendshipListener* listener);

    /**
     * 1.2 移除关系链监听器
     */
    static void RemoveFriendListener(V2TIMFriendshipListener* listener);

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                       好友添加、删除、列表获取、资料设置接口函数
    //
    /////////////////////////////////////////////////////////////////////////////////

    /**
     * 2.1 获取好友列表
     */
    static void GetFriendList(V2TIMValueCallback<V2TIMFriendInfoVector>* callback);

    /**
     * 2.2 获取指定好友资料
     *
     * @param userIDList 好友 userID 列表
     *                   - ID 建议一次最大 100
     * 个，因为数量过多可能会导致数据包太大被后台拒绝，后台限制数据包最大为 1M。
     */
    static void GetFriendsInfo(const V2TIMStringVector& userIDList,
                                V2TIMValueCallback<V2TIMFriendInfoResultVector>* callback);

    /**
     * 2.3 设置指定好友资料
     */
    static void SetFriendInfo(const V2TIMFriendInfo& info, V2TIMCallback* callback);

    /**
     * 2.4 搜索好友（5.4.666 及以上版本支持，需要您购买旗舰版套餐）
     * @note
     *  - 接口返回本地存储的用户资料，可以根据 V2TIMFriendInfoResult 中的 getRelation 来判断是否为好友。
     *  - 该功能为 IM 旗舰版功能，[购买旗舰版套餐包](https://buy.cloud.tencent.com/avc?from=17474)后可使用，详见[价格说明](https://cloud.tencent.com/document/product/269/11673?from=17176#.E5.9F.BA.E7.A1.80.E6.9C.8D.E5.8A.A1.E8.AF.A6.E6.83.85)
     */
    static void SearchFriends(const V2TIMFriendSearchParam& searchParam,
                               V2TIMValueCallback<V2TIMFriendInfoResultVector>* callback);

    /**
     * 2.5 添加好友
     */
    static void AddFriend(const V2TIMFriendAddApplication& application,
                           V2TIMValueCallback<V2TIMFriendOperationResult>* callback);

    /**
     * 2.6 删除好友
     *
     * @param userIDList 要删除的好友 userID 列表
     *                   - ID 建议一次最大 100
     * 个，因为数量过多可能会导致数据包太大被后台拒绝，后台限制数据包最大为 1M。
     * @param deleteType 删除类型
     *                   - @ref V2TIM_FRIEND_TYPE_SINGLE：单向好友
     *                   - @ref V2TIM_FRIEND_TYPE_BOTH：双向好友
     */
    static void DeleteFromFriendList(
        const V2TIMStringVector& userIDList, V2TIMFriendType deleteType,
        V2TIMValueCallback<V2TIMFriendOperationResultVector>* callback);

    /**
     * 2.7 检查指定用户的好友关系
     *
     *  @param userIDList 要检查的 userID 列表
     *  @param checkType 检查类型 （单向好友检查、双向好友检查）
     *
     *  @note checkType 的使用需要注意：
     *  - checkType 如果传入
     * V2TIM_FRIEND_TYPE_SINGLE，结果返回：V2TIM_FRIEND_RELATION_TYPE_NONE、V2TIM_FRIEND_RELATION_TYPE_IN_MY_FRIEND_LIST
     * 两种情况
     *  - checkType 如果传入
     * V2TIM_FRIEND_TYPE_BOTH，结果返回：V2TIM_FRIEND_RELATION_TYPE_NONE、V2TIM_FRIEND_RELATION_TYPE_IN_MY_FRIEND_LIST、
     *                                                      V2TIM_FRIEND_RELATION_TYPE_IN_OTHER_FRIEND_LIST、V2TIM_FRIEND_RELATION_TYPE_BOTH_WAY
     * 四种情况
     */
    static void CheckFriend(const V2TIMStringVector& userIDList, V2TIMFriendType checkType,
                             V2TIMValueCallback<V2TIMFriendCheckResultVector>* callback);

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                          好友申请、删除接口函数
    //
    /////////////////////////////////////////////////////////////////////////////////

    /**
     *  3.1 获取好友申请列表
     *
     *  @note
     *   - 好友申请列表包括发起的好友申请和收到的好友申请。
     *   - 最多支持100个。
     */
    static void GetFriendApplicationList(
        V2TIMValueCallback<V2TIMFriendApplicationResult>* callback);

    /**
     * 3.2 同意好友申请
     *
     * @param application  好友申请信息，GetFriendApplicationList 成功后会返回
     * @param responseType 建立单向/双向好友关系
     */
    static void AcceptFriendApplication(
        const V2TIMFriendApplication& application, V2TIMFriendAcceptType acceptType,
        V2TIMValueCallback<V2TIMFriendOperationResult>* callback);

    /**
     * 3.3 拒绝好友申请
     *
     * @param application 好友申请信息，@ref GetFriendApplicationList 成功后会返回
     */
    static void RefuseFriendApplication(
        const V2TIMFriendApplication& application,
        V2TIMValueCallback<V2TIMFriendOperationResult>* callback);

    /**
     * 3.4 删除好友申请
     *
     * @param application 好友申请信息，@ref GetFriendApplicationList 成功后会返回
     */
    static void DeleteFriendApplication(const V2TIMFriendApplication& application,
                                         V2TIMCallback* callback);

    /**
     * 3.5 设置好友申请已读
     */
    static void SetFriendApplicationRead(V2TIMCallback* callback);

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                          黑名单接口函数
    //
    /////////////////////////////////////////////////////////////////////////////////

    /**
     * 4.1 添加用户到黑名单
     */
    static void AddToBlackList(const V2TIMStringVector& userIDList,
                                V2TIMValueCallback<V2TIMFriendOperationResultVector>* callback);

    /**
     * 4.2 把用户从黑名单中删除
     */
    static void DeleteFromBlackList(
        const V2TIMStringVector& userIDList,
        V2TIMValueCallback<V2TIMFriendOperationResultVector>* callback);

    /**
     * 4.3 获取黑名单列表
     */
    static void GetBlackList(V2TIMValueCallback<V2TIMFriendInfoVector>* callback);

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                          好友分组接口函数
    //
    /////////////////////////////////////////////////////////////////////////////////

    /**
     * 5.1 新建好友分组
     *
     * @param groupName  分组名称
     * @param userIDList 要添加到分组中的好友 userID 列表
     */
    static void CreateFriendGroup(
        const V2TIMString& groupName, const V2TIMStringVector& userIDList,
        V2TIMValueCallback<V2TIMFriendOperationResultVector>* callback);

    /**
     * 5.2 获取分组信息
     *
     * @param groupNameList 要获取信息的好友分组名称列表,传入 传入空的 V2TIMStringVector 获得所有分组信息
     */
    static void GetFriendGroups(const V2TIMStringVector& groupNameList,
                                 V2TIMValueCallback<V2TIMFriendGroupVector>* callback);

    /**
     * 5.3 删除好友分组
     */
    static void DeleteFriendGroup(const V2TIMStringVector& groupNameList,
                                   V2TIMCallback* callback);

    /**
     * ## 修改好友分组的名称
     *
     * @param oldName  旧的分组名称
     * @param newName  新的分组名称
     * @param callback 回调
     */
    static void RenameFriendGroup(const V2TIMString& oldName, const V2TIMString& newName,
                                   V2TIMCallback* callback);

    /**
     * 5.4 添加好友到一个好友分组
     */
    static void AddFriendsToFriendGroup(
        const V2TIMString& groupName, const V2TIMStringVector& userIDList,
        V2TIMValueCallback<V2TIMFriendOperationResultVector>* callback);

    /**
     * 5.5 从好友分组中删除好友
     */
    static void DeleteFriendsFromFriendGroup(
        const V2TIMString& groupName, const V2TIMStringVector& userIDList,
        V2TIMValueCallback<V2TIMFriendOperationResultVector>* callback);

    /**
     * 设置离线推送配置信息
     * note
     *  - 如果您想关闭离线推送，请把 config 设置为 V2TIMOfflinePushConfig()
     * @param config   离线推送配置
     * @param callback 回调
     */
    static void SetOfflinePushConfig(const V2TIMOfflinePushConfig &config, V2TIMCallback *callback);

    /**
     * APP 检测到应用退后台时可以调用此接口，可以用作桌面应用角标的初始化未读数量。
     *
     * - 从 6.1 版本开始，如果配置了离线推送，会收到厂商或 TPNS 的离线推送通道下发的通知栏消息。
     *
     * @param unreadCount 未读数量
     * @param callback 回调
     */
    static void DoBackground(uint32_t unreadCount, V2TIMCallback *callback);

    /**
     * APP 检测到应用进前台时可以调用此接口
     *
     * - 从 6.1 版本开始，调用 DoForeground，会停止离线推送。但如果应用被 kill，仍然可以正常接收离线推送。
     *
     * @param callback 回调
     */
    static void DoForeground(V2TIMCallback *callback);

    /**
     * 添加信令监听
     */
    static void AddSignalingListener(V2TIMSignalingListener* listener);

    /**
     * 移除信令监听
     */
    static void RemoveSignalingListener(V2TIMSignalingListener* listener);

    /**
     * 邀请某个人
     *
     * @param invitee 被邀请人用户 ID
     * @param data 自定义数据
     * @param onlineUserOnly 是否只有在线用户才能收到邀请，如果设置为 true，只有在线用户才能收到，
     *                       并且 Invite 操作也不会产生历史消息（针对该次 Invite 的后续
     * Cancel、Accept、Reject、Timeout 操作也同样不会产生历史消息）。
     * @param offlinePushInfo 离线推送信息，其中 desc 为必填字段，推送的时候会默认展示 desc 信息。
     * @param timeout 超时时间，单位秒，如果设置为 0，SDK 不会做超时检测，也不会触发
     * onInvitationTimeout 回调
     * @return inviteID 邀请 ID，如果邀请失败，返回空字符串
     */
    static V2TIMString Invite(const V2TIMString& invitee, const V2TIMString& data,
                               bool onlineUserOnly,
                               const V2TIMOfflinePushInfo& offlinePushInfo, int timeout,
                               V2TIMCallback* callback);

    /**
     * 邀请群内的某些人
     *
     * @param groupID   发起邀请所在群组
     * @param inviteeList 被邀请人列表，inviteeList 必须已经在 groupID 群里，否则邀请无效
     * @param onlineUserOnly 是否只有在线用户才能收到邀请，如果设置为 true，只有在线用户才能收到，
     *                       并且 Invite 操作也不会产生历史消息（针对该次 Invite 的后续
     * Cancel、Accept、Reject、Timeout 操作也同样不会产生历史消息）。
     * @param timeout 超时时间，单位秒，如果设置为 0，SDK 不会做超时检测，也不会触发
     * onInvitationTimeout 回调
     * @return inviteID 邀请 ID，如果邀请失败，返回空字符串
     *
     * @note
     * 群邀请暂不支持离线推送，如果您需要离线推送，可以针对被邀请的用户单独发离线推送自定义消息，
     * 详细代码请参考 TRTCAVCallImpl -> sendOnlineMessageWithOfflinePushInfo 函数。
     */
    static V2TIMString InviteInGroup(const V2TIMString& groupID,
                                      const V2TIMStringVector& inviteeList, const V2TIMString& data,
                                      bool onlineUserOnly, int timeout,
                                      V2TIMCallback* callback);

    /**
     * 邀请方取消邀请
     *
     * @param inviteID 邀请 ID
     *
     * @note 如果所有被邀请人都已经处理了当前邀请（包含超时），不能再取消当前邀请。
     */
    static void Cancel(const V2TIMString& inviteID, const V2TIMString& data,
                        V2TIMCallback* callback);

    /**
     * 接收方接收邀请
     *
     * @note 不能接受不是针对自己的邀请，请在收到 OnReceiveNewInvitation 回调的时候先判断
     * inviteeList 有没有自己，如果没有自己，不能 Accept 邀请。
     */
    static void Accept(const V2TIMString& inviteID, const V2TIMString& data,
                        V2TIMCallback* callback);

    /**
     * 接收方拒绝邀请
     *
     * @note 不能拒绝不是针对自己的邀请，请在收到 OnReceiveNewInvitation 回调的时候先判断
     * inviteeList 有没有自己，如果没有自己，不能 Reject 邀请。
     */
    static void Reject(const V2TIMString& inviteID, const V2TIMString& data,
                        V2TIMCallback* callback);

    /**
     * 获取信令信息
     *
     * 如果 Invite 设置 onlineUserOnly 为 false，每次信令操作（包括
     * Invite、Cancel、Accept、Reject、Timeout）都会产生一条自定义消息， 该消息会通过
     * V2TIMAdvancedMsgListener -> onRecvNewMessage
     * 抛给用户，用户也可以通过历史消息拉取，如果需要根据信令信息做自定义化文本展示，可以调用下面接口获取信令信息。
     *
     * @param msg 消息对象
     * @return V2TIMSignalingInfo 信令信息，如果 V2TIMSignalingInfo::inviteID 为空字符串，则 msg 不是一条信令消息。
     */
    static V2TIMSignalingInfo GetSignalingInfo(const V2TIMMessage& msg);

    /**
     *  添加邀请信令
     *
     *  主要用于邀请者在被邀请者离线期间，发送了群聊邀请，被邀请者上线后将该信令同步给 SDK，从而正常使用信令功能。
     *
     *  当被邀请者点击离线推送提示，拉起 App 时：
     *  1. 如果被邀请者离线期间，邀请者发送的是 1V1 信令，SDK 可以自动同步邀请信令。邀请未超时，回调 onReceiveNewInvitation。
     *
     *  2. 如果被邀请者离线期间，邀请者发送的是群聊信令，不同 SDK 版本表现如下：
     *  - 6.7 以前的版本：
     *  SDK 无法自动同步邀请信令（信令本质上就是一条自定义消息，群离线消息在程序启动后无法自动同步）。
     *  如果被邀请者需要处理该邀请信令，可以让邀请者在发起信令时，向每个被邀请者额外发送一条 C2C 离线推送消息，消息携带 V2TIMSignalingInfo 信息。
     *  被邀请者收到离线推送时通过 addInvitedSignaling 将 V2TIMSignalingInfo 信息告知 SDK。
     *  - 6.7 及以后的版本：
     *  SDK 会自动同步最近 30 秒的消息。如果其中包含了未超时的邀请信令，回调 onReceiveNewInvitation。您无需再调用本接口同步邀请信令。
     *
     *  @note 如果添加的信令信息已存在，fail callback 会抛 ERR_SDK_SIGNALING_ALREADY_EXISTS 错误码。
     */
    static void AddInvitedSignaling(const V2TIMSignalingInfo& info, V2TIMCallback* callback);

    /**
     *  修改邀请信令（6.7 及其以上版本支持）
     *
     *  @note 仅支持修改邀请信令的自定义字段 data。只有在线用户才能收到的邀请信令不能被修改。
     */
    static void ModifyInvitation(const V2TIMString& inviteID, const V2TIMString& data,
                                  V2TIMCallback* callback);
};
