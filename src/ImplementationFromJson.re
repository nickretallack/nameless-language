let f = (json: Js.Json.t): Implementation.t =>
  Implementation.(
    Json.Decode.(
      switch (field("type", string, json)) {
      | "constant" =>
        ConstantImplementation(
          field("value", PrimitiveValueFromJson.f, json),
        )
      | "interface" =>
        InterfaceImplementation({
          input: field("input", TypedFieldsFromJson.f, json),
          output: field("output", TypedFieldsFromJson.f, json),
        })
      }
    )
  );
