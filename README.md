#作者：
橙子,C++服务器开发\中间件\redis，目前在职,email:karl.zhao@qq.com  

#功能：
1.这是一个SQL文件转XML的工具，主要是MySql导出来的sql文件（Linux导出来，默认编码为UTF-8）转成XML文件;  
2.支持多文件同时转换;  
3.增加日志文档，日志文档根据年月日来创建;

#关于编码：
1.这里的所有源码都是ANSI编码格式;  
2.sql文件编码，如果sql文件是ANSI编码，那转的xml文件也是ANSI编码，目前这个工具，支持ansi、utf-8的转换。  

#关于运行环境：
1.windows下，使用C-free IDE默认的mingw5(默认编译器是gcc、g++)，成功编译且运行;  
2.Linux，可直接运行makefile文件，生成sql2xml执行文件，可直接运行.  

#关于文件：
1. main.cpp  
2. SimpleLog.h：简单的日志操作函数的声明文件  
3. SimpleLog.cpp：简单的日志操作函数的实现文件   
4. SqlToXml.h：SqlToXml文件类   
5. SqlToXml.cpp：SqlToXml文件类包括解析sql文件，创建xml文件的操作  
6. tinystr.h：xml文件的字符类   
7. tinystr.cpp：xml文件的字符类的实现   
8. tinyxml.h：xml类   
9. tinyxml.cpp：xml类的实现   
10.tinyxmlerror.cpp：xml的异常   
11.tinyxmlparser.cpp：XML的分析器   
12.demo.sql : 测试所用的sql文件   

其中6-10号文件，是TinyXML库文件  
