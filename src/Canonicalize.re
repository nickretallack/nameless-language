open Definition;

let encodePrimitiveValue = (primitiveValue: primitiveValue) =>
  Json.(
    Encode.object_(
      switch (primitiveValue) {
      | IntegerValue(value) => [
          ("type", Encode.string("integer")),
          ("value", Encode.int(value)),
        ]
      | NumberValue(value) => [
          ("type", Encode.string("number")),
          ("value", Encode.float(value)),
        ]
      | TextValue(value) => [
          ("type", Encode.string("text")),
          ("value", Encode.string(value)),
        ]
      },
    )
  );

let encodeConstant = (primitiveValue: primitiveValue) : string =>
  Json.stringify(
    Json.Encode.(
      object_([
        ("type", string("constant")),
        ("value", encodePrimitiveValue(primitiveValue)),
      ])
    ),
  );

let canonicalizeImplementation =
    (implementation: implementation, dependencies: publishingDependencies)
    : (string, Belt.List.t(nibID), Belt.List.t(nibID)) =>
  switch (implementation) {
  | GraphImplementation(graphImplementation) =>
    CanonicalizeGraph.canonicalizeGraph(graphImplementation, dependencies)
  | ConstantImplementation(primitiveValue) => (
      encodeConstant(primitiveValue),
      [],
      [],
    )
  | _ => ("todo", [], [])
  };
