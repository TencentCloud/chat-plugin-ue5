// Copyright (c) 2022 Tencent. All rights reserved.

/////////////////////////////////////////////////////////////////////
//
//                     腾讯云通信服务 IMSDK
//
//                        离线推送的接口
//
/////////////////////////////////////////////////////////////////////

#ifndef __V2TIM_OFFLINE_PUSH_MANAGER_H__ 
#define __V2TIM_OFFLINE_PUSH_MANAGER_H__ 

#include "V2TIMCallback.h"
#include "V2TIMMessage.h"

class V2TIMOfflinePushManager  {
public:
    virtual ~V2TIMOfflinePushManager () {}

    /**
     * 设置离线推送配置信息
     * note
     *  - 如果您想关闭离线推送，请把 config 设置为 V2TIMOfflinePushConfig()
     * @param config   离线推送配置
     * @param callback 回调
     */
    virtual void SetOfflinePushConfig(const V2TIMOfflinePushConfig &config, V2TIMCallback *callback) = 0;

    /**
     * APP 检测到应用退后台时可以调用此接口，可以用作桌面应用角标的初始化未读数量。
     *
     * - 从 6.1 版本开始，如果配置了离线推送，会收到厂商或 TPNS 的离线推送通道下发的通知栏消息。
     *
     * @param unreadCount 未读数量
     * @param callback 回调
     */
    virtual void DoBackground(uint32_t unreadCount, V2TIMCallback *callback) = 0;

    /**
     * APP 检测到应用进前台时可以调用此接口
     *
     * - 从 6.1 版本开始，调用 DoForeground，会停止离线推送。但如果应用被 kill，仍然可以正常接收离线推送。
     *
     * @param callback 回调
     */
    virtual void DoForeground(V2TIMCallback *callback) = 0;
};

#endif  // __V2TIM_OFFLINE_PUSH_MANAGER_H__ 
