let f = (json: Js.Json.t): DefinitionDisplay.t => {
  open Json.Decode
  open DefinitionDisplay
  {
    inputOrdering: field("inputOrdering", array(string), json),
    outputOrdering: field("outputOrdering", array(string), json),
  }
}
