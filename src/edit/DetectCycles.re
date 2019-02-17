[%%debugger.chrome];
open Definition;

let detectCycles = (connections: connections, nodes: nodes): bool =>
  switch (ColumnizeNodes.columnizeNodes(nodes, connections)) {
  | exception CycleDetected => true
  | _ => false
  };

let rec isParentScope = (parent: nodeID, child: nodeID, nodes: nodes) =>
  if (parent == child) {
    true;
  } else {
    switch (Belt.Map.String.getExn(nodes, child).scope) {
    | GraphScope => false
    | NodeScope(nodeID) => isParentScope(parent, nodeID, nodes)
    };
  };
let checkConnectionScope =
    (source: connectionSide, sink: connectionSide, nodes: nodes): bool =>
  switch (sink.node, source.node) {
  | (_, GraphConnection) => true
  | (GraphConnection, NodeConnection(sourceNodeID)) =>
    Belt.Map.String.getExn(nodes, sourceNodeID).scope == GraphScope
  | (NodeConnection(sinkNodeID), NodeConnection(sourceNodeID)) =>
    let sinkNode = Belt.Map.String.getExn(nodes, sinkNodeID);
    let sourceNode = Belt.Map.String.getExn(nodes, sourceNodeID);

    if (sourceNodeID == sinkNodeID) {
      isFunctionDefinitionNode(sourceNode)
      && isKeywordNib(source.nib)
      && isFunctionDefinitionNode(sinkNode)
      && isKeywordNib(sink.nib);
    } else if (isFunctionDefinitionNode(sourceNode)
               && isKeywordNib(source.nib)) {
      if (sourceNodeID == sinkNodeID) {
        isKeywordNib(sink.nib);
      } else {
        switch (sinkNode.scope) {
        | GraphScope => sourceNodeID == sinkNodeID
        | NodeScope(sinkScopeNodeID) =>
          isParentScope(sourceNodeID, sinkScopeNodeID, nodes)
        };
      };
    } else {
      switch (sourceNode.scope) {
      | GraphScope => true
      | NodeScope(sourceScopeNodeID) =>
        /* connecting to an internal output? */
        sourceScopeNodeID == sinkNodeID
        || (
          switch (sinkNode.scope) {
          | GraphScope => false
          | NodeScope(sinkScopeNodeID) =>
            isParentScope(sinkScopeNodeID, sourceScopeNodeID, nodes)
          }
        )
      };
    };
  };

let checkScopes = (connections: connections, nodes: nodes): bool =>
  !
    Belt.Map.some(connections, (sink, source) =>
      !checkConnectionScope(source, sink, nodes)
    );
