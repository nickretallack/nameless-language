let f = (json: Js.Json.t): DefinedNodeKind.t =>
  DefinedNodeKindFromString.f(Json.Decode.string(json));
