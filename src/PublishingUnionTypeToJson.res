let f = (fields: list<PublishingValueType.t>): Js.Json.t => {
  open Json.Encode
  object_(list{("type", string("union")), ("types", PublishingValueTypeListToJson.f(fields))})
}
