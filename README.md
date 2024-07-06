 # wLog 

### headerOnly

​	将日志文件封装到一个文件

​			优点是使用简单 包含头文件即可 ，缺点是 大量文件重复编译，并且日志文件正常都是由大量文件引用，更会加剧这种问题



### log

​		正常使用 



## 文件夹介绍

​		

~~~log
config 内部有logConf.conf 根据此文件 来生成 所需日志级别  和 是否保存日志文件等  如果没有该文件 或者 文件名称错误 只有默认ERROR级别打印

inclue 包含 所有头文件

src  包含 所需要源文件

main.cpp 和 print.cpp 为测试日志文件  执行make  会生成wLog  ./wLog即可运行测试demo
~~~



​		
