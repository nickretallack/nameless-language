let f = (display: DefinitionDisplay.t): Js.Json.t => {
  open Json.Encode
  object_(list{
    ("inputOrdering", list(string, display.inputOrdering)),
    ("outputOrdering", list(string, display.outputOrdering)),
  })
}
