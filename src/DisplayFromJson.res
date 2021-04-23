let f = (json: Js.Json.t): DefinitionDisplay.t => {
  open Json.Decode
  open DefinitionDisplay
  {
    inputOrdering: field("inputOrdering", list(string), json),
    outputOrdering: field("outputOrdering", list(string), json),
  }
}
