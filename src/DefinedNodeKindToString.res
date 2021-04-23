let f = (kind: DefinedNodeKind.t): string =>
  switch kind {
  | ValueNode => "value"
  | FunctionCallNode => "function call"
  | FunctionPointerCallNode => "function pointer call"
  | FunctionDefinitionNode => "function definition"
  | ConstructorNode => "constructor"
  | AccessorNode => "accessor"
  }
