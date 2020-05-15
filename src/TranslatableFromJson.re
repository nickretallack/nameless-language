let f = (json: Js.Json.t): Translatable.t =>
  Json.Decode.(
    Translatable.{
      sourceLanguage: field("sourceLanguage", string, json),
      translations:
        field("translations", StringMapFromJson.f(VettableFromJson.f), json),
    }
  );
