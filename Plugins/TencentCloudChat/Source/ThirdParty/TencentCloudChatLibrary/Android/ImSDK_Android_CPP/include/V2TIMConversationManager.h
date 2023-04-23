// Copyright (c) 2021 Tencent. All rights reserved.

/////////////////////////////////////////////////////////////////////
//
//                     腾讯云通信服务 IMSDK
//
//          会话接口，里面包含了会话的获取，删除，更新的逻辑
//
/////////////////////////////////////////////////////////////////////

#ifndef __V2TIM_CONVERSATION_MANAGER_H__
#define __V2TIM_CONVERSATION_MANAGER_H__

#include "V2TIMCallback.h"
#include "V2TIMConversation.h"
#include "V2TIMDefine.h"
#include "V2TIMListener.h"

class TIM_API V2TIMConversationManager {
public:
    virtual ~V2TIMConversationManager() {}

    /**
     * 1.1 添加会话监听器
     */
    virtual void AddConversationListener(V2TIMConversationListener* listener) = 0;

    /**
     * 1.2 移除会话监听器
     */
    virtual void RemoveConversationListener(V2TIMConversationListener* listener) = 0;

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
    virtual void GetConversationList(uint64_t nextSeq, uint32_t count,
                                     V2TIMValueCallback<V2TIMConversationResult>* callback) = 0;

    /**
     * 1.4 获取单个会话
     *
     * @param conversationID  会话唯一 ID，C2C 单聊组成方式为: "c2c_userID"：
     * 群聊组成方式为: "group_groupID")
     */
    virtual void GetConversation(const V2TIMString& conversationID,
                                 V2TIMValueCallback<V2TIMConversation>* callback) = 0;

    /**
     * 1.5 获取指定会话列表
     *
     * @param conversationIDList 会话唯一 ID，C2C 单聊组成方式为: "c2c_userID"：
     * 群聊组成方式为: "group_groupID")
     */
    virtual void GetConversationList(const V2TIMStringVector& conversationIDList,
                                     V2TIMValueCallback<V2TIMConversationVector>* callback) = 0;

    /**
     *  1.6 获取会话列表高级接口（从 6.5 版本开始支持）
     *
     * @param filter 会话 filter
     * @param nextSeq 分页拉取的游标
     * @param count 分页拉取的个数
     *
     */
    virtual void GetConversationListByFilter(const V2TIMConversationListFilter &filter,
                                             uint64_t nextSeq, uint32_t count,
                                             V2TIMValueCallback<V2TIMConversationResult>* callback) = 0;

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
    virtual void DeleteConversation(const V2TIMString& conversationID, V2TIMCallback* callback) = 0;

    /**
     * 1.8 删除会话列表（7.1 及以上版本支持）
     *
     * @param conversationIDList 会话唯一 ID 列表，C2C 单聊组成方式为: "c2c_userID"：群聊组成方式为: "group_groupID")
     * @param clearMessage 是否删除会话中的消息；设置为 false 时，保留会话消息；设置为 true 时，本地和服务器的消息会一起删除，并且不可恢复
     *
     * @note 请注意: 每次最多支持删除 100 个会话
     */
    virtual void DeleteConversationList(const V2TIMStringVector& conversationIDList, bool clearMessage,
                                        V2TIMValueCallback<V2TIMConversationOperationResultVector>* callback) = 0;

    /**
     * 1.9 设置会话草稿
     *
     * @param conversationID 会话唯一 ID，C2C 会话唯一 ID，C2C 单聊组成方式为: "c2c_userID"：
     * 群聊组成方式为: "group_groupID")
     *
     * 只在本地保存，不会存储 Server，不能多端同步，程序卸载重装会失效。
     *
     * @param draftText 草稿内容, 空字符串表示 则表示取消草稿
     */
    virtual void SetConversationDraft(const V2TIMString& conversationID,
                                      const V2TIMString& draftText, V2TIMCallback* callback) = 0;

    /**
     * 1.10 设置会话自定义数据（从 6.5 版本开始支持）
     *
     * @param customData 自定义数据，最大支持 256 bytes
     */
    virtual void SetConversationCustomData(const V2TIMStringVector &conversationIDList, const V2TIMBuffer &customData,
                                           V2TIMValueCallback<V2TIMConversationOperationResultVector>* callback) = 0;

    /**
     * 1.11 设置会话置顶（5.3.425 及以上版本支持）
     *
     * @param conversationID 会话唯一 ID，C2C 单聊组成方式为: "c2c_userID"：
     * 群聊组成方式为: "group_groupID")
     *
     * @param isPinned 是否置顶
     */
    virtual void PinConversation(const V2TIMString& conversationID, bool isPinned,
                                 V2TIMCallback* callback) = 0;

    /**
     * 1.12 标记会话（从 6.5 版本开始支持，需要您购买旗舰版套餐）
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
    virtual void MarkConversation(const V2TIMStringVector &conversationIDList, uint64_t markType, bool enableMark,
                                  V2TIMValueCallback<V2TIMConversationOperationResultVector>* callback) = 0;

    /**
     * 1.13 获取全部会话的未读总数（5.3.425 及以上版本支持）
     * @note
     *  - 调用该接口以后，任意会话的未读数发生变化时，SDK 都会给您抛 OnTotalUnreadMessageCountChanged 回调。
     *  - 未读总数会减去设置为免打扰的会话的未读数，即消息接收选项设置为
     *  V2TIM_NOT_RECEIVE_MESSAGE 或 V2TIM_RECEIVE_NOT_NOTIFY_MESSAGE 的会话。
     */
    virtual void GetTotalUnreadMessageCount(V2TIMValueCallback<uint64_t>* callback) = 0;

    /**
     * 1.14 获取按会话 filter 过滤的未读总数（7.0 及以上版本支持）
     *
     * @param filter 会话 filter
     *
     * @note
     *  - 未读总数会减去设置为免打扰的会话的未读数，即消息接收选项设置为
     *  V2TIM_NOT_RECEIVE_MESSAGE 或 V2TIM_RECEIVE_NOT_NOTIFY_MESSAGE 的会话。
     */
    virtual void GetUnreadMessageCountByFilter(const V2TIMConversationListFilter &filter,
        V2TIMValueCallback<uint64_t>* callback) = 0;

    /**
     *  1.15 注册监听指定 filter 的会话未读总数变化（7.0 及以上版本支持）
     *
     * @param filter 会话 filter
     *
     * @note
     *  - 当您调用这个接口以后，该 filter 下的未读数发生变化时，SDK 会给您抛 OnUnreadMessageCountChangedByFilter 回调。
     */
    virtual void SubscribeUnreadMessageCountByFilter(const V2TIMConversationListFilter &filter) = 0;

    /**
     *  1.16 取消监听指定 filter 的会话未读总数变化（7.0 及以上版本支持）
     *
     * @param filter 会话 filter
     *
     */
    virtual void UnsubscribeUnreadMessageCountByFilter(const V2TIMConversationListFilter &filter) = 0;

    /**  1.17 清理会话的未读消息计数（7.1 及以上版本支持）
     *
     * @param conversationID  会话唯一 ID， C2C 单聊组成方式：[NSString stringWithFormat:@"c2c_%@",userID]；群聊组成方式为 [NSString stringWithFormat:@"group_%@",groupID]
     * @param cleanTimestamp  清理时间戳，仅对单聊会话生效，指定清理哪一个 timestamp 之前的未读消息计数；当传入为 0 时，对应会话所有的未读消息将被清理，会话的未读数会清 0
     * @param cleanSequence  清理 sequence，仅对群聊会话生效，指定清理哪一个 sequence 之前的未读消息计数；当传入为 0 时，对应会话所有的未读消息将被清理，会话的未读数会清 0
     *
     * @note
     *  - 当您想清理所有单聊会话的未读消息计数，conversationID 请传入 "c2c"，即不指定具体的 userID；
     *  - 当您想清理所有群聊会话的未读消息计数，conversationID 请传入 "group"，即不指定具体的 groupID；
     *  - 当您想清理所有会话的未读消息计数，conversationID 请传入 ""；
     *  - 该接口调用成功后，SDK 会通过 onConversationChanged 回调将对应会话的最新未读数通知给您。
     */
    virtual void CleanConversationUnreadMessageCount(const V2TIMString& conversationID, uint64_t cleanTimestamp, uint64_t cleanSequence, V2TIMCallback* callback) = 0;

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
    virtual void CreateConversationGroup(const V2TIMString &groupName, const V2TIMStringVector &conversationIDList,
                                         V2TIMValueCallback<V2TIMConversationOperationResultVector>* callback) = 0;

    /**
     * 2.2 获取会话分组列表
     */
    virtual void GetConversationGroupList(V2TIMValueCallback<V2TIMStringVector>* callback) = 0;

    /**
     * 2.3 删除会话分组
     */
    virtual void DeleteConversationGroup(const V2TIMString &groupName, V2TIMCallback* callback) = 0;

    /**
     * 2.4 重命名会话分组
     *
     */
    virtual void RenameConversationGroup(const V2TIMString &oldName, const V2TIMString &newName,
                                         V2TIMCallback* callback) = 0;

    /**
     * 2.5 添加会话到一个会话分组
     */
    virtual void AddConversationsToGroup(const V2TIMString &groupName, const V2TIMStringVector &conversationIDList,
                                         V2TIMValueCallback<V2TIMConversationOperationResultVector>* callback) = 0;

    /**
     * 2.6 从一个会话分组中删除会话
     */
    virtual void DeleteConversationsFromGroup(const V2TIMString &groupName, const V2TIMStringVector &conversationIDList,
                                              V2TIMValueCallback<V2TIMConversationOperationResultVector>* callback) = 0;

};

#endif  // __V2TIM_CONVERSATION_MANAGER_H__
