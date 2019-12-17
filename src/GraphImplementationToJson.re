let f = (graphImplementation: GraphImplementation.t): Js.Json.t =>
  Json.Encode.(
    object_([
      ("type", string("graph")),
      ("nodes", MapToJson.f(graphImplementation.nodes, NodeToJson.f)),
      (
        "connections",
        list(
          ((sink, source)) =>
            object_([
              ("sink", ConnectionSideToJson.f(sink)),
              ("source", ConnectionSideToJson.f(source)),
            ]),
          Belt.Map.toList(graphImplementation.connections),
        ),
      ),
    ])
  );
