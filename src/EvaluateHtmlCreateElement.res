let f = (
  inputs: Belt.Map.String.t<option<Value.t>>,
  _outputID: string,
): ExternalEvaluationResult.t =>
  switch Belt.Map.String.getExn(inputs, "tagName") {
  | None => EvaluationRequired(list{"tagName"})
  | Some(Value.PrimitiveValue(TextValue(tagName))) =>
    let element = Webapi.Dom.Document.createElement(tagName, Webapi.Dom.document)
    SideEffect(
      Some(HTMLElement(element)),
      (webView, _) => {
        let webViewElement = Belt.Option.getUnsafe(Js.Nullable.toOption(webView.current))
        Webapi.Dom.Element.appendChild(element, webViewElement)
        None
      },
    )
  | _ => raise(Exception.TypeError("htmlCreateElement expected a string"))
  }
