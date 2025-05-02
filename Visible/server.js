const express = require('express');
const { exec } = require('child_process');
const path = require('path');
const cors = require('cors');

const app = express();
app.use(cors());
app.use(express.json());

// 配置程序路径 - 使用用户提供的绝对路径
const PROGRAM_PATH = 'D:\\Experiment\\Task1\\bin\\Task1.exe';

// 执行程序端点
app.post('/execute', (req, res) => {
    const { command } = req.body;
    
    console.log(`Executing program: ${PROGRAM_PATH}`);
    const child = exec(`"${PROGRAM_PATH}"`, { windowsVerbatimArguments: true });
    
    if (command) {
        console.log(`Sending command: ${command}`);
        child.stdin.write(command + '\n');
        child.stdin.end();
    }

    let output = '';
    // 合并输出处理
    child.stdout.on('data', (data) => {
        console.log(`stdout: ${data}`);
        output += data;
    });
    
    child.stderr.on('data', (data) => {
        console.error(`stderr: ${data}`);
        output += data;
    });

    child.on('close', (code) => {
        console.log(`Process exited with code ${code}`);
        if (code !== 0) {
            console.error('Program execution failed:', output);
            return res.status(500).json({
                status: "error",
                message: "程序执行失败",
                details: output,
                exitCode: code
            });
        }
        
        // 清理输出 - 移除调试信息和空行
        const cleanOutput = output.split('\n')
            .filter(line => !line.includes('Process exited with code') &&
                          !line.includes('Executing program:') &&
                          line.trim() !== '')
            .join('\n')
            .trim();

        try {
            // 尝试解析JSON输出
            const jsonOutput = JSON.parse(cleanOutput);
            console.log('Returning JSON output:', jsonOutput);
            res.json({
                ...jsonOutput,
                status: "success"
            });
        } catch (e) {
            console.log('Returning cleaned output:', cleanOutput);
            // 如果不是JSON格式，返回清理后的输出
            res.json({ 
                type: "raw_output",
                message: cleanOutput || "命令执行成功",
                status: "success"
            });
        }
    });

        child.on('error', (err) => {
            console.error('Failed to start program:', err);
            res.status(500).json({
                status: "error",
                message: "无法启动程序",
                details: err.message
            });
        });
});

const PORT = 3000;
app.listen(PORT, () => {
    console.log(`Server running on http://localhost:${PORT}`);
});
