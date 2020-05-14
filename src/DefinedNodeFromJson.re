let f = (json: Js.Json.t): DefinedNode.t =>
  Json.Decode.(
    DefinedNode.{
      definitionID: field("definitionID", string, json),
      kind: field("kind", DefinedNodeKindFromJson.f, json),
    }
  );
