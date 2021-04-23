let f = (primitiveValue: PrimitiveValue.t): Js.Json.t => {
  open Json
  Encode.object_(
    switch primitiveValue {
    | IntegerValue(value) => list{("type", Encode.string("integer")), ("value", Encode.int(value))}
    | NumberValue(value) => list{("type", Encode.string("number")), ("value", Encode.float(value))}
    | TextValue(value) => list{("type", Encode.string("text")), ("value", Encode.string(value))}
    },
  )
}
