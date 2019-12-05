let f = (node: Node.t): bool =>
  switch (node.kind) {
  | DefinedNode({kind: FunctionDefinitionNode}) => true
  | _ => false
  };
