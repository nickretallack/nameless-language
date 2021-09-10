let f = (fields: array<PublishingValueType.t>) =>
  Json.Encode.array(PublishingValueTypeToJson.f, fields)
