let f = (
  inputs: Belt.Map.String.t<option<Value.t>>,
  _outputID: string,
): ExternalEvaluationResult.t => {
  EvaluateWithAllValues.f(inputs, values => {
    let handler = Belt.Map.String.getExn(values, "handler")
    switch handler {
    | InlineFunction(inlineFunctionContext) =>
      SideEffect(
        None,
        (_webView, callingContext, {send}: ReactUpdate.self<AppAction.t, AppState.t>) => {
          let eventID = ScheduledEventIdentifier.AnimationFrameRequestID(
            AnimationFrame.request(() => {
              send(
                AppAction.QueueEvaluation({
                  definitionID: "scheduledEventHandler",
                  inputs: Belt.Map.fromArray([], ~id=module(ConnectionSideComparable.C)),
                  connectionNib: NibConnection(";"),
                  inlineFunctionContext: inlineFunctionContext,
                  callingContext: callingContext,
                }),
              )
            }),
          )
          send(
            AppAction.AddScheduledEvent({
              eventID: eventID,
              values: Belt.Map.String.fromArray([
                (";", Value.Prerequisite),
                ("identifier", Value.ScheduledEventIdentifier(eventID)),
              ]),
            }),
          )
          None
        },
      )

    | _ =>
      raise(
        Exception.TypeError("Schedule Recurring Event received something that wasn't a handler"),
      )
    }
  })
}
