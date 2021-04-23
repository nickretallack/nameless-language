let f = (id: DefinitionID.t, valueType: PublishingValueType.t): Js.Json.t => {
  open Json.Encode
  object_(list{
    ("type", string("label")),
    ("id", string(id)),
    ("wrapped", PublishingValueTypeToJson.f(valueType)),
  })
}
