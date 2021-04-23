let f = (json: Js.Json.t): ValueType.t => {
  open ValueType
  open Json.Decode
  switch field("type", string, json) {
  | "primitive" => PrimitiveValueType(field("primitiveType", PrimitiveValueTypeFromJson.f, json))
  | "defined" => DefinedValueType(field("definitionID", string, json))
  | "any" => AnyType
  | type_ => raise(Exception.JsonDecodeInvalidTypeName(type_, "ValueType"))
  }
}
