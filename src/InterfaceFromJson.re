let f = (json: Js.Json.t): Interface.t =>
  Json.Decode.(
    Interface.{
      input: field("input", TypedFieldsFromJson.f, json),
      output: field("output", TypedFieldsFromJson.f, json),
    }
  );
