let f = (json: Js.Json.t): ValueType.t =>
  ValueType.(
    Json.Decode.(
      switch (field("type", string, json)) {
      | "primitive" =>
        PrimitiveValueType(
          field("primitiveType", PrimitiveValueTypeFromJson.f, json),
        )
      | "defined" => DefinedValueType(field("definitionID", string, json))
      | "any" => AnyType
      }
    )
  );
