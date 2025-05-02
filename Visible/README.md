# 图形算法可视化工具使用指南

## 系统架构
- 前端：HTML/CSS/JavaScript (D3.js可视化)
- 后端：Node.js Express服务
- 核心程序：C++可执行文件

## 环境准备
1. 安装Node.js (https://nodejs.org/)
2. 打开命令行，进入项目目录：
```bash
cd D:\Experiment\Task1\Visible
```
3. 安装依赖：
```bash
npm install express cors
```
4. 验证安装：
```bash
npm list express cors
```

## 启动步骤
1. 启动后端服务：
```bash
node server.js
```
2. 在浏览器中打开`index.html`

## 使用说明
1. 点击"Run Program"启动程序
2. 在输入框中输入指令与程序交互
3. 查看下方算法输出和图形可视化

## 配置说明
- 修改`server.js`中的`PROGRAM_PATH`指向您的C++程序
- 确保C++程序已编译为`bin/Task1.exe`

## 常见问题
1. 如果无法连接，检查后端服务是否运行
2. 按F12查看浏览器控制台错误信息
3. 确保C++程序能独立运行并产生输出

## 服务管理
1. 正常停止服务：
   - 在运行server.js的命令行窗口中按Ctrl+C
2. 强制停止服务：
   - 打开任务管理器
   - 结束Node.js进程
3. 查看运行中的Node服务：
   ```bash
   tasklist | find "node"
   ```
4. 终止特定Node进程：
   ```bash
   taskkill /PID [进程ID] /F
   ```
