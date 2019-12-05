let f = (connectionNode: ConnectionNode.t): string => {
  switch (connectionNode) {
  | GraphConnection => "graph"
  | NodeConnection(nodeID) => nodeID
  };
};
