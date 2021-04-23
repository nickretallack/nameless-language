let f = (documentation: Documentation.t): Js.Json.t => {
  open Json.Encode
  object_(list{
    ("name", TranslatableToJson.f(documentation.name)),
    ("description", TranslatableToJson.f(documentation.description)),
    ("inputs", StringMapToJson.f(documentation.inputs, TranslatableToJson.f)),
    ("outputs", StringMapToJson.f(documentation.outputs, TranslatableToJson.f)),
  })
}
