let f = (json: Js.Json.t): NodeKind.t =>
  Json.Decode.(
    NodeKind.(
      switch (field("type", string, json)) {
      | "reference" => ReferenceNode
      | "list" => ListNode(field("length", int, json))
      | "defined" => DefinedNode(DefinedNodeFromJson.f(json))
      | _ => raise(Exception.JsonDecodeInvalidTypeName)
      }
    )
  );
