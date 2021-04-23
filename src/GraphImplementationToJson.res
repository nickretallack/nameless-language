let f = (graphImplementation: GraphImplementation.t): Js.Json.t => {
  open Json.Encode
  object_(list{
    ("type", string("graph")),
    ("nodes", StringMapToJson.f(graphImplementation.nodes, NodeToJson.f)),
    (
      "connections",
      list(
        ((sink, source)) =>
          object_(list{
            ("sink", ConnectionSideToJson.f(sink)),
            ("source", ConnectionSideToJson.f(source)),
          }),
        Belt.Map.toList(graphImplementation.connections),
      ),
    ),
    ("interface", InterfaceToJson.f(graphImplementation.interface)),
  })
}
