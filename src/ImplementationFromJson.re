let f = (json: Js.Json.t): Implementation.t =>
  Implementation.(
    Json.Decode.(
      switch (field("type", string, json)) {
      | "constant" =>
        ConstantImplementation(
          json |> field("value", PrimitiveValueFromJson.f),
        )
      }
    )
  );
