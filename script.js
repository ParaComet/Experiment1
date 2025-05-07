document.addEventListener('DOMContentLoaded', function () {
    const nodes = new vis.DataSet([]);
    const edges = new vis.DataSet([]);
    let network = null;
    const container = document.getElementById('mynetwork');

    const options = {
        layout: {
            hierarchical: false,
            improvedLayout: true // Added for potentially better layout
        },
        edges: {
            arrows: {
                to: { enabled: true, scaleFactor: 0.7, type: 'arrow' }
            },
            color: {
                color: '#78909C', // Material Design Grey
                highlight: '#0288D1', // Material Design Light Blue
                hover: '#0288D1', // Material Design Light Blue
                inherit: 'from',
                opacity: 1.0
            },
            font: {
                size: 12,
                color: '#455A64', // Darker Grey for text
                face: 'Roboto, Arial, sans-serif',
                strokeWidth: 0, // No stroke for cleaner look
                strokeColor: '#ffffff'
            },
            smooth: {
                enabled: true,
                type: "cubicBezier", // Smoother curves
                roundness: 0.7
            },
            width: 1.5
        },
        nodes: {
            shape: 'dot',
            size: 18, // Slightly larger nodes
            borderWidth: 2,
            color: {
                border: '#0D47A1', // Material Design Primary Blue
                background: '#BBDEFB', // Material Design Light Blue
                highlight: {
                    border: '#0D47A1',
                    background: '#E3F2FD' // Lighter shade for highlight
                },
                hover: {
                    border: '#0D47A1',
                    background: '#E3F2FD'
                }
            },
            font: {
                size: 14,
                color: '#37474F', // Material Design Text Color
                face: 'Roboto, Arial, sans-serif'
            }
        },
        interaction: {
            hover: true,
            tooltipDelay: 200,
            navigationButtons: false, // Disable built-in navigation buttons
            keyboard: true // Enables keyboard navigation
        },
        physics:{
            enabled: true,
            barnesHut: {
                gravitationalConstant: -3000, // Slightly increased for more spread
                centralGravity: 0.25,
                springLength: 100,
                springConstant: 0.05,
                damping: 0.09,
                avoidOverlap: 0.15 // Increased to prevent overlap more
            },
            solver: 'barnesHut',
            stabilization: {
                iterations: 1500 // Increased for better initial stability
            }
        }
    };

    function fetchAndUpdateGraph() {
        // Add a cache-busting parameter to ensure fresh data
        fetch('bin/graph_data.json?cachebust=' + new Date().getTime())
            .then(response => {
                if (!response.ok) {
                    throw new Error('Network response was not ok ' + response.statusText);
                }
                return response.json();
            })
            .then(data => {
                // Ensure data.nodes and data.edges are arrays, even if the JSON provides null or other types
                const newNodesData = Array.isArray(data.nodes) ? data.nodes : []; // Renamed for clarity
                const rawEdges = Array.isArray(data.edges) ? data.edges : [];

                // --- BEGIN MODIFICATION FOR NODE REMOVAL ---
                // Get current node IDs from the DataSet
                const currentNodeIds = nodes.getIds();
                // Get new node IDs from the fetched data
                const newNodeIds = new Set(newNodesData.map(node => node.id));

                // Identify nodes to remove
                const nodesToRemove = currentNodeIds.filter(id => !newNodeIds.has(id));

                // Remove stale nodes
                if (nodesToRemove.length > 0) {
                    nodes.remove(nodesToRemove);
                }
                // --- END MODIFICATION FOR NODE REMOVAL ---

                // Generate unique IDs for edges to prevent duplicates
                const newEdgesData = rawEdges.map(edge => { // Renamed for clarity
                    const edgeObject = {
                        ...edge,
                        id: `${edge.from}-${edge.to}-${edge.label}-${edge.isError}` // Ensure ID is unique, include isError
                    };
                    if (edge.isError) {
                        edgeObject.color = { color: 'red', highlight: 'red', hover: 'red' };
                        edgeObject.arrows = {
                            to: { enabled: true, scaleFactor: 1, type: 'arrow' }
                        };
                    } else {
                        // Ensure non-error edges also have default arrow type if not specified elsewhere
                        // Or rely on global options if that's preferred
                        edgeObject.arrows = {
                            to: { enabled: true, scaleFactor: 1, type: 'arrow' }
                        };
                    }
                    return edgeObject;
                });

                // Get current edge IDs from the DataSet
                const currentEdgeIds = edges.getIds();
                // Get new edge IDs from the fetched data
                const newEdgeIdsSet = new Set(newEdgesData.map(edge => edge.id)); // Renamed for clarity

                // Identify edges to remove
                const edgesToRemove = currentEdgeIds.filter(id => !newEdgeIdsSet.has(id));

                // Remove stale edges
                if (edgesToRemove.length > 0) {
                    edges.remove(edgesToRemove);
                }

                // Update the DataSets with new/updated nodes and edges
                nodes.update(newNodesData); // Use newNodesData
                edges.update(newEdgesData); // Use newEdgesData

                if (!network) {
                    // If the network hasn't been initialized, create it now
                    const graphData = {
                        nodes: nodes,
                        edges: edges
                    };
                    network = new vis.Network(container, graphData, options);
                }
            })
            .catch(error => {
                console.error('Error fetching or parsing graph_data.json:', error);
                if (!network) {
                    // If it's the initial load and it failed, display an error message in the container
                    container.innerHTML = '<p style="color: red;">Error loading graph data. Please ensure graph_data.json is available and correctly formatted. Check the console for more details.</p>';
                }
                // For subsequent update errors, we just log it and keep the current graph displayed.
            });
    }

    let lastSignalTimestamp = 0;

    function checkForUpdates() {
        fetch('bin/update_signal.json?cachebust=' + new Date().getTime())
            .then(response => {
                if (!response.ok) {
                    // If the signal file is not found, log an error but continue trying
                    console.warn('Could not fetch update_signal.json. Will retry.');
                    return null;
                }
                return response.json();
            })
            .then(signalData => {
                if (signalData && signalData.timestamp > lastSignalTimestamp) {
                    console.log('Update signal detected. New timestamp:', signalData.timestamp, 'Old timestamp:', lastSignalTimestamp);
                    lastSignalTimestamp = signalData.timestamp;
                    fetchAndUpdateGraph(); // Fetch and update the main graph data
                }
            })
            .catch(error => {
                // Log errors related to fetching or parsing the signal file, but don't stop the polling
                console.error('Error checking for updates (update_signal.json):', error);
            })
            .finally(() => {
                // Poll every 1 second (1000 milliseconds)
                setTimeout(checkForUpdates, 1000);
            });
    }

    // Perform the initial fetch and graph creation
    fetchAndUpdateGraph();

    // Start checking for updates
    checkForUpdates();

    // 绑定控件功能
    document.querySelector('.vis-zoomIn').addEventListener('click', () => {
        network.moveTo({
            scale: network.getScale() * 1.2,
            animation: true
        });
    });

    document.querySelector('.vis-zoomOut').addEventListener('click', () => {
        network.moveTo({
            scale: network.getScale() * 0.8,
            animation: true
        });
    });

    document.querySelector('.vis-zoomExtends').addEventListener('click', () => {
        network.fit({
            animation: true
        });
    });

    document.querySelector('.vis-up').addEventListener('click', () => {
        const viewPosition = network.getViewPosition();
        network.moveTo({
            position: {
                x: viewPosition.x,
                y: viewPosition.y - 100
            },
            animation: true
        });
    });

    document.querySelector('.vis-down').addEventListener('click', () => {
        const viewPosition = network.getViewPosition();
        network.moveTo({
            position: {
                x: viewPosition.x,
                y: viewPosition.y + 100
            },
            animation: true
        });
    });

    document.querySelector('.vis-left').addEventListener('click', () => {
        const viewPosition = network.getViewPosition();
        network.moveTo({
            position: {
                x: viewPosition.x - 100,
                y: viewPosition.y
            },
            animation: true
        });
    });

    document.querySelector('.vis-right').addEventListener('click', () => {
        const viewPosition = network.getViewPosition();
        network.moveTo({
            position: {
                x: viewPosition.x + 100,
                y: viewPosition.y
            },
            animation: true
        });
    });
});