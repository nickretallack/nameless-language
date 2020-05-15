let f = (json: Js.Json.t): PrimitiveValue.t =>
  PrimitiveValue.(
    Json.(
      switch (Decode.field("type", Decode.string, json)) {
      | "integer" => IntegerValue(Decode.field("value", Decode.int, json))
      | "number" => NumberValue(Decode.field("value", Decode.float, json))
      | "text" => TextValue(Decode.field("value", Decode.string, json))
      | type_ => raise(Exception.JsonDecodeInvalidTypeName(type_, "PrimitiveValue"))
      }
    )
  );
