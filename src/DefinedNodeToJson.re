let f = (definedNode: DefinedNode.t): Js.Json.t =>
  Json.Encode.(
    object_([
      ("type", string("defined")),
      ("definitionID", string(definedNode.definitionID)),
      ("kind", string(DefinedNodeKindToString.f(definedNode.kind))),
    ])
  );
