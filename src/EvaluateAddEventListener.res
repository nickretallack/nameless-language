let f = (
  webView: React.ref<Js.Nullable.t<Dom.element>>,
  inputs: Belt.Map.String.t<option<Value.t>>,
  outputID: string,
): ExternalEvaluationResult.t => {
  if outputID != ";" {
    raise(Not_found)
  }

  EvaluateWithAllValues.f(inputs, values => {
    let event = switch Belt.Map.String.getExn(values, "event") {
    | PrimitiveValue(TextValue(text)) => text
    | _ => raise(Exception.ShouldntHappen("Add Event Listener received non-text event name"))
    }
    let element = switch Js.Nullable.toOption(webView.current) {
    | Some(element) => element
    | None =>
      raise(Exception.ShouldntHappen("Add Event Listener acted before the web view was ready"))
    }

    // let handler = Belt.Map.String.getExn(inputs, "handler")
    Webapi.Dom.Element.addEventListener(event, _ => {Js.log("hello")}, element)
    Prerequisite // TODO: handle prerequisite evaluation in the evaluator
  })
}
