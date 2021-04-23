let f = (json: Js.Json.t): RecordType.t => StringMapFromJson.f(ValueTypeFromJson.f, json)
