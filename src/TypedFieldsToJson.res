let f = (typedFields: RecordType.t): Js.Json.t => StringMapToJson.f(typedFields, ValueTypeToJson.f)
