[%%debugger.chrome];
open Definition;

let detectCycles = (connections: connections, nodes: nodes): bool =>
  switch (ColumnizeNodes.columnizeNodes(nodes, connections)) {
  | exception CycleDetected => true
  | _ => false
  };
