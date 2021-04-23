let f = (translatable: Translatable.t): Js.Json.t => {
  open Json.Encode
  object_(list{
    ("sourceLanguage", string(translatable.sourceLanguage)),
    ("translations", StringMapToJson.f(translatable.translations, VettableToJson.f)),
  })
}
