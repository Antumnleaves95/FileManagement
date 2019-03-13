#   实验室管理系统

> 本来之前是做一个文件管理系统，后面发展了很多功能就叫实验室管理系统了。

这边只是客户端展示。

## 介绍

界面用的QT 5.7，之前也没接触过直接上手写了。主要是实验室几个人一起写的，研一的时候有一门课是软件实践，要求几个人组队开发软件。

## 特点

- 仿MacOS界面，阴影无边框；

- 充分利用面向对象编程思想，类之前的继承多态等实现较好；

- 多线程下载，断点传输

  可以同时下载多个文件，一开始是用TCP + 本地SQLite数据库实现，正常使用是没什么问题，但是如果客户端突然挂掉就不好记录断点。后面通过读取文件大小再计算断点发给服务端，后续计划换成http实现；

- MD5加密密码，本地自动记住密码

  本地通过简单的Base64加密存储密码，然后解密成密码再加密为MD5进行密码验证；

- 待更新。






