let f = (kind: string): DefinedNodeKind.t =>
  DefinedNodeKind.(
    switch (kind) {
    | "value" => ValueNode
    | "function call" => FunctionCallNode
    | "function pointer call" => FunctionPointerCallNode
    | "function definition" => FunctionDefinitionNode
    | "constructor" => ConstructorNode
    | "accessor" => AccessorNode
    | type_ =>
      raise(Exception.JsonDecodeInvalidTypeName(type_, "DefinedNodeKind"))
    }
  );
