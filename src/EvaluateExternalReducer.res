let f = (
  execution: Execution.t,
  definitions: DefinitionMap.t,
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
      SourceResolveValue.f(
        scope,
        Belt.Map.getExn(connections, {node: source.node, nib: NibConnection(nibID)}),
        execution,
        definitions,
      )
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
