let f = (json: Js.Json.t): Definition.t => {
  open Json.Decode
  open Definition
  {
    implementation: field("implementation", ImplementationFromJson.f, json),
    documentation: field("documentation", DocumentationFromJson.f, json),
    display: field("display", DisplayFromJson.f, json),
  }
}
