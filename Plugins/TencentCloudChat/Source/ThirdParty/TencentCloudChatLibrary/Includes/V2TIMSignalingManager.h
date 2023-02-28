// Copyright (c) 2021 Tencent. All rights reserved.

/////////////////////////////////////////////////////////////////////
//
//                     腾讯云通信服务 IMSDK
//
//           消息高级接口，里面包含了所有高级消息的创建、收发逻辑
//
/////////////////////////////////////////////////////////////////////

#ifndef __V2TIM_SIGNALING_MANAGER_H__
#define __V2TIM_SIGNALING_MANAGER_H__

#include "V2TIMCallback.h"
#include "V2TIMDefine.h"
#include "V2TIMListener.h"
#include "V2TIMSignaling.h"

class TIM_API V2TIMSignalingManager {
public:
    virtual ~V2TIMSignalingManager() {}

    /**
     * 添加信令监听
     */
    virtual void AddSignalingListener(V2TIMSignalingListener* listener) = 0;

    /**
     * 移除信令监听
     */
    virtual void RemoveSignalingListener(V2TIMSignalingListener* listener) = 0;

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
    virtual V2TIMString Invite(const V2TIMString& invitee, const V2TIMString& data,
                               bool onlineUserOnly,
                               const V2TIMOfflinePushInfo& offlinePushInfo, int timeout,
                               V2TIMCallback* callback) = 0;

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
    virtual V2TIMString InviteInGroup(const V2TIMString& groupID,
                                      const V2TIMStringVector& inviteeList, const V2TIMString& data,
                                      bool onlineUserOnly, int timeout,
                                      V2TIMCallback* callback) = 0;

    /**
     * 邀请方取消邀请
     *
     * @param inviteID 邀请 ID
     *
     * @note 如果所有被邀请人都已经处理了当前邀请（包含超时），不能再取消当前邀请。
     */
    virtual void Cancel(const V2TIMString& inviteID, const V2TIMString& data,
                        V2TIMCallback* callback) = 0;

    /**
     * 接收方接收邀请
     *
     * @note 不能接受不是针对自己的邀请，请在收到 OnReceiveNewInvitation 回调的时候先判断
     * inviteeList 有没有自己，如果没有自己，不能 Accept 邀请。
     */
    virtual void Accept(const V2TIMString& inviteID, const V2TIMString& data,
                        V2TIMCallback* callback) = 0;

    /**
     * 接收方拒绝邀请
     *
     * @note 不能拒绝不是针对自己的邀请，请在收到 OnReceiveNewInvitation 回调的时候先判断
     * inviteeList 有没有自己，如果没有自己，不能 Reject 邀请。
     */
    virtual void Reject(const V2TIMString& inviteID, const V2TIMString& data,
                        V2TIMCallback* callback) = 0;

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
    virtual V2TIMSignalingInfo GetSignalingInfo(const V2TIMMessage& msg) = 0;

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
    virtual void AddInvitedSignaling(const V2TIMSignalingInfo& info, V2TIMCallback* callback) = 0;

    /**
     *  修改邀请信令（6.7 及其以上版本支持）
     *
     *  @note 仅支持修改邀请信令的自定义字段 data。只有在线用户才能收到的邀请信令不能被修改。
     */
    virtual void ModifyInvitation(const V2TIMString& inviteID, const V2TIMString& data,
                                  V2TIMCallback* callback) = 0;
};

#endif  // __V2TIM_SIGNALING_MANAGER_H__
