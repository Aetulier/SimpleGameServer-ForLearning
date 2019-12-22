## SimpleGameServer-ForLearning
#### 丛林战争Demo C++服务端
服务端主要结构为：  
* Socket块：负责建立Socket连接和为每个客户端Socket内存分配，  
建立监听Socket使用select查询连接请求（主线程中） 
* Server块：负责创建线程池，初始化Socket连接，保存每个连接的客户端及消息的转发  
游戏房间的创建和保存操作  
* Client块：创建客户端连接后，启动单独线程来接收该客户端发送来的数据(select)  
* 反射实现：利用工厂类和map实现反射机制，为接受到的数据创建相应的类及处理方法  
* 其他：1.线程池的实现(ThreadPool) 2.对接收数据的解析及发送数据的打包 3.mysql的连接、查询等  
  
  
#### 服务端环境搭建
linux：  
需要GCC、mysql、cmake  
编辑器使用vscode  
mysql导入文件中备份
  
[线程池实现参考](https://github.com/mtrebi/thread-pool)\
[反射实现](https://www.cnblogs.com/qiuhongli/p/9019062.html)
[客户端](https://github.com/Aetulier/BattleGameDemo)
