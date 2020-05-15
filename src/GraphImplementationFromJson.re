let f = (json: Js.Json.t): GraphImplementation.t =>
  Json.Decode.(
    GraphImplementation.{
      nodes: field("nodes", StringMapFromJson.f(NodeFromJson.f), json),
      connections:
        ConnectionMap.fromArray(
          field(
            "connections",
            array(itemJson =>
              (
                field("sink", ConnectionSideFromJson.f, itemJson),
                field("source", ConnectionSideFromJson.f, itemJson),
              )
            ),
            json,
          ),
        ),
      interface: field("interface", InterfaceFromJson.f, json),
    }
  );
