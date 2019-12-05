let f = (kind: DefinedNodeKind.t): bool =>
  switch (kind) {
  | FunctionPointerCallNode => true
  | AccessorNode => true
  | _ => false
  };
