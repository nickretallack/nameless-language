let f = (kind: DefinedNodeKind.t): bool =>
  switch kind {
  | ValueNode => true
  | FunctionDefinitionNode => true
  | ConstructorNode => true
  | _ => false
  }
