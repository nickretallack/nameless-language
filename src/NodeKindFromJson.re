let f = (json: Js.Json.t): NodeKind.t =>
  Json.Decode.(
    NodeKind.(
      switch (field("type", string, json)) {
      | "reference" => ReferenceNode
      | "list" => ListNode(field("length", int, json))
      | "defined" => DefinedNode(DefinedNodeFromJson.f(json))
      | type_ =>
        raise(Exception.JsonDecodeInvalidTypeName(type_, "NodeKind"))
      }
    )
  );
