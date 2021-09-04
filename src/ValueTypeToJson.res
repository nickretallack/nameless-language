let f = (valueType: ValueType.t): Js.Json.t => {
  open Json.Encode
  object_(
    switch valueType {
    | PrimitiveValueType(primitiveValueType) => list{
        ("type", string("primitive")),
        ("primitiveType", PrimitiveValueTypeToJson.f(primitiveValueType)),
      }
    | DefinedValueType(definitionID) => list{
        ("type", string("defined")),
        ("definitionID", string(definitionID)),
      }
    | AnyType => list{("type", string("any"))}
    | SequencerType => list{("type", string("sequencer"))}
    },
  )
}
