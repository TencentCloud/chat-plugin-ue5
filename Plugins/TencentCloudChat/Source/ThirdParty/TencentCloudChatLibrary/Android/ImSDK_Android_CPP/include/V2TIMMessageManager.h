// Copyright (c) 2021 Tencent. All rights reserved.

/////////////////////////////////////////////////////////////////////
//
//                     腾讯云通信服务 IMSDK
//
//          消息高级接口，里面包含了所有高级消息的创建、收发逻辑
//
/////////////////////////////////////////////////////////////////////

#ifndef __V2TIM_MESSAGE_MANAGER_H__
#define __V2TIM_MESSAGE_MANAGER_H__

#include "V2TIMCallback.h"
#include "V2TIMDefine.h"
#include "V2TIMListener.h"
#include "V2TIMMessage.h"

class V2TIMMessageManager {
public:
    virtual ~V2TIMMessageManager() {}

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                         监听 - 高级（图片、语音、视频等）消息
    //
    /////////////////////////////////////////////////////////////////////////////////

    /**
     * 1.1 添加高级消息的事件监听器
     */
    virtual void AddAdvancedMsgListener(V2TIMAdvancedMsgListener *listener) = 0;

    /**
     * 1.2 移除高级消息监听器
     */
    virtual void RemoveAdvancedMsgListener(V2TIMAdvancedMsgListener *listener) = 0;

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                         创建 - 高级（图片、语音、视频等）消息
    //
    /////////////////////////////////////////////////////////////////////////////////

    /**
     * 2.1 创建文本消息
     */
    virtual V2TIMMessage CreateTextMessage(const V2TIMString &text) = 0;

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
    virtual V2TIMMessage CreateTextAtMessage(const V2TIMString &text,
                                             const V2TIMStringVector &atUserList) = 0;

    /**
     * 2.3 创建自定义消息
     */
    virtual V2TIMMessage CreateCustomMessage(const V2TIMBuffer &data) = 0;

    /**
     * 2.4 创建自定义消息
     *
     * @param description 自定义消息描述信息，做离线Push时文本展示。
     * @param extension 离线 Push 时扩展字段信息。
     */
    virtual V2TIMMessage CreateCustomMessage(const V2TIMBuffer &data,
                                             const V2TIMString &description,
                                             const V2TIMString &extension) = 0;

    /**
     * 2.5 创建图片消息（图片最大支持 28 MB）
     */
    virtual V2TIMMessage CreateImageMessage(const V2TIMString &imagePath) = 0;

    /**
     * 2.6 创建语音消息（语音最大支持 28 MB）
     *
     * @param duration  语音时长，单位 s
     */
    virtual V2TIMMessage CreateSoundMessage(const V2TIMString &soundPath, uint32_t duration) = 0;

    /**
     * 2.7 创建视频消息（视频最大支持 100 MB）
     *
     * @param type             视频类型，如 mp4 mov 等
     * @param duration         视频时长，单位 s
     * @param snapshotPath     视频封面图片路径
     */
    virtual V2TIMMessage CreateVideoMessage(const V2TIMString &videoFilePath,
                                            const V2TIMString &type, uint32_t duration,
                                            const V2TIMString &snapshotPath) = 0;

    /**
     * 2.8 创建文件消息（文件最大支持 100 MB）
     */
    virtual V2TIMMessage CreateFileMessage(const V2TIMString &filePath,
                                           const V2TIMString &fileName) = 0;

    /**
     * 2.9 创建地理位置消息
     */
    virtual V2TIMMessage CreateLocationMessage(const V2TIMString &desc, double longitude,
                                               double latitude) = 0;

    /**
     * 2.10 创建表情消息
     *
     * SDK 并不提供表情包，如果开发者有表情包，可使用 index 存储表情在表情包中的索引，
     * 或者直接使用 data 存储表情二进制信息以及字符串 key，都由用户自定义，SDK 内部只做透传。
     *
     * @param index 表情索引
     * @param data  自定义数据
     */
    virtual V2TIMMessage CreateFaceMessage(uint32_t index, const V2TIMBuffer &data) = 0;

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
    virtual V2TIMMessage CreateMergerMessage(const V2TIMMessageVector &messageList,
                                             const V2TIMString &title,
                                             const V2TIMStringVector &abstractList,
                                             const V2TIMString &compatibleText) = 0;

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
    virtual V2TIMMessage CreateForwardMessage(const V2TIMMessage &message) = 0;

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
    virtual V2TIMMessage CreateTargetedGroupMessage(const V2TIMMessage &message, const V2TIMStringVector &receiverList) = 0;
    
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
    virtual V2TIMMessage CreateAtSignedGroupMessage(const V2TIMMessage &message, const V2TIMStringVector &atUserList) = 0;


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
    virtual V2TIMString SendMessage(V2TIMMessage &message, const V2TIMString &receiver,
                                    const V2TIMString &groupID, V2TIMMessagePriority priority,
                                    bool onlineUserOnly,
                                    const V2TIMOfflinePushInfo &offlinePushInfo,
                                    V2TIMSendCallback *callback) = 0;

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
    virtual void SetC2CReceiveMessageOpt(const V2TIMStringVector &userIDList,
                                         V2TIMReceiveMessageOpt opt, V2TIMCallback *callback) = 0;

    /**
     *  4.2 查询针对某个用户的 C2C 消息接收选项
     *  <p>5.3.425 及以上版本支持
     */
    virtual void GetC2CReceiveMessageOpt(
        const V2TIMStringVector &userIDList,
        V2TIMValueCallback<V2TIMReceiveMessageOptInfoVector> *callback) = 0;

    /**
     *  4.3 设置群消息的接收选项
     *
     * @param opt      三种类型的消息接收选项：
     *                 V2TIMMessage.V2TIM_RECEIVE_MESSAGE：在线正常接收消息，离线时会有厂商的离线推送通知
     *                 V2TIMMessage.V2TIM_NOT_RECEIVE_MESSAGE：不会接收到群消息
     *                 V2TIMMessage.V2TIM_RECEIVE_NOT_NOTIFY_MESSAGE：在线正常接收消息，离线不会有推送通知
     */
    virtual void SetGroupReceiveMessageOpt(const V2TIMString &groupID, V2TIMReceiveMessageOpt opt,
                                           V2TIMCallback *callback) = 0;

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
    virtual void GetHistoryMessageList(const V2TIMMessageListGetOption &option,
                                       V2TIMValueCallback<V2TIMMessageVector> *callback) = 0;

    /**
     * 5.2 撤回消息
     *
     * @note 请注意：
     *  - 撤回消息的时间限制默认 2 minutes，超过 2 minutes 的消息不能撤回，您也可以在 [控制台](https://console.cloud.tencent.com/im)（功能配置 -> 登录与消息 ->
     * 消息撤回设置）自定义撤回时间限制。
     *  - 仅支持单聊和群组中发送的普通消息，无法撤销 onlineUserOnly 为 true 即仅在线用户才能收到的消息，也无法撤销直播群（AVChatRoom）中的消息。
     *  - 如果发送方撤回消息，已经收到消息的一方会收到 V2TIMAdvancedMsgListener::OnRecvMessageRevoked 回调。
     */
    virtual void RevokeMessage(const V2TIMMessage &message, V2TIMCallback *callback) = 0;

    /**
     *  5.3 消息变更
     *
     *  @note 请注意：
     *  - 如果消息修改成功，自己和对端用户（C2C）或群组成员（Group）都会收到 OnRecvMessageModified 回调。
     *  - 如果在修改消息过程中，消息已经被其他人修改，callback 会返回 ERR_SDK_MSG_MODIFY_CONFLICT 错误。
     *  - 消息无论修改成功或则失败，callback 都会返回最新的消息对象。
     */
    virtual void ModifyMessage(const V2TIMMessage &message, V2TIMCompleteCallback<V2TIMMessage> *callback) = 0;

    /**
     * 5.4 删除本地及云端的消息
     *
     *  @note 该接口删除本地及云端的消息，且无法恢复。需要注意的是：
     *  - 一次最多只能删除 30 条消息
     *  - 要删除的消息必须属于同一会话
     *  - 一秒钟最多只能调用一次该接口
     *  -
     * 如果该账号在其他设备上拉取过这些消息，那么调用该接口删除后，这些消息仍然会保存在那些设备上，即删除消息不支持多端同步。
     */
    virtual void DeleteMessages(const V2TIMMessageVector &messages, V2TIMCallback *callback) = 0;

    /**
     * 5.5 清空单聊本地及云端的消息（不删除会话）
     * <p> 5.4.666 及以上版本支持
     *
     * @note 请注意：
     * - 会话内的消息在本地删除的同时，在服务器也会同步删除。
     *
     */
    virtual void ClearC2CHistoryMessage(const V2TIMString &userID, V2TIMCallback *callback) = 0;

    /**
     * 5.6 清空群聊本地及云端的消息（不删除会话）
     * <p> 5.4.666 及以上版本支持
     *
     * @note 请注意：
     * - 会话内的消息在本地删除的同时，在服务器也会同步删除。
     */
    virtual void ClearGroupHistoryMessage(const V2TIMString &groupID, V2TIMCallback *callback) = 0;

    /**
     * 5.7 向群组消息列表中添加一条消息
     *
     * 该接口主要用于满足向群组聊天会话中插入一些提示性消息的需求，比如“您已经退出该群”，这类消息有展示
     * 在聊天消息区的需求，但并没有发送给其他人的必要。
     * 所以 InsertGroupMessageToLocalStorage() 相当于一个被禁用了网络发送能力的 SendMessage() 接口。
     *
     * @return msgID 消息唯一标识
     *
     * @note 通过该接口 save 的消息只存本地，程序卸载后会丢失。
     */
    virtual V2TIMString InsertGroupMessageToLocalStorage(
        V2TIMMessage &message, const V2TIMString &groupID, const V2TIMString &sender,
        V2TIMValueCallback<V2TIMMessage> *callback) = 0;

    /**
     *  5.8 向C2C消息列表中添加一条消息
     *
     *  该接口主要用于满足向C2C聊天会话中插入一些提示性消息的需求，比如“您已成功发送消息”，这类消息有展示
     *  在聊天消息去的需求，但并没有发送给对方的必要。
     *  所以 InsertC2CMessageToLocalStorage()相当于一个被禁用了网络发送能力的 SendMessage() 接口。
     *
     *  @return msgID 消息唯一表示
     *  @note 通过该接口 save 的消息只存本地，程序卸载后会丢失。
     */
    virtual V2TIMString InsertC2CMessageToLocalStorage(
        V2TIMMessage &message, const V2TIMString &userID, const V2TIMString &sender,
        V2TIMValueCallback<V2TIMMessage> *callback) = 0;

    /**
     * 5.9 根据 messageID 查询指定会话中的本地消息
     * @param messageIDList 消息 ID 列表
     */
    virtual void FindMessages(const V2TIMStringVector &messageIDList,
                              V2TIMValueCallback<V2TIMMessageVector> *callback) = 0;

    /**
     * 5.10 搜索本地消息（5.4.666 及以上版本支持，需要您购买旗舰版套餐）
     * @param searchParam 消息搜索参数，详见 V2TIMMessageSearchParam 的定义
     * @note 该功能为 IM 旗舰版功能，[购买旗舰版套餐包](https://buy.cloud.tencent.com/avc?from=17474)后可使用，详见[价格说明](https://cloud.tencent.com/document/product/269/11673?from=17176#.E5.9F.BA.E7.A1.80.E6.9C.8D.E5.8A.A1.E8.AF.A6.E6.83.85)
     */
    virtual void SearchLocalMessages(const V2TIMMessageSearchParam &searchParam,
                                     V2TIMValueCallback<V2TIMMessageSearchResult> *callback) = 0;

    /**
     *  5.11 发送消息已读回执（6.1 及其以上版本支持）
     * 
     * @note 请注意：
     * - 该功能为旗舰版功能，[购买旗舰版套餐包](https://buy.cloud.tencent.com/avc?from=17485)后可使用，详见[价格说明](https://cloud.tencent.com/document/product/269/11673?from=17221#.E5.9F.BA.E7.A1.80.E6.9C.8D.E5.8A.A1.E8.AF.A6.E6.83.85)。
     * - 向群消息发送已读回执，需要您先到控制台打开对应的开关，详情参考文档 [群消息已读回执](https://cloud.tencent.com/document/product/269/75343#.E8.AE.BE.E7.BD.AE.E6.94.AF.E6.8C.81.E5.B7.B2.E8.AF.BB.E5.9B.9E.E6.89.A7.E7.9A.84.E7.BE.A4.E7.B1.BB.E5.9E.8B) 。
     * - messageList 里的消息必须在同一个会话中。
     * - 该接口调用成功后，会话未读数不会变化，消息发送者会收到 onRecvMessageReadReceipts 回调，回调里面会携带消息的最新已读信息。
     */
    virtual void SendMessageReadReceipts(const V2TIMMessageVector &messageList, V2TIMCallback *callback) = 0;

    /**
     *  5.12 获取消息已读回执（6.1 及其以上版本支持）
     * @param messageList 消息列表
     *
     * @note 请注意：
     * - 该功能为旗舰版功能，[购买旗舰版套餐包](https://buy.cloud.tencent.com/avc?from=17485)后可使用，详见[价格说明](https://cloud.tencent.com/document/product/269/11673?from=17221#.E5.9F.BA.E7.A1.80.E6.9C.8D.E5.8A.A1.E8.AF.A6.E6.83.85)。
     * - 获取群消息已读回执，需要您先到控制台打开对应的开关，详情参考文档 [群消息已读回执](https://cloud.tencent.com/document/product/269/75343#.E8.AE.BE.E7.BD.AE.E6.94.AF.E6.8C.81.E5.B7.B2.E8.AF.BB.E5.9B.9E.E6.89.A7.E7.9A.84.E7.BE.A4.E7.B1.BB.E5.9E.8B) 。
     * - messageList 里的消息必须在同一个会话中。
     */
    virtual void GetMessageReadReceipts(const V2TIMMessageVector &messageList, V2TIMValueCallback<V2TIMMessageReceiptVector> *callback) = 0;

    /**
     * 5.13 获取群消息已读群成员列表（6.1 及其以上版本支持）
     * @param message 群消息
     * @param filter  指定拉取已读或未读群成员列表。
     * @param nextSeq 分页拉取的游标，第一次默认取传 0，后续分页拉取时，传上一次分页拉取成功回调里的 nextSeq。
     * @param count   分页拉取的个数，最多支持 100 个。
     *
     * @note 请注意：
     * - 该功能为旗舰版功能，[购买旗舰版套餐包](https://buy.cloud.tencent.com/avc?from=17485)后可使用，详见[价格说明](https://cloud.tencent.com/document/product/269/11673?from=17221#.E5.9F.BA.E7.A1.80.E6.9C.8D.E5.8A.A1.E8.AF.A6.E6.83.85)。
     * - 使用该功能之前，请您先到控制台打开对应的开关，详情参考文档 [群消息已读回执](https://cloud.tencent.com/document/product/269/75343#.E8.AE.BE.E7.BD.AE.E6.94.AF.E6.8C.81.E5.B7.B2.E8.AF.BB.E5.9B.9E.E6.89.A7.E7.9A.84.E7.BE.A4.E7.B1.BB.E5.9E.8B) 。
     */
    virtual void GetGroupMessageReadMemberList(const V2TIMMessage &message, V2TIMGroupMessageReadMembersFilter filter, uint64_t nextSeq, uint32_t count, V2TIMValueCallback<V2TIMGroupMessageReadMemberList> *callback) = 0;

    /**
     * 5.14 设置消息扩展（6.7 及其以上版本支持，需要您购买旗舰版套餐）
     * @param message 消息对象，消息需满足三个条件：1、消息发送前需设置 supportMessageExtension 为 true，2、消息必须是发送成功的状态，3、消息不能是社群（Community）和直播群（AVChatRoom）消息。
     * @param extensions 扩展信息，如果扩展 key 已经存在，则修改扩展的 value 信息，如果扩展 key 不存在，则新增扩展。
     *
     * @note
     * - 扩展 key 最大支持 100 字节，扩展 value 最大支持 1KB，单次最多支持设置 20 个扩展，单条消息最多可设置 300 个扩展。
     * - 当多个用户同时设置或删除同一个扩展 key 时，只有第一个用户可以执行成功，其它用户会收到 23001 错误码和最新的扩展信息，在收到错误码和扩展信息后，请按需重新发起设置操作。
     * - 我们强烈建议不同的用户设置不同的扩展 key，这样大部分场景都不会冲突，比如投票、接龙、问卷调查，都可以把自己的 userID 作为扩展 key。
     */
    virtual void SetMessageExtensions(const V2TIMMessage &message, const V2TIMMessageExtensionVector &extensions, V2TIMValueCallback<V2TIMMessageExtensionResultVector> *callback) = 0;

    /**
     * 5.15 获取消息扩展（6.7 及其以上版本支持，需要您购买旗舰版套餐）
     */
    virtual void GetMessageExtensions(const V2TIMMessage &message, V2TIMValueCallback<V2TIMMessageExtensionVector> *callback) = 0;

    /**
     * 5.16 删除消息扩展（6.7 及其以上版本支持，需要您购买旗舰版套餐）
     * @param keys 消息扩展 key 列表, 单次最大支持删除 20 个消息扩展，如果设置为 nil ，表示删除所有消息扩展
     *
     * @note
     * - 当多个用户同时设置或删除同一个扩展 key 时，只有第一个用户可以执行成功，其它用户会收到 23001 错误码和最新的扩展信息，在收到错误码和扩展信息后，请按需重新发起删除操作。
     */
    virtual void DeleteMessageExtensions(const V2TIMMessage &message, const V2TIMStringVector &keys, V2TIMValueCallback<V2TIMMessageExtensionResultVector> *callback) = 0;

    /**
     *  5.17 翻译文本消息
     *
     *  @param sourceTextList 待翻译文本数组。
     *  @param sourceLanguage 源语言。可以设置为特定语言或 ”auto“。“auto“ 表示自动识别源语言。传空默认为 ”auto“。
     *  @param targetLanguage 目标语言。支持的目标语言有多种，例如：英语-“en“，简体中文-”zh“，法语-”fr“，德语-”de“等。详情请参考文档：[文本翻译语言支持](https://cloud.tencent.com/document/product/269/85380#.E6.96.87.E6.9C.AC.E7.BF.BB.E8.AF.91.E8.AF.AD.E8.A8.80.E6.94.AF.E6.8C.81)。
     *  @param callback 翻译结果回调。其中 result 的 key 为待翻译文本, value 为翻译后文本。
     */
    virtual void TranslateText(const V2TIMStringVector &sourceTextList,
                               const V2TIMString &sourceLanguage, const V2TIMString &targetLanguage,
                               V2TIMValueCallback<V2TIMStringToV2TIMStringMap> *callback) = 0;

    /**
     * 5.18 标记单聊会话已读（待废弃接口，请使用 CleanConversationUnreadMessageCount 接口）
     * @note 请注意：
     *  - 该接口调用成功后，自己的未读数会清 0，对端用户会收到 OnRecvC2CReadReceipt 回调，回调里面会携带标记会话已读的时间。
     *  - 从 5.8 版本开始，当 userID 为 nil 时，标记所有单聊会话为已读状态。
     */
    virtual void MarkC2CMessageAsRead(const V2TIMString &userID, V2TIMCallback *callback) = 0;

    /**
     * 5.19 标记群组会话已读（待废弃接口，请使用 CleanConversationUnreadMessageCount 接口）
      *  @note 请注意：
      *  - 该接口调用成功后，自己的未读数会清 0。
      *  - 从 5.8 版本开始，当 groupID 为 nil 时，标记所有群组会话为已读状态。
     */
    virtual void MarkGroupMessageAsRead(const V2TIMString &groupID, V2TIMCallback *callback) = 0;

    /**
     * 5.20 标记所有会话为已读（待废弃接口，请使用 CleanConversationUnreadMessageCount 接口）
     */
    virtual void MarkAllMessageAsRead(V2TIMCallback *callback) = 0;
};

#endif  // __V2TIM_MESSAGE_MANAGER_H__
