let f = (
  execution: Execution.t,
  scope: Scope.t,
  frame: StackFrame.t,
  source: ConnectionSide.t,
  connections: ConnectionMap.t,
  externalImplementation: ExternalImplementation.t,
  outputID: NibID.t,
  webView,
) =>
  switch EvaluateExternalFunction.f(
    externalImplementation.name,
    outputID,
    Belt.Map.String.mapWithKey(externalImplementation.interface.input, (nibID, _) => {
      let value = Belt.Map.get(
        scope.sourceValues,
        Belt.Map.getExn(connections, {node: source.node, nib: NibConnection(nibID)}),
      )
      // try to resolve lazy values
      switch value {
      | Some(LazyValue(lazyValue)) =>
        switch Belt.Map.get(
          scope.sourceValues,
          Belt.Map.getExn(connections, lazyValue.connectionSide),
        ) {
        | Some(value) => Some(value) // found the real value
        | None => value // just pass through the un-evaluated value
        }
      | value => value
      }
    }),
    webView,
  ) {
  | EvaluationResult(value) => ExecutionReducerReturn.f(value, execution, source)
  | EvaluationRequired(nibIDs) => {
      ...execution,
      stack: Belt.List.concat(
        Belt.List.map(nibIDs, nibID => {
          ...frame,
          explicitConnectionSide: {
            isSource: false,
            connectionSide: {
              node: source.node,
              nib: NibConnection(nibID),
            },
          },
          action: Evaluating,
        }),
        execution.stack,
      ),
    }
  }
