let f = (
  inputs: Belt.Map.String.t<option<Value.t>>,
  _outputID: string,
): ExternalEvaluationResult.t => {
  EvaluateWithAllValues.f(inputs, values => {
    let handler = Belt.Map.String.getExn(values, "handler")
    let interval = Belt.Map.String.getExn(values, "interval")
    switch handler {
    | InlineFunction({scopeID, nodeID}) =>
      switch interval {
      | PrimitiveValue(NumberValue(intervalValue)) =>
        SideEffect(
          None,
          (_webView, {send}: ReactUpdate.self<AppAction.t, AppState.t>) => {
            let eventID = ScheduledEventIdentifier.IntervalID(Js.Global.setIntervalFloat(() =>
                send(
                  AppAction.QueueEvaluation({
                    definitionID: "scheduledEventHandler",
                    inputs: Belt.Map.fromArray([], ~id=module(ConnectionSideComparable.C)),
                    connectionNib: NibConnection(";"),
                    inlineScope: {scopeID: scopeID, nodeID: nodeID},
                  }),
                )
              , intervalValue))
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
      | _ => raise(Exception.TypeError("Schedule Recurring Event received non-integer interval."))
      }

    | _ =>
      raise(
        Exception.TypeError("Schedule Recurring Event received something that wasn't a handler"),
      )
    }
  })
}
