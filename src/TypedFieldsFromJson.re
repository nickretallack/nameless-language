let f = (json: Js.Json.t): RecordType.t =>
  MapFromJson.f(ValueTypeFromJson.f, json);
