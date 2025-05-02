// 图形数据
let graphData = {
    nodes: [],
    edges: []
};

// 初始化可视化
document.addEventListener('DOMContentLoaded', function() {
    const runBtn = document.getElementById('run-btn');
    const outputElement = document.getElementById('algorithm-output');
    
    // 运行按钮点击事件
    runBtn.addEventListener('click', function() {
        runProgram(outputElement);
    });
    
    // 初始渲染空图形
    renderEmptyGraph();
});

// 运行程序
function runProgram(outputElement) {
    outputElement.textContent = "程序已启动，请输入指令...\n";
    
    // 显示输入区域
    document.getElementById('input-area').style.display = 'block';
    
    // 一次性绑定提交按钮事件
    const submitBtn = document.getElementById('submit-btn');
    submitBtn.onclick = function() {
        const command = document.getElementById('command-input').value;
        if (!command) return;
        
        outputElement.textContent += `> ${command}\n`;
        document.getElementById('command-input').value = '';
        
        // 使用Fetch API调用后端服务
        
        fetch('http://localhost:3000/execute', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({ command })
        })
        .then(response => response.json())
        .then(data => {
            console.log('Received response:', data);
            if (data.status === "success") {
                if (data.type === "raw_output") {
                    outputElement.textContent += `${data.message}\n`;
                    parseOutput(data.message);
                } else if (data.type === "shortest_path") {
                    outputElement.textContent += `${data.message}\n`;
                    updateGraphData(data);
                }
                renderGraphWithD3();
            } else {
                outputElement.textContent += `Error: ${data.message || 'Unknown error'}\n`;
            }
            outputElement.scrollTop = outputElement.scrollHeight;
        })
        .catch(error => {
            console.error('Request error:', error);
            let errorMessage = `请求错误: ${error.message || '未知错误'}\n`;
            
            if (error.message.includes('Failed to fetch')) {
                errorMessage = `无法连接到服务端，请检查:\n`
                            + `1. 服务端是否启动 (node server.js)\n`
                            + `2. 端口3000是否被占用\n`
                            + `3. 网络连接是否正常\n`;
            } else if (error.response) {
                // 处理HTTP错误响应
                error.response.json().then(errData => {
                    errorMessage = `错误: ${errData.message || '请求失败'}\n`;
                    if (errData.details) {
                        errorMessage += `详情: ${errData.details}\n`;
                    }
                    outputElement.textContent += errorMessage;
                    outputElement.scrollTop = outputElement.scrollHeight;
                }).catch(() => {
                    outputElement.textContent += errorMessage;
                    outputElement.scrollTop = outputElement.scrollHeight;
                });
                return;
            }
            
            // 处理网络错误
            outputElement.textContent = errorMessage;
            outputElement.scrollTop = outputElement.scrollHeight;
        });
    };
}

// 解析程序输出
// 清理ANSI转义序列
function cleanANSI(text) {
    return text.replace(/[\u001b\u009b][[()#;?]*(?:[0-9]{1,4}(?:;[0-9]{0,4})*)?[0-9A-ORZcf-nqry=><]/g, '');
}

function parseOutput(output) {
    // 重置图形数据
    graphData = { nodes: [], edges: [] };
    
    try {
        // 确保我们处理的是对象
        const data = typeof output === 'string' ? JSON.parse(output) : output;
        
        if (data.status === "error") {
            console.error('Server error:', data.message);
            return;
        }

        // 处理不同类型的响应
        switch(data.type) {
            case 'shortest_path':
                if (data.nodes) {
                    graphData.nodes = data.nodes.map(name => ({
                        id: name,
                        name: name
                    }));
                }
                
                if (data.edges) {
                    graphData.edges = data.edges.map(edge => ({
                        source: edge.source,
                        target: edge.target,
                        value: 1 // 默认权重
                    }));
                }
                break;
                
            case 'raw_output':
                // 处理原始输出
                const lines = data.message.split('\n');
                const nodeNames = new Set();
                const edges = new Set();
                
                lines.forEach(line => {
                    // 解析节点和边
                    if (line.includes('->')) {
                        const [source, target] = line.split('->');
                        edges.add({ source: source.trim(), target: target.trim() });
                        nodeNames.add(source.trim());
                        nodeNames.add(target.trim());
                    } else if (line.length === 1 && line.match(/[A-Z]/)) {
                        nodeNames.add(line.trim());
                    }
                });
                
                graphData.nodes = Array.from(nodeNames).map(name => ({
                    id: name,
                    name: name
                }));
                
                graphData.edges = Array.from(edges).map(edge => ({
                    source: edge.source,
                    target: edge.target,
                    value: 1 // 默认权重
                }));
                break;
                
            default:
                console.log('Unhandled response type:', data.type);
        }
        
        // 渲染更新后的图
        renderGraphWithD3();
    } catch (e) {
        console.error('Error parsing output:', e);
    }
}

// 使用D3.js渲染图形
function renderGraphWithD3() {
    const container = d3.select('#graph-container');
    container.html(''); // 清空容器
    
    const width = container.node().clientWidth;
    const height = 500;
    
    // 创建SVG画布
    const svg = container.append('svg')
        .attr('width', width)
        .attr('height', height);
    
    // 创建力导向图模拟
    const simulation = d3.forceSimulation(graphData.nodes)
        .force('link', d3.forceLink(graphData.edges).id(d => d.id).distance(100))
        .force('charge', d3.forceManyBody().strength(-300))
        .force('center', d3.forceCenter(width / 2, height / 2));
    
    // 绘制边
    const link = svg.append('g')
        .selectAll('line')
        .data(graphData.edges)
        .enter().append('line')
        .attr('stroke', '#999')
        .attr('stroke-width', 2);
    
    // 绘制节点
    const node = svg.append('g')
        .selectAll('circle')
        .data(graphData.nodes)
        .enter().append('circle')
        .attr('r', 20)
        .attr('fill', '#4CAF50')
        .call(d3.drag()
            .on('start', dragstarted)
            .on('drag', dragged)
            .on('end', dragended));
    
    // 添加节点标签
    const labels = svg.append('g')
        .selectAll('text')
        .data(graphData.nodes)
        .enter().append('text')
        .text(d => d.name)
        .attr('text-anchor', 'middle')
        .attr('dy', 4)
        .attr('fill', 'white');
    
    // 添加边权重标签
    const edgeLabels = svg.append('g')
        .selectAll('text')
        .data(graphData.edges)
        .enter().append('text')
        .text(d => d.value)
        .attr('font-size', 12)
        .attr('fill', '#333');
    
    // 更新模拟过程中的位置
    simulation.on('tick', () => {
        link
            .attr('x1', d => d.source.x)
            .attr('y1', d => d.source.y)
            .attr('x2', d => d.target.x)
            .attr('y2', d => d.target.y);
        
        node
            .attr('cx', d => d.x)
            .attr('cy', d => d.y);
        
        labels
            .attr('x', d => d.x)
            .attr('y', d => d.y);
        
        edgeLabels
            .attr('x', d => (d.source.x + d.target.x) / 2)
            .attr('y', d => (d.source.y + d.target.y) / 2);
    });
    
    // 拖拽函数
    function dragstarted(event, d) {
        if (!event.active) simulation.alphaTarget(0.3).restart();
        d.fx = d.x;
        d.fy = d.y;
    }
    
    function dragged(event, d) {
        d.fx = event.x;
        d.fy = event.y;
    }
    
    function dragended(event, d) {
        if (!event.active) simulation.alphaTarget(0);
        d.fx = null;
        d.fy = null;
    }
}

function renderEmptyGraph() {
    const container = document.getElementById('graph-container');
    container.innerHTML = "<p>Graph will be displayed here after running the program</p>";
}
