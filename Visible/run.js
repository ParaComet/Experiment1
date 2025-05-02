const { exec } = require('child_process');
const { argv } = require('process');
const fs = require('fs');
const path = require('path');

// 获取传递的命令参数
const command = argv[2] || '';

// 设置程序路径 - 请确认路径是否正确
const programPath = path.join(__dirname, '../bin/Task1.exe');

// 验证程序是否存在
try {
    fs.accessSync(programPath, fs.constants.F_OK);
    console.log(`找到程序: ${programPath}`);
} catch (err) {
    console.error(`错误: 找不到程序 ${programPath}`);
    console.error('请确认:');
    console.error('1. 程序已编译为bin/Task1.exe');
    console.error('2. 路径配置正确');
    process.exit(1);
}

console.log('启动程序...');
const child = exec(`"${programPath}"`, { windowsVerbatimArguments: true }, (error, stdout, stderr) => {
    console.log('程序执行完成');
    if (error) {
        console.error(`执行错误: ${error}`);
        return;
    }
    if (stderr) {
        console.error(`程序错误输出: ${stderr}`);
    }
    if (stdout) {
        console.log('程序输出:');
        console.log(stdout);
    } else {
        console.log('程序未产生输出');
    }
});

// 处理命令输入
if (command) {
    console.log(`发送命令: ${command}`);
    child.stdin.write(command + '\n');
    child.stdin.end();
}

// 添加错误处理
child.on('error', (err) => {
    console.error('子进程错误:', err);
});

child.stdin.on('error', (err) => {
    console.error('输入流错误:', err);
});
