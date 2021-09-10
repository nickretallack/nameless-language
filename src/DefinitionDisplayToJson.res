let f = (display: DefinitionDisplay.t): Js.Json.t => {
  open Json.Encode
  object_(list{
    ("inputOrdering", array(string, display.inputOrdering)),
    ("outputOrdering", array(string, display.outputOrdering)),
  })
}
