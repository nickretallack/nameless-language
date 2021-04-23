let f = (fields: list<PublishingValueType.t>) =>
  Json.Encode.list(PublishingValueTypeToJson.f, fields)
