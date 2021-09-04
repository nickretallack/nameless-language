let f = (
  execution: Execution.t,
  definitions: DefinitionMap.t,
  scope: Scope.t,
  frame: StackFrame.t,
  source: ConnectionSide.t,
  connections: ConnectionMap.t,
  externalImplementation: ExternalImplementation.t,
  outputID: NibID.t,
  state: AppState.t,
  webView,
  urlHash,
): ReactUpdate.update<AppAction.t, AppState.t> =>
  switch EvaluateExternalFunction.f(
    execution,
    definitions,
    state.languageName,
    externalImplementation.name,
    outputID,
    Belt.Map.String.mapWithKey(externalImplementation.interface.input, (nibID, _) => {
      switch Belt.Map.get(connections, {node: source.node, nib: NibConnection(nibID)}) {
      | None => None
      | Some(connectionSide) => SourceResolveValue.f(scope, connectionSide, execution, definitions)
      }
    }),
  ) {
  | EvaluationResult(value) => ExecutionReducerReturn.f(value, execution, source, state, urlHash)
  | SideEffect(value, sideEffect) =>
    ReactUpdate.UpdateWithSideEffects(
      ExecutionReducerReturnState.f(value, execution, source, state),
      arg => {
        let _ = ExecutionReducerSideEffects.f(urlHash, arg)
        sideEffect(webView, arg)
      },
    )
  | EvaluationRequired(nibIDs) =>
    ReactUpdate.UpdateWithSideEffects(
      {
        ...state,
        execution: Some({
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
        }),
      },
      ExecutionReducerSideEffects.f(urlHash),
    )
  }
