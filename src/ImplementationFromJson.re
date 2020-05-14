let f = (json: Js.Json.t): Implementation.t =>
  Implementation.(
    Json.Decode.(
      switch (field("type", string, json)) {
      | "constant" =>
        ConstantImplementation(
          field("value", PrimitiveValueFromJson.f, json),
        )
      | "interface" => InterfaceImplementation(InterfaceFromJson.f(json))
      | "external" =>
        ExternalImplementation({
          name: field("name", string, json),
          interface: field("interface", InterfaceFromJson.f, json),
        })
      | "graph" => GraphImplementation(GraphImplementationFromJson.f(json))
      }
    )
  );
