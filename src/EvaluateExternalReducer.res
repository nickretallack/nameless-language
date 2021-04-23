let f = (
  execution: Execution.t,
  scope: Scope.t,
  frame: StackFrame.t,
  source: ConnectionSide.t,
  connections: ConnectionMap.t,
  externalImplementation: ExternalImplementation.t,
  outputID: NibID.t,
) =>
  switch EvaluateExternalFunction.f(
    externalImplementation.name,
    outputID,
    Belt.Map.String.mapWithKey(externalImplementation.interface.input, (nibID, _) =>
      Belt.Map.get(
        scope.sourceValues,
        Belt.Map.getExn(connections, {node: source.node, nib: NibConnection(nibID)}),
      )
    ),
  ) {
  | EvaluationResult(value) => {
      ...execution,
      stack: list{{...frame, action: Returning(value)}, ...Belt.List.tailExn(execution.stack)},
      scopes: Belt.Map.String.set(
        execution.scopes,
        frame.scopeID,
        {
          ...scope,
          sourceValues: Belt.Map.set(scope.sourceValues, source, value),
        },
      ),
    }
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
