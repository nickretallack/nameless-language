let f = (json: Js.Json.t): GraphImplementation.t =>
  Json.Decode.(
    GraphImplementation.{
      nodes: field("nodes", StringMapFromJson.f(NodeFromJson.f), json),
      connections: ConnectionMap.fromArray([||]),
      interface: {
        input: Belt.Map.String.empty,
        output: Belt.Map.String.empty,
      },
    }
  );
