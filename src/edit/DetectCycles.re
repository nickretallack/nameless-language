[%%debugger.chrome];
open Definition;

let detectCycles = (connections: connections, nodes: nodes): bool =>
  switch (ColumnizeNodes.columnizeNodes(nodes, connections)) {
  | exception CycleDetected => true
  | _ => false
  };

let rec isParentScope = (parent: nodeID, child: nodeID, nodes: nodes) => {
  Js.log("so far...");
  if (parent == child) {
    true;
  } else {
    switch (Belt.Map.String.getExn(nodes, child).scope) {
    | GraphScope => false
    | NodeScope(nodeID) => isParentScope(parent, nodeID, nodes)
    };
  };
};
let checkScopes =
    (source: connectionSide, sink: connectionSide, nodes: nodes): bool =>
  switch (sink.node) {
  | GraphConnection =>
    switch (source.node) {
    | GraphConnection => true
    | NodeConnection(sourceNodeID) =>
      let sourceNode = Belt.Map.String.getExn(nodes, sourceNodeID);
      sourceNode.scope == GraphScope;
    }
  | NodeConnection(sinkNodeID) =>
    switch (source.node) {
    | GraphConnection => true
    | NodeConnection(sourceNodeID) =>
      let sinkNode = Belt.Map.String.getExn(nodes, sinkNodeID);
      let sourceNode = Belt.Map.String.getExn(nodes, sourceNodeID);
      if (isFunctionDefinitionNode(sourceNode) && isKeywordNib(source.nib)) {
        switch (sinkNode.scope) {
        | GraphScope => sourceNodeID == sinkNodeID
        | NodeScope(sinkScopeNodeID) =>
          isParentScope(sourceNodeID, sinkScopeNodeID, nodes)
        };
      } else {
        switch (sourceNode.scope) {
        | GraphScope => true
        | NodeScope(sourceScopeNodeID) =>
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
    }
  };
