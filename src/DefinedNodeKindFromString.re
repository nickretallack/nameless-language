let f = (kind: string): DefinedNodeKind.t =>
  DefinedNodeKind.(
    switch (kind) {
    | "value" => ValueNode
    | "function call" => FunctionCallNode
    | "function pointer call" => FunctionPointerCallNode
    | "function definition" => FunctionDefinitionNode
    | "constructior" => ConstructorNode
    | "accessor" => AccessorNode
    | _ => raise(Exception.JsonDecodeInvalidTypeName)
    }
  );
