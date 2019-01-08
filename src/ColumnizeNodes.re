open Definition;

let isRootNode = (nodeID: nodeID, connections: connections): bool =>
  !
    Belt.Map.some(connections, (sink: connectionSide, source: connectionSide) =>
      switch (source.node) {
      | GraphConnection => false
      | NodeConnection(connectionNodeID) =>
        nodeID != connectionNodeID ?
          false :
          (
            switch (sink.node) {
            | NodeConnection(_) => true
            | GraphConnection => false
            }
          )
      }
    );

let rec topoSort = (nodes: nodes, connections: connections) => {
  let (availableNodes, unavailableNodes) =
    Belt.Map.String.partition(nodes, (nodeID, _node) =>
      isRootNode(nodeID, connections)
    );
  let remainingConnections =
    Belt.Map.keep(connections, (sink, _source) =>
      switch (sink.node) {
      | GraphConnection => false
      | NodeConnection(nodeID) =>
        !
          Belt.Map.String.some(availableNodes, (connectionNodeID, _node) =>
            connectionNodeID == nodeID
          )
      }
    );
  if (Belt.Map.String.isEmpty(unavailableNodes)) {
    [availableNodes];
  } else {
    [availableNodes, ...topoSort(unavailableNodes, remainingConnections)];
  };
};
