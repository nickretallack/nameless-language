[%%debugger.chrome];
open Definition;

/* Find all source nodeIDs that are connected to this sink nodeID */
let followConnections =
    (connections: connections, nodeID: nodeID): Belt.Set.String.t =>
  Belt.Map.reduce(connections, Belt.Set.String.empty, (acc, sink, source) =>
    switch (sink.node) {
    | NodeConnection(sinkNodeID) =>
      nodeID == sinkNodeID ?
        switch (source.node) {
        | NodeConnection(sourceNodeID) =>
          Belt.Set.String.add(acc, sourceNodeID)
        | GraphConnection => acc
        } :
        acc
    | GraphConnection => acc
    }
  );

let rec dfs =
        (connections: connections, nodeID, seen: Belt.Set.String.t): bool =>
  Belt.List.some(
    Belt.Set.String.toList(followConnections(connections, nodeID)),
    sourceNodeID =>
    Belt.Set.String.has(seen, sourceNodeID)
    || dfs(connections, sourceNodeID, Belt.Set.String.add(seen, nodeID))
  );

let detectCycles = (connections: connections, nodes: nodes): bool =>
  Belt.List.some(
    Array.to_list(Belt.Map.String.keysToArray(nodes)), (nodeID: nodeID) =>
    dfs(connections, nodeID, Belt.Set.String.fromArray([|nodeID|]))
  );
