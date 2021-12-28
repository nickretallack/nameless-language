let f = (
  source: ConnectionSide.t,
  values: Belt.Map.String.t<Value.t>,
  execution: Execution.t,
  state: AppState.t,
  urlHash,
) => {
  let frame = Belt.List.headExn(execution.stack)
  let scope = Belt.Map.String.getExn(execution.scopes, frame.scopeID)
  let newSourceValues = ValuesMerge.f(source, values, scope.sourceValues)

  ReactUpdate.UpdateWithSideEffects(
    {
      ...state,
      execution: Some({
        ...execution,
        stack: list{{...frame, action: Returning}, ...Belt.List.tailExn(execution.stack)},
        scopes: Belt.Map.String.set(
          execution.scopes,
          frame.scopeID,
          {
            ...scope,
            sourceValues: newSourceValues,
          },
        ),
      }),
    },
    ExecutionReducerSideEffects.f(urlHash),
  )
}
