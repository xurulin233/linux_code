# chat

#### 介绍
基于Linux套接字的多线程聊天室


## 操作

#### 编译安装

1.  ./gcc.sh

#### 运行

1.  服务器
./server 8888

2.  客户端
./cliet 127.0.0.1 8888

#### 清除
./clean.sh

## 设计参考文章

《[从0实现基于Linux socket聊天室-多线程服务器模型-1](https://mp.weixin.qq.com/s?__biz=MzUxMjEyNDgyNw==&mid=2247487494&idx=1&sn=24bb6d39889d3cd27982bb996a811470&chksm=f96862f2ce1febe4e329663a236bd6c569de8fe5c3e95f71316b7800b7c0b06d11a64cea4c86&scene=21&token=1206659739&lang=zh_CN#wechat_redirect)》

《[从0实现基于Linux socket聊天室-多线程服务器一个很隐晦的错误-2](https://mp.weixin.qq.com/s?__biz=MzUxMjEyNDgyNw==&mid=2247487509&idx=1&sn=efd0ee711ded58a4f5c86ac37b4a4ace&chksm=f96862e1ce1febf7313f8ef7f3a79976536c6c51b753be922cbc9f047a455cd33080561a2841&scene=21&token=1206659739&lang=zh_CN#wechat_redirect)》

《[从0实现基于Linux socket聊天室-实现聊天室的登录、注册功能-3](https://mp.weixin.qq.com/s?__biz=MzUxMjEyNDgyNw==&mid=2247487573&idx=1&sn=b2d64611ad56203d0a2d4cdb582483d5&chksm=f96862a1ce1febb74bda0d0fa79ff01d834b66406549e6aec447fdcc97742bc393e969163434&scene=21&token=1206659739&lang=zh_CN#wechat_redirect)》

《[从0实现基于Linux socket聊天室-增加公聊、私聊-4](https://mp.weixin.qq.com/s?__biz=MzUxMjEyNDgyNw==&mid=2247487625&idx=1&sn=dc9ac657a9440abade9110d1c0fd92c0&chksm=f968627dce1feb6b03b07f3fbb427f1b3b962f88064f1d04bdcd382b5a9677340a3089d9bb23&scene=21&token=1206659739&lang=zh_CN#wechat_redirect)》

本文需要增加数据库功能，关于数据库的基础知识点，表的创建、增删改查等操作，以及对应的库函数的使用请参考以下3篇文章：


《[嵌入式数据库sqlite3【基础篇】-基本命令操作，小白一看就懂](https://mp.weixin.qq.com/s?__biz=MzUxMjEyNDgyNw==&mid=2247487673&idx=1&sn=1503fe6d5a9a935c69e31088989bd303&chksm=f968624dce1feb5b7f89641374c64b3d6f8fd2703c2efe652e6bb6817779dd725d66741ed012&scene=21&token=1206659739&lang=zh_CN#wechat_redirect)》


《[嵌入式数据库sqlite3【进阶篇】-子句和函数的使用，小白一文入门](https://mp.weixin.qq.com/s?__biz=MzUxMjEyNDgyNw==&mid=2247487776&idx=1&sn=6f41a0b12195e1a8c808a9d317dd132e&chksm=f96863d4ce1feac277bc0218ea9f7f65754b0e652c8543149df598f4c8ef87f3e1646de0de4c&scene=21&token=1206659739&lang=zh_CN#wechat_redirect)》


《[如何用C语言操作sqlite3，一文搞懂](https://mp.weixin.qq.com/s?__biz=MzUxMjEyNDgyNw==&mid=2247488235&idx=1&sn=6fa05807122804931a8bf35c78d221bd&chksm=f968601fce1fe909ebcd396000dea7d864f94cd891abfbaabc20550268b89fb7113f81092a93&scene=21&token=1206659739&lang=zh_CN#wechat_redirect)》


《[手把手教你如何实现一个简单的数据加解密算法](https://mp.weixin.qq.com/s?__biz=MzUxMjEyNDgyNw==&mid=2247499346&idx=1&sn=24e51acab57c498b853780933cc276f0&chksm=f96b8ca6ce1c05b06ebd06cc2cb7081caba86cc2602bc7b234fc08b62ba1363b918f306e419f&scene=21&token=787829922&lang=zh_CN#wechat_redirect)》


