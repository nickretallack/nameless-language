let f = (implementation: Implementation.t): Js.Json.t =>
  Json.Encode.(
    switch (implementation) {
    | ConstantImplementation(primitiveValue) =>
      object_([
        ("type", string("constant")),
        ("value", PrimitiveValueToJson.f(primitiveValue)),
      ])
    | InterfaceImplementation(interface) => InterfaceToJson.f(interface)
    | ExternalImplementation({name, interface}) =>
      object_([
        // can I avoid doing this?
        ("type", string("external")),
        ("name", string(name)),
        ("interface", InterfaceToJson.f(interface)),
      ])
    | GraphImplementation(graphImplementation) =>
      GraphImplementationToJson.f(graphImplementation)
    | RecordTypeImplementation(recordType) =>
      object_([
        ("type", string("recordType")),
        ("fields", TypedFieldsToJson.f(recordType)),
      ])
    | LabeledTypeImplementation(maybeValueType) =>
      object_([
        ("type", string("label")),
        ("wrapped", nullable(ValueTypeToJson.f, maybeValueType)),
      ])
    | UnionTypeImplementation(typeSet) =>
      object_([
        ("type", string("union")),
        ("types", array(ValueTypeToJson.f, Belt.Set.toArray(typeSet))),
      ])
    }
  );
