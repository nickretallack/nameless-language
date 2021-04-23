let f = (json: Js.Json.t): Translatable.t => {
  open Json.Decode
  open Translatable
  {
    sourceLanguage: field("sourceLanguage", string, json),
    translations: field("translations", StringMapFromJson.f(VettableFromJson.f), json),
  }
}
