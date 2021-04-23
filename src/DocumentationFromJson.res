let f = (json: Js.Json.t): Documentation.t => {
  open Json.Decode
  open Documentation
  {
    name: field("name", TranslatableFromJson.f, json),
    description: field("description", TranslatableFromJson.f, json),
    inputs: field("inputs", StringMapFromJson.f(TranslatableFromJson.f), json),
    outputs: field("outputs", StringMapFromJson.f(TranslatableFromJson.f), json),
  }
}
