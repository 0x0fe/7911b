-----------------------------------------------------------
# Release-v3.0.7   2018年 9月 6日 星期四
-----------------------------------------------------------

## libduer-device Release-v3.0.7
1.   新增airkiss配网即绑定功能对接小度儿童乐园公众号
2.   优化本地vad参数，继续提升语音交互速度
3.   优化微聊语音支持不立即播放
4.   优化codec出错的处理逻辑
5.   优化支持下载302跳转后超过1K的url

-----------------------------------------------------------
# Release-v3.0.6   2018年 8月 1日 星期三
-----------------------------------------------------------

## libduer-device Release-v3.0.6
1. 新增 设备端本地vad，提升语音交互速度
2. 新增 添加记录语音包写入套接字缓冲区的时间点逻辑，帮助实现端到端时延统计
3. 优化 http模块代码,提高http模块的稳定性
4. 优化 使用snprintf替代esp32中的打印函数，优化栈空间使用
5. 修复 去除LinkClicked事件的token字段，满足协议要求
6. 修复 上传空语音包的bug