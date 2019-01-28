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

let encodeCanonicalConstant = (primitiveValue: primitiveValue) : Js.Json.t =>
  Json.Encode.(
    object_([
      ("type", string("constant")),
      ("value", encodePrimitiveValue(primitiveValue)),
    ])
  );
