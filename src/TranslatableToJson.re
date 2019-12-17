let f = (translatable: Translatable.t): Js.Json.t =>
  Json.Encode.(
    object_([
      ("sourceLanguage", string(translatable.sourceLanguage)),
      (
        "translations",
        MapToJson.f(translatable.translations, VettableToJson.f),
      ),
    ])
  );
