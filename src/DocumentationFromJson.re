let f = (json: Js.Json.t): Documentation.t =>
  Json.Decode.(
    Documentation.{
      name: field("name", TranslatableFromJson.f, json),
      description: field("description", TranslatableFromJson.f, json),
      inputs:
        field("inputs", StringMapFromJson.f(TranslatableFromJson.f), json),
      outputs:
        field("outputs", StringMapFromJson.f(TranslatableFromJson.f), json),
    }
  );
