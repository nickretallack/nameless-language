let f = (nodeKind: NodeKind.t): Js.Json.t =>
  Json.Encode.(
    switch (nodeKind) {
    | ReferenceNode => object_([("type", string("reference"))])
    | ListNode(length) =>
      object_([("type", string("list")), ("length", int(length))])
    | DefinedNode(definedNode) => DefinedNodeToJson.f(definedNode)
    }
  );
