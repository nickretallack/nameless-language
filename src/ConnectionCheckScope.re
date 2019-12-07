let f =
    (source: ConnectionSide.t, sink: ConnectionSide.t, nodes: NodeMap.t): bool =>
  switch (sink.node, source.node) {
  | (_, GraphConnection) => true
  | (GraphConnection, NodeConnection(sourceNodeID)) =>
    Belt.Map.String.getExn(nodes, sourceNodeID).scope == GraphScope
  | (NodeConnection(sinkNodeID), NodeConnection(sourceNodeID)) =>
    let sinkNode = Belt.Map.String.getExn(nodes, sinkNodeID);
    let sourceNode = Belt.Map.String.getExn(nodes, sourceNodeID);

    if (sourceNodeID == sinkNodeID) {
      NodeIsFunctionDefinition.f(sourceNode)
      && ConnectionNibIsKeyword.f(source.nib)
      && NodeIsFunctionDefinition.f(sinkNode)
      && ConnectionNibIsKeyword.f(sink.nib);
    } else if (NodeIsFunctionDefinition.f(sourceNode)
               && ConnectionNibIsKeyword.f(source.nib)) {
      if (sourceNodeID == sinkNodeID) {
        ConnectionNibIsKeyword.f(sink.nib);
      } else {
        switch (sinkNode.scope) {
        | GraphScope => sourceNodeID == sinkNodeID
        | NodeScope(sinkScopeNodeID) =>
          NodeIsParentScope.f(sourceNodeID, sinkScopeNodeID, nodes)
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
            NodeIsParentScope.f(sourceScopeNodeID, sinkScopeNodeID, nodes)
          }
        )
      };
    };
  };
