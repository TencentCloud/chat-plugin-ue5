### 腾讯云IM UE5 Plugin集成

###### 步骤一 在腾讯云IM 控制台创建应用

登录到[腾讯云IM控制台](https://console.cloud.tencent.com/im)，如图所示创建IM应用：

<img src="https://markdown-1252238885.cos.ap-guangzhou.myqcloud.com/2022-09-12-053828.png" alt="image-20220912133828217" style="zoom:50%;" />

在应用[基础信息页面](https://console.cloud.tencent.com/im/detail)，记录应用sdkappid和密钥，在后续使用[初始化](https://im.sdk.qcloud.com/doc/zh-cn/classV2TIMManager.html#aecee922675b671cd979d68604a4be1bb)和[登录](https://im.sdk.qcloud.com/doc/zh-cn/classV2TIMManager.html#a6a9c19be21327ace77ab75657d2944b3)接口时会用到。

###### 步骤二 安装UE5，并创建新项目

安装[Epic games Luncher](https://store.epicgames.com/en-US/download)，并且通过Epic安装Unreal Engine 5.0.0+版本，安装好如图所示：

![image-20220912134459287](https://markdown-1252238885.cos.ap-guangzhou.myqcloud.com/2022-09-12-054459.png)

启动UE5，创建UE5应用，如图所示：
![image](https://user-images.githubusercontent.com/6051292/226270906-5b64d82e-40b8-4df5-af19-d92efc37fe3c.png)



其中项目默认设置中蓝图和C++、目标平台的配置，可根据业务方自己的需求进行配置，腾讯云IM UE5 Plugin对此没有限制。

创建好后应用后，项目目录如图所示：

![image-20220912135806412](https://markdown-1252238885.cos.ap-guangzhou.myqcloud.com/2022-09-12-055806.png)

###### 步骤三 下载Plugin

```shell
// 将插件从GitHub下来
git clone https://github.com/TencentCloud/chat-plugin-ue5.git
```



###### 步骤四 Plugin引入

在项目根目录创建插件目录，命名问Plugins，将步骤三中下载的插件包（Plugins/下的TencentCoudChat文件夹）引入其中，引入后可在UE5 Editor的编辑 => 插件模块查看到如图所示插件安装：

![image-20230228143510351](https://markdown-1252238885.cos.ap-guangzhou.myqcloud.com/2023-02-28-063517.png)

插件引入后，在项目主模块中的Build.cs文件中，引入插件模块：如图所示：
![image-20230228143546719](https://markdown-1252238885.cos.ap-guangzhou.myqcloud.com/2023-02-28-063547.png)

###### 步骤五 头文件引入

在业务CPP文件中引入TencentCloudChat.h后，开始使用腾讯云IM UE5 Plugin提供的能力

###### 步骤六 测试验证

验证代码如图所示：

![image-20230228143650915](https://markdown-1252238885.cos.ap-guangzhou.myqcloud.com/2023-02-28-063651.png)

其中初始化用到的sdkappid由在步骤一种获得，登录使用到的usersig，可在[控制台辅助工具](https://console.cloud.tencent.com/im/tool-usersig)生成。在生产环境，usersig由业务侧后台生成。本篇入门教程只演示了初始化、登录、注册消息监听事件的API调用，完整的API文档可以查看[此处](https://im.sdk.qcloud.com/doc/zh-cn/classV2TIMManager.html)。UE5各个平台均集成的是C++ SDK，且所有接口一致。

在UE5控制台中见到如下日志，代表plugin集成成功：

![image-20220912143743846](https://markdown-1252238885.cos.ap-guangzhou.myqcloud.com/2022-09-12-063744.png)

![image-20220912143819779](https://markdown-1252238885.cos.ap-guangzhou.myqcloud.com/2022-09-12-063820.png)

###### 相关文档

[API文档](https://im.sdk.qcloud.com/doc/zh-cn/classV2TIMManager.html)

[UE5官方文档](https://docs.unrealengine.com/5.0/zh-CN/)

###### 注意
1. 引入插件且编译成功后再加入API调用测试代码
2. 此插件只能在ue 模块中集成


###### 常见问题

1. 在mac调试环境，如遇到提示文件已经损坏，可执行如下命令修复sudo xattr -r -d com.apple.quarantine $pathToframgeWork 
2. 初始化传入的日志和db目录，需要应用有读写权限
3. 插件暂不支持win32进行调试和编译

