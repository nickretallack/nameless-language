let f = (
  inputs: Belt.Map.String.t<option<Value.t>>,
  outputID: string,
): ExternalEvaluationResult.t => {
  if outputID != ";" {
    raise(Not_found)
  }

  switch Belt.Map.String.getExn(inputs, "handler") {
  | None => ExternalEvaluationResult.EvaluationRequired(list{"handler"})
  | Some(handler) =>
    switch handler {
    | InlineFunction(inlineFunctionContext) =>
      SideEffect(
        Some(Prerequisite),
        (webView, {send}: ReactUpdate.self<AppAction.t, AppState.t>) => {
          let element = switch Js.Nullable.toOption(webView.current) {
          | Some(element) => element
          | None =>
            raise(
              Exception.ShouldntHappen("Add Event Listener acted before the web view was ready"),
            )
          }

          Webapi.Dom.Element.addEventListener(
            "keydown",
            event =>
              send(
                AppAction.QueueEvaluation({
                  definitionID: "keyboardEventListener",
                  inputs: Belt.Map.fromArray(
                    [
                      (
                        {
                          ConnectionSide.node: NodeConnection(inlineFunctionContext.nodeID),
                          nib: NibConnection("code"),
                        },
                        Value.PrimitiveValue(TextValue(EventGetCode.f(event))),
                      ),
                    ],
                    ~id=module(ConnectionSideComparable.C),
                  ),
                  connectionNib: NibConnection(";"),
                  inlineScope: inlineFunctionContext,
                }),
              ),
            element,
          )
          None
        },
      )

    | _ =>
      raise(
        Exception.TypeError("Add Keyboard Event Handler received something that wasn't a handler"),
      )
    }
  }
}
