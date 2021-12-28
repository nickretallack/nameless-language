let f = (
  value: Value.t,
  execution: Execution.t,
  source: ConnectionSide.t,
  state: AppState.t,
): AppState.t => {
  let frame = Belt.List.headExn(execution.stack)
  let scope = Belt.Map.String.getExn(execution.scopes, frame.scopeID)
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
          sourceValues: Belt.Map.set(scope.sourceValues, source, value),
        },
      ),
    }),
  }
}
