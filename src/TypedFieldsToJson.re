let f = (typedFields: RecordType.t): Js.Json.t =>
  MapToJson.f(typedFields, ValueTypeToJson.f);
