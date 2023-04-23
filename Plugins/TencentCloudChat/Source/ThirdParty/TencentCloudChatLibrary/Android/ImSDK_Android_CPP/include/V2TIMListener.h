// Copyright (c) 2021 Tencent. All rights reserved.

/////////////////////////////////////////////////////////////////////////////////
//
//                          IMSDK 全局监听类
//
/////////////////////////////////////////////////////////////////////////////////

#ifndef __V2TIM_LISTENER_H__
#define __V2TIM_LISTENER_H__

#include "V2TIMCommon.h"
#include "V2TIMConversation.h"
#include "V2TIMFriendship.h"
#include "V2TIMGroup.h"
#include "V2TIMMessage.h"
#include "V2TIMSignaling.h"

/////////////////////////////////////////////////////////////////////////////////
//
//                      （一）SDK 事件监听类
//
/////////////////////////////////////////////////////////////////////////////////

class TIM_API V2TIMSDKListener {
public:
    V2TIMSDKListener();

    virtual ~V2TIMSDKListener();

    /**
     * SDK 正在连接到腾讯云服务器
     */
    virtual void OnConnecting() {}

    /**
     * SDK 已经成功连接到腾讯云服务器
     */
    virtual void OnConnectSuccess() {}

    /**
     * SDK 连接腾讯云服务器失败
     */
    virtual void OnConnectFailed(int error_code, const V2TIMString &error_message) {}

    /**
     * 当前用户被踢下线，此时可以 UI 提示用户，并再次调用 V2TIMManager 的 login() 函数重新登录。
     */
    virtual void OnKickedOffline() {}

    /**
     * 在线时票据过期：此时您需要生成新的 userSig 并再次调用 V2TIMManager 的 login() 函数重新登录。
     */
    virtual void OnUserSigExpired() {}

    /**
     * 登录用户的资料发生了更新
     *
     */
    virtual void OnSelfInfoUpdated(const V2TIMUserFullInfo &info) {}

    /**
     * 用户状态变更通知
     *
     * @note 收到通知的情况：
     * 1. 订阅过的用户发生了状态变更（包括在线状态和自定义状态），会触发该回调
     * 2. 在 IM 控制台打开了好友状态通知开关，即使未主动订阅，当好友状态发生变更时，也会触发该回调
     * 3. 同一个账号多设备登录，当其中一台设备修改了自定义状态，所有设备都会收到该回调
     */
    virtual void OnUserStatusChanged(const V2TIMUserStatusVector &userStatusList) {}
};

/////////////////////////////////////////////////////////////////////////////////
//
//                      （二）基本消息（文本消息和自定义消息）的事件监听类
//
/////////////////////////////////////////////////////////////////////////////////

class TIM_API V2TIMSimpleMsgListener {
public:
    V2TIMSimpleMsgListener();

    virtual ~V2TIMSimpleMsgListener();

    /**
     * 收到 C2C 文本消息
     *
     * @param msgID 消息唯一标识
     * @param sender 发送方信息
     * @param text 发送内容
     */
    virtual void OnRecvC2CTextMessage(const V2TIMString &msgID, const V2TIMUserFullInfo &sender,
                                      const V2TIMString &text) {}

    /**
     * 收到 C2C 自定义（信令）消息
     *
     * @param msgID 消息唯一标识
     * @param sender 发送方信息
     * @param customData 发送内容
     */
    virtual void OnRecvC2CCustomMessage(const V2TIMString &msgID, const V2TIMUserFullInfo &sender,
                                        const V2TIMBuffer &customData) {}

    /**
     * 收到群文本消息
     *
     * @param msgID 消息唯一标识
     * @param groupID 群 ID
     * @param sender 发送方群成员信息
     * @param text 发送内容
     */
    virtual void OnRecvGroupTextMessage(const V2TIMString &msgID, const V2TIMString &groupID,
                                        const V2TIMGroupMemberFullInfo &sender,
                                        const V2TIMString &text) {}

    /**
     * 收到群自定义（信令）消息
     *
     * @param msgID 消息唯一标识
     * @param groupID 群 ID
     * @param sender 发送方群成员信息
     * @param customData 发送内容
     */
    virtual void OnRecvGroupCustomMessage(const V2TIMString &msgID, const V2TIMString &groupID,
                                          const V2TIMGroupMemberFullInfo &sender,
                                          const V2TIMBuffer &customData) {}
};

DEFINE_POINT_VECTOR(V2TIMSimpleMsgListener)
typedef TXPV2TIMSimpleMsgListenerVector V2TIMSimpleMsgListenerVector;

/////////////////////////////////////////////////////////////////////////////////
//
//                       （三）高级消息监听器
//
/////////////////////////////////////////////////////////////////////////////////

class TIM_API V2TIMAdvancedMsgListener {
public:
    V2TIMAdvancedMsgListener();

    virtual ~V2TIMAdvancedMsgListener();

    /**
     * 收到新消息
     *
     * @param message 消息
     */
    virtual void OnRecvNewMessage(const V2TIMMessage &message) {}

    /**
     * C2C 对端用户会话已读通知（如果对端用户调用 MarkC2CMessageAsRead 接口，自己会收到该回调，回调只会携带对端 userID 和对端已读 timestamp 信息）
     *
     * @param receiptList 已读回执列表
     */
    virtual void OnRecvC2CReadReceipt(const V2TIMMessageReceiptVector &receiptList) {}

    /**
     * 消息已读回执通知
     *
     * @param receiptList 已读回执列表
     */
    virtual void OnRecvMessageReadReceipts(const V2TIMMessageReceiptVector &receiptList) {}

    /**
     * 收到消息撤回的通知
     *
     * @param messageID 消息唯一标识
     */
    virtual void OnRecvMessageRevoked(const V2TIMString &messageID) {}

    /**
     * 消息内容被修改
     */
    virtual void OnRecvMessageModified(const V2TIMMessage &message) {}

    /**
     * 消息扩展信息更新
     */
    virtual void OnRecvMessageExtensionsChanged(const V2TIMString &msgID,
                                                const V2TIMMessageExtensionVector &extensions) {}

    /**
     * 消息扩展信息被删除
     */
    virtual void OnRecvMessageExtensionsDeleted(const V2TIMString &msgID,
                                                const V2TIMStringVector &extensionKeys) {}
};

/////////////////////////////////////////////////////////////////////////////////
//
//                      （四）群组事件的监听类
//
/////////////////////////////////////////////////////////////////////////////////

class TIM_API V2TIMGroupListener {
public:
    V2TIMGroupListener();

    virtual ~V2TIMGroupListener();

    /**
     * 有新成员加入群（该群所有的成员都能收到）
     * 会议群（Meeting）默认无此回调，如需回调，请前往 [控制台](https://console.cloud.tencent.com/im) 
     * (功能配置 -> 群组配置 -> 群系统通知配置 -> 群成员变更通知) 主动配置。
     * 
     * @param groupID    群 ID
     * @param memberList 加入的成员
     */
    virtual void OnMemberEnter(const V2TIMString &groupID,
                               const V2TIMGroupMemberInfoVector &memberList) {}

    /**
     * 有成员离开群（该群所有的成员都能收到）
     * 会议群（Meeting）默认无此回调，如需回调，请前往 [控制台](https://console.cloud.tencent.com/im) 
     * (功能配置 -> 群组配置 -> 群系统通知配置 -> 群成员变更通知) 主动配置。
     *
     * @param groupID 群 ID
     * @param member  离开的成员
     */
    virtual void OnMemberLeave(const V2TIMString &groupID, const V2TIMGroupMemberInfo &member) {}

    /**
     * 某成员被拉入某群（该群所有的成员都能收到）
     *
     * @param groupID    群 ID
     * @param opUser     处理人
     * @param memberList 被拉进群成员
     */
    virtual void OnMemberInvited(const V2TIMString &groupID, const V2TIMGroupMemberInfo &opUser,
                                 const V2TIMGroupMemberInfoVector &memberList) {}

    /**
     * 有成员被踢出某群（该群所有的成员都能收到）
     *
     * @param groupID    群 ID
     * @param opUser     处理人
     * @param memberList 被踢成员
     */
    virtual void OnMemberKicked(const V2TIMString &groupID, const V2TIMGroupMemberInfo &opUser,
                                const V2TIMGroupMemberInfoVector &memberList) {}

    /**
     * 某成员信息发生变更（该群所有的成员都能收到）。
     * @note 会议群（Meeting）和直播群（AVChatRoom）默认无此回调，如需回调，请前往 [控制台](https://console.cloud.tencent.com/im) (功能配置 -> 群组配置 -> 群系统通知配置 -> 群成员资料变更通知) 主动配置。
     *
     * @param groupID 群 ID
     * @param v2TIMGroupMemberChangeInfoList 被修改的群成员信息
     */
    virtual void OnMemberInfoChanged(
        const V2TIMString &groupID,
        const V2TIMGroupMemberChangeInfoVector &v2TIMGroupMemberChangeInfoList) {}

    /**
     * 创建群（主要用于多端同步）
     *
     * @param groupID 群 ID
     */
    virtual void OnGroupCreated(const V2TIMString &groupID) {}

    /**
     * 群被解散了（全员能收到）
     *
     * @param groupID 群 ID
     * @param opUser  处理人
     */
    virtual void OnGroupDismissed(const V2TIMString &groupID, const V2TIMGroupMemberInfo &opUser) {}

    /**
     * 群被回收（全员能收到）
     *
     * @param groupID 群 ID
     * @param opUser  处理人
     */
    virtual void OnGroupRecycled(const V2TIMString &groupID, const V2TIMGroupMemberInfo &opUser) {}

    /**
     * 群信息被修改（全员能收到）
     * 以下字段的修改可能会引发该通知 groupName & introduction & notification & faceUrl & owner & allMute & custom
     * 控制指定字段 下发通知/存漫游 请前往 [控制台](https://console.cloud.tencent.com/im) (功能配置 -> 群组配置 -> 群系统通知配置 -> 群资料变更通知) 主动配置。
     *
     * @param changeInfos 修改的群信息
     */
    virtual void OnGroupInfoChanged(const V2TIMString &groupID,
                                    const V2TIMGroupChangeInfoVector &changeInfos) {}

    /**
     * 收到群属性更新的回调
     *
     * @param groupID           群 ID
     * @param groupAttributeMap 群的所有属性
     */
    virtual void OnGroupAttributeChanged(const V2TIMString &groupID,
                                         const V2TIMGroupAttributeMap &groupAttributeMap) {}

    /**
     * 某个已加入的群的计数器被修改了（全员能收到）
     *
     * @param groupID  群 ID
     * @param key 当前变更的群计数器的 key
     * @param newValue 变更之后的 value
     */
    virtual void OnGroupCounterChanged(const V2TIMString &groupID,
                                       const V2TIMString &key, int64_t newValue) {}

    /**
     * 有新的加群请求（只有群主或管理员会收到）
     *
     * @param groupID  群 ID
     * @param member   申请人
     * @param opReason 申请原因
     */
    virtual void OnReceiveJoinApplication(const V2TIMString &groupID,
                                          const V2TIMGroupMemberInfo &member,
                                          const V2TIMString &opReason) {}

    /**
     * 加群请求已经被群主或管理员处理了（只有申请人能够收到）
     *
     * @param groupID     群 ID
     * @param opUser      处理人
     * @param isAgreeJoin 是否同意加群
     * @param opReason    处理原因
     */
    virtual void OnApplicationProcessed(const V2TIMString &groupID,
                                        const V2TIMGroupMemberInfo &opUser, bool isAgreeJoin,
                                        const V2TIMString &opReason) {}

    /**
     * 指定管理员身份
     *
     * @param groupID    群 ID
     * @param opUser     处理人
     * @param memberList 被处理的群成员
     */
    virtual void OnGrantAdministrator(const V2TIMString &groupID,
                                      const V2TIMGroupMemberInfo &opUser,
                                      const V2TIMGroupMemberInfoVector &memberList) {}

    /**
     * 取消管理员身份
     *
     * @param groupID    群 ID
     * @param opUser     处理人
     * @param memberList 被处理的群成员
     */
    virtual void OnRevokeAdministrator(const V2TIMString &groupID,
                                       const V2TIMGroupMemberInfo &opUser,
                                       const V2TIMGroupMemberInfoVector &memberList) {}

    /**
     * 主动退出群组（主要用于多端同步，直播群（AVChatRoom）不支持）
     *
     * @param groupID 群 ID
     */
    virtual void OnQuitFromGroup(const V2TIMString &groupID) {}

    /**
     * 收到 RESTAPI 下发的自定义系统消息
     *
     * @param groupID    群 ID
     * @param customData 自定义数据
     */
    virtual void OnReceiveRESTCustomData(const V2TIMString &groupID,
                                         const V2TIMBuffer &customData) {}

    /**
     * 话题创建
     *
     * @param topicID 话题 ID
     */
    virtual void OnTopicCreated(const V2TIMString &groupID, const V2TIMString &topicID) {}

    /**
     * 话题被删除
     * @param groupID 话题所属的社群 ID
     * @param topicIDList 话题列表
     */
    virtual void OnTopicDeleted(const V2TIMString &groupID, const V2TIMStringVector &topicIDList) {}

    /**
    * 话题更新
    * @param topicInfo 话题信息
    */
    virtual void OnTopicChanged(const V2TIMString &groupID, const V2TIMTopicInfo &topicInfo) {}
};

/////////////////////////////////////////////////////////////////////////////////
//
//                      （五）日志监听类
//
/////////////////////////////////////////////////////////////////////////////////

class TIM_API V2TIMLogListener {
public:
    V2TIMLogListener();

    virtual ~V2TIMLogListener();

    /**
     * 14.1 有日志打印时的回调
     *
     * @param logLevel 日志等级, 参见 V2TIMLogLevel
     * @param logContent 日志内容
     */
    virtual void OnLog(V2TIMLogLevel logLevel, const V2TIMString &logContent) {}
};

/////////////////////////////////////////////////////////////////////////////////
//
//                      （六）会话事件的监听类
//
/////////////////////////////////////////////////////////////////////////////////

class TIM_API V2TIMConversationListener {
public:
    V2TIMConversationListener();

    virtual ~V2TIMConversationListener();

    /**
     * 同步服务器会话开始，SDK
     * 会在登录成功或者断网重连后自动同步服务器会话，您可以监听这个事件做一些 UI 进度展示操作。
     */
    virtual void OnSyncServerStart() {}

    /**
     * 同步服务器会话完成，如果会话有变更，会通过 OnNewConversation | OnConversationChanged
     * 回调告知客户
     */
    virtual void OnSyncServerFinish() {}

    /**
     * 同步服务器会话失败
     */
    virtual void OnSyncServerFailed() {}

    /**
     * 有新的会话（比如收到一个新同事发来的单聊消息、或者被拉入了一个新的群组中），可以根据会话的
     * lastMessage -> timestamp 重新对会话列表做排序
     *
     * @param conversationList 会话列表
     */
    virtual void OnNewConversation(const V2TIMConversationVector &conversationList) {}

    /**
     * 某些会话的关键信息发生变化（未读计数发生变化、最后一条消息被更新等等），可以根据会话的
     * lastMessage -> timestamp 重新对会话列表做排序
     *
     * @param conversationList 会话列表
     */
    virtual void OnConversationChanged(const V2TIMConversationVector &conversationList) {}

    /**
     * 全部会话未读总数变更的通知（5.3.425 及以上版本支持）
     *
     * @note
     *  - 当您调用 GetTotalUnreadMessageCount 获取全部会话未读总数以后，任意会话的未读数发生变化时，SDK 都会通过该回调把最新的未读总数通知给您。
     *  - 未读总数会减去设置为免打扰的会话的未读数，即消息接收选项设置为
     *  V2TIM_NOT_RECEIVE_MESSAGE  or V2TIM_RECEIVE_NOT_NOTIFY_MESSAGE 的会话。
     */
    virtual void OnTotalUnreadMessageCountChanged(uint64_t totalUnreadCount) {}

    /**
     * 根据 filter 过滤的未读消息总数变更通知（7.0 及以上版本支持）
     * @note
     *  - 您可以调用 SubscribeUnreadMessageCountByFilter 注册监听指定 filter 下的未读总数变化，SDK 通过这个回调把最新的未读总数通知给您。
     *  - 您可以注册监听多个不同 filter 下的未读总数变化，这个回调的 filter 参数就是注册监听时指定的 filter，该 filter 携带了 conversationType、conversationGroup 和 markType 三个字段，通过判断这三字段是不是都相同，来区分出不同的 filter。
     *  - 未读总数会减去设置为免打扰的会话的未读数，即消息接收选项设置为
     *  V2TIM_NOT_RECEIVE_MESSAGE  or V2TIM_RECEIVE_NOT_NOTIFY_MESSAGE 的会话。
     */
    virtual void OnUnreadMessageCountChangedByFilter(const V2TIMConversationListFilter &filter, uint64_t totalUnreadCount) {}

    /////////////////////////////////////////////////////////////////////////////////
    //
    //        会话分组相关通知（从 6.5 版本开始支持）
    //
    /////////////////////////////////////////////////////////////////////////////////

    /**
     * 会话分组被创建
     */
    virtual void OnConversationGroupCreated(const V2TIMString &groupName,
                                            const V2TIMConversationVector &conversationList) {}

    /**
     * 会话分组被删除
     */
    virtual void OnConversationGroupDeleted(const V2TIMString &groupName) {}

    /**
     * 会话分组名变更
     */
    virtual void OnConversationGroupNameChanged(const V2TIMString &oldName, const V2TIMString &newName) {}

    /**
     * 会话分组新增会话
     */
    virtual void OnConversationsAddedToGroup(const V2TIMString &groupName,
                                             const V2TIMConversationVector &conversationList) {}

    /**
     * 会话分组删除会话
     */
    virtual void OnConversationsDeletedFromGroup(const V2TIMString &groupName,
                                                 const V2TIMConversationVector &conversationList) {}
};

/////////////////////////////////////////////////////////////////////////////////
//
//                      （七）关系链事件的监听类
//
/////////////////////////////////////////////////////////////////////////////////

class TIM_API V2TIMFriendshipListener {
public:
    V2TIMFriendshipListener();

    virtual ~V2TIMFriendshipListener();

    /**
     * 好友申请新增通知，两种情况会收到这个回调：
     * 1. 自己申请加别人好友
     * 2. 别人申请加自己好友
     */
    virtual void OnFriendApplicationListAdded(const V2TIMFriendApplicationVector &applicationList) {}

    /**
     * 好友申请删除通知，四种情况会收到这个回调
     * 1. 调用 DeleteFriendApplication 主动删除好友申请
     * 2. 调用 RefuseFriendApplication 拒绝好友申请
     * 3. 调用 AcceptFriendApplication 同意好友申请且同意类型为 V2TIM_FRIEND_ACCEPT_AGREE 时
     * 4. 申请加别人好友被拒绝
     */
    virtual void OnFriendApplicationListDeleted(const V2TIMStringVector &userIDList) {}

    /**
     * 好友申请已读通知，如果调用 setFriendApplicationRead
     * 设置好友申请列表已读，会收到这个回调（主要用于多端同步）
     */
    virtual void OnFriendApplicationListRead() {}

    /**
     * 好友新增通知
     */
    virtual void OnFriendListAdded(const V2TIMFriendInfoVector &userIDList) {}

    /**
     * 好友删除通知，，两种情况会收到这个回调：
     * 1. 自己删除好友（单向和双向删除都会收到回调）
     * 2. 好友把自己删除（双向删除会收到）
     */
    virtual void OnFriendListDeleted(const V2TIMStringVector &userIDList) {}

    /**
     * 黑名单新增通知
     */
    virtual void OnBlackListAdded(const V2TIMFriendInfoVector &infoList) {}

    /**
     * 黑名单删除通知
     */
    virtual void OnBlackListDeleted(const V2TIMStringVector &userIDList) {}

    /**
     * 好友资料更新通知
     */
    virtual void OnFriendInfoChanged(const V2TIMFriendInfoVector &infoList) {}
};

/////////////////////////////////////////////////////////////////////////////////
//
//                      （八）信令事件监听类
//
/////////////////////////////////////////////////////////////////////////////////

class TIM_API V2TIMSignalingListener {
public:
    V2TIMSignalingListener();

    virtual ~V2TIMSignalingListener();

    /**
     * 收到邀请
     */
    virtual void OnReceiveNewInvitation(const V2TIMString &inviteID, const V2TIMString &inviter,
                                        const V2TIMString &groupID,
                                        const V2TIMStringVector &inviteeList,
                                        const V2TIMString &data) {}

    /**
     * 被邀请者接受邀请
     */
    virtual void OnInviteeAccepted(const V2TIMString &inviteID, const V2TIMString &invitee,
                                   const V2TIMString &data) {}

    /**
     * 被邀请者拒绝邀请
     */
    virtual void OnInviteeRejected(const V2TIMString &inviteID, const V2TIMString &invitee,
                                   const V2TIMString &data) {}

    /**
     * 邀请被取消
     */
    virtual void OnInvitationCancelled(const V2TIMString &inviteID, const V2TIMString &inviter,
                                       const V2TIMString &data) {}

    /**
     * 邀请超时
     */
    virtual void OnInvitationTimeout(const V2TIMString &inviteID,
                                     const V2TIMStringVector &inviteeList) {}

    /**
     * 邀请被修改（6.7 及其以上版本支持）
     */
    virtual void OnInvitationModified(const V2TIMString &inviteID, const V2TIMString &data) {}
};

#endif /* __V2TIM_LISTENER_H__ */
