// Copyright (c) 2021 Tencent. All rights reserved.

/////////////////////////////////////////////////////////////////////
//
//                     腾讯云通信服务 IMSDK
//
//   群组高级接口，里面包含了群组的高级功能，比如群成员邀请、非群成员申请进群等操作接口。
//
/////////////////////////////////////////////////////////////////////

#ifndef __V2TIM_GROUP_MANAGER_H__
#define __V2TIM_GROUP_MANAGER_H__

#include "V2TIMCallback.h"
#include "V2TIMDefine.h"
#include "V2TIMGroup.h"
#include "V2TIMListener.h"

/**
 * ## 群组高级接口，包含了群组的高级功能，例如群成员邀请、非群成员申请进群等操作接口。
 */
class TIM_API V2TIMGroupManager {
public:
    virtual ~V2TIMGroupManager() {}

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
    virtual void CreateGroup(const V2TIMGroupInfo& info,
                             const V2TIMCreateGroupMemberInfoVector& memberList,
                             V2TIMValueCallback<V2TIMString>* callback) = 0;

    /**
     * 1.2 获取当前用户已经加入的群列表
     *
     * @note
     * - 直播群(AVChatRoom) 不支持该 API。
     * - 该接口有频限检测，SDK 限制调用频率为1 秒 10 次，超过限制后会报
     * ERR_SDK_COMM_API_CALL_FREQUENCY_LIMIT （7008）错误
     */
    virtual void GetJoinedGroupList(V2TIMValueCallback<V2TIMGroupInfoVector>* callback) = 0;

    /////////////////////////////////////////////////////////////////////////////////
    //                         群资料和高级设置项
    /////////////////////////////////////////////////////////////////////////////////

    /**
     *  2.1 拉取群资料
     *
     *  @param groupIDList 群组 ID 列表
     */
    virtual void GetGroupsInfo(const V2TIMStringVector& groupIDList,
                               V2TIMValueCallback<V2TIMGroupInfoResultVector>* callback) = 0;

    /**
     * 2.2 搜索群资料（5.4.666 及以上版本支持）
     *
     * @param searchParam 搜索参数
     * @note 该功能为 IM 旗舰版功能，[购买旗舰版套餐包](https://buy.cloud.tencent.com/avc?from=17474)后可使用，详见[价格说明](https://cloud.tencent.com/document/product/269/11673?from=17176#.E5.9F.BA.E7.A1.80.E6.9C.8D.E5.8A.A1.E8.AF.A6.E6.83.85)
     */
    virtual void SearchGroups(const V2TIMGroupSearchParam& searchParam,
                              V2TIMValueCallback<V2TIMGroupInfoVector>* callback) = 0;

    /**
     * 2.3 修改群资料
     */
    virtual void SetGroupInfo(const V2TIMGroupInfo& info, V2TIMCallback* callback) = 0;

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
    virtual void InitGroupAttributes(const V2TIMString& groupID,
                                     const V2TIMGroupAttributeMap& attributes,
                                     V2TIMCallback* callback) = 0;

    /**
     * 2.5 设置群属性。已有该群属性则更新其 value 值，没有该群属性则添加该属性。
     * @note
     *   - 6.7 及其以前版本，只支持 AVChatRoom 直播群；
     *   - 从 6.8 版本开始，同时支持 AVChatRoom、Public、Meeting、Work 四种群类型；
     *   - 从 7.0 版本开始，除了话题外，群属性支持所有的群类型。
     */
    virtual void SetGroupAttributes(const V2TIMString& groupID,
                                    const V2TIMGroupAttributeMap& attributes,
                                    V2TIMCallback* callback) = 0;

    /**
     * 2.6 删除指定群属性，keys 传大小为 0 的 V2TIMStringVector 则清空所有群属性。
     * @note
     *   - 6.7 及其以前版本，只支持 AVChatRoom 直播群；
     *   - 从 6.8 版本开始，同时支持 AVChatRoom、Public、Meeting、Work 四种群类型；
     *   - 从 7.0 版本开始，除了话题外，群属性支持所有的群类型。
     */
    virtual void DeleteGroupAttributes(const V2TIMString& groupID, const V2TIMStringVector& keys,
                                       V2TIMCallback* callback) = 0;

    /**
     * 2.7 获取指定群属性，keys 传 keys 传大小为 0 的 V2TIMStringVector 则获取所有群属性。
     * @note
     *   - 6.7 及其以前版本，只支持 AVChatRoom 直播群；
     *   - 从 6.8 版本开始，同时支持 AVChatRoom、Public、Meeting、Work 四种群类型；
     *   - 从 7.0 版本开始，除了话题外，群属性支持所有的群类型。
     */
    virtual void GetGroupAttributes(const V2TIMString& groupID, const V2TIMStringVector& keys,
                                    V2TIMValueCallback<V2TIMGroupAttributeMap>* callback) = 0;

    /**
     * 2.8 获取指定群在线人数
     *
     * @note 请注意：
     *   - 目前只支持：直播群（AVChatRoom）。
     *   - 该接口有频限检测，SDK 限制调用频率为60秒1次。
     */
    virtual void GetGroupOnlineMemberCount(const V2TIMString& groupID,
                                           V2TIMValueCallback<uint32_t>* callback) = 0;

    /**
     * 2.9 设置群计数器（7.0 及其以上版本支持）
     *
     * @note
     *  - 该计数器的 key 如果存在，则直接更新计数器的 value 值；如果不存在，则添加该计数器的 key-value；
     *  - 当群计数器设置成功后，在 succ 回调中会返回最终成功设置的群计数器信息；
     *  - 除了社群和话题，群计数器支持所有的群组类型。
     */
    virtual void SetGroupCounters(const V2TIMString& groupID, const V2TIMStringToInt64Map& counters,
                                  V2TIMValueCallback<V2TIMStringToInt64Map>* callback) = 0;

    /**
     * 2.10 获取群计数器（7.0 及其以上版本支持）
     *
     * @note
     *  - 如果 keys 为空，则表示获取群内的所有计数器；
     *  - 除了社群和话题，群计数器支持所有的群组类型。
     */
    virtual void GetGroupCounters(const V2TIMString& groupID, const V2TIMStringVector& keys,
                                  V2TIMValueCallback<V2TIMStringToInt64Map>* callback) = 0;

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
    virtual void IncreaseGroupCounter(const V2TIMString& groupID,
                                      const V2TIMString& key, int64_t value,
                                      V2TIMValueCallback<V2TIMStringToInt64Map>* callback) = 0;

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
    virtual void DecreaseGroupCounter(const V2TIMString& groupID,
                                      const V2TIMString& key, int64_t value,
                                      V2TIMValueCallback<V2TIMStringToInt64Map>* callback) = 0;

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
    virtual void GetGroupMemberList(const V2TIMString& groupID, uint32_t filter,
                                    uint64_t nextSeq,
                                    V2TIMValueCallback<V2TIMGroupMemberInfoResult>* callback) = 0;

    /**
     * 3.2 获取指定的群成员资料
     */
    virtual void GetGroupMembersInfo(
        const V2TIMString& groupID, V2TIMStringVector memberList,
        V2TIMValueCallback<V2TIMGroupMemberFullInfoVector>* callback) = 0;

    /**
     * 3.3 搜索群成员（5.4.666 及以上版本支持）
     *
     * @param param 搜索参数
     * @note 该功能为 IM 旗舰版功能，[购买旗舰版套餐包](https://buy.cloud.tencent.com/avc?from=17474)后可使用，详见[价格说明](https://cloud.tencent.com/document/product/269/11673?from=17176#.E5.9F.BA.E7.A1.80.E6.9C.8D.E5.8A.A1.E8.AF.A6.E6.83.85)
     */
    virtual void SearchGroupMembers(
        const V2TIMGroupMemberSearchParam& param,
        V2TIMValueCallback<V2TIMGroupSearchGroupMembersMap>* callback) = 0;

    /**
     * 3.4 修改指定的群成员资料
     */
    virtual void SetGroupMemberInfo(const V2TIMString& groupID,
                                    const V2TIMGroupMemberFullInfo& info,
                                    V2TIMCallback* callback) = 0;

    /**
     * 3.5 禁言（只有管理员或群主能够调用）
     *
     * @param seconds 禁言时间长度，单位秒，表示调用该接口成功后多少秒内不允许被禁言用户再发言。
     */
    virtual void MuteGroupMember(const V2TIMString& groupID, const V2TIMString& userID,
                                 uint32_t seconds,
                                 V2TIMCallback* callback) = 0;

    /**
     * 3.6 邀请他人入群
     *
     * @note 请注意不同类型的群有如下限制：
     * - 工作群（Work）：群里的任何人都可以邀请其他人进群。
     * - 会议群（Meeting）和公开群（Public）：只有通过rest api 使用 App
     * 管理员身份才可以邀请其他人进群。
     * - 直播群（AVChatRoom）：不支持此功能。
     */
    virtual void InviteUserToGroup(
        const V2TIMString& groupID, const V2TIMStringVector& userList,
        V2TIMValueCallback<V2TIMGroupMemberOperationResultVector>* callback) = 0;

    /**
     * 3.7 踢人（直播群踢人从 6.6 版本开始支持，需要您购买旗舰版套餐）
     *
     * @note 请注意不同类型的群有如下限制：
     * - 工作群（Work）：只有群主或 APP 管理员可以踢人。
     * - 公开群（Public）、会议群（Meeting）：群主、管理员和 APP 管理员可以踢人
     * - 直播群（AVChatRoom）：6.6 之前版本只支持禁言（MuteGroupMember），不支持踢人（KickGroupMember）。6.6 及以上版本支持禁言和踢人。
     */
    virtual void KickGroupMember(
        const V2TIMString& groupID, const V2TIMStringVector& memberList, const V2TIMString& reason,
        V2TIMValueCallback<V2TIMGroupMemberOperationResultVector>* callback) = 0;

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
    virtual void SetGroupMemberRole(const V2TIMString& groupID, const V2TIMString& userID,
        uint32_t role, V2TIMCallback* callback) = 0;

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
    virtual void MarkGroupMemberList(const V2TIMString& groupID,
        const V2TIMStringVector& memberList, uint32_t markType,
        bool enableMark, V2TIMCallback* callback) = 0;

    /**
     * 3.10 转让群主
     *
     * @note 请注意不同类型的群有如下限制：
     *  - 普通类型的群（Work、Public、Meeting）：只有群主才有权限进行群转让操作。
     *  - 直播群（AVChatRoom）：不支持转让群主。
     */
    virtual void TransferGroupOwner(const V2TIMString& groupID, const V2TIMString& userID,
                                    V2TIMCallback* callback) = 0;

    /////////////////////////////////////////////////////////////////////////////////
    //                         加群申请
    /////////////////////////////////////////////////////////////////////////////////

    /**
     * 4.1 获取加群申请列表
     * @note 最多支持50个
     */
    virtual void GetGroupApplicationList(
        V2TIMValueCallback<V2TIMGroupApplicationResult>* callback) = 0;

    /**
     * 4.2 同意某一条加群申请
     */
    virtual void AcceptGroupApplication(const V2TIMGroupApplication& application,
                                        const V2TIMString& reason, V2TIMCallback* callback) = 0;

    /**
     * 4.3 拒绝某一条加群申请
     */
    virtual void RefuseGroupApplication(const V2TIMGroupApplication& application,
                                        const V2TIMString& reason, V2TIMCallback* callback) = 0;

    /**
     * 4.4 标记申请列表为已读
     */
    virtual void SetGroupApplicationRead(V2TIMCallback* callback) = 0;

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                         社群-话题
    //
    /////////////////////////////////////////////////////////////////////////////////

    /**
     * 5.1 获取当前用户已经加入的支持话题的社群列表
     */
    virtual void GetJoinedCommunityList(V2TIMValueCallback<V2TIMGroupInfoVector> *callback) = 0;

    /**
     * 5.2 创建话题
     */
    virtual void CreateTopicInCommunity(const V2TIMString &groupID, const V2TIMTopicInfo &topicInfo,
        V2TIMValueCallback<V2TIMString> *callback) = 0;

    /**
     * 5.3 删除话题
     */
    virtual void DeleteTopicFromCommunity(const V2TIMString &groupID,
        const V2TIMStringVector &topicIDList,
        V2TIMValueCallback<V2TIMTopicOperationResultVector> *callback) = 0;

    /**
     * 5.4 修改话题信息
     */
    virtual void SetTopicInfo(const V2TIMTopicInfo &topicInfo, V2TIMCallback *callback) = 0;

    /**
     * 5.5 获取话题列表。
     * @note: topicIDList 传空时，获取此社群下的所有话题列表
     */
    virtual void GetTopicInfoList(const V2TIMString &groupID, const V2TIMStringVector &topicIDList,
        V2TIMValueCallback<V2TIMTopicInfoResultVector> *callback) = 0;
};

#endif  // __V2TIM_GROUP_MANAGER_H__
