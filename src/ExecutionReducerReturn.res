let f = (
  value: Value.t,
  execution: Execution.t,
  source: ConnectionSide.t,
  state: AppState.t,
  urlHash,
): ReactUpdate.update<AppAction.t, AppState.t> => ReactUpdate.UpdateWithSideEffects(
  ExecutionReducerReturnState.f(value, execution, source, state),
  ExecutionReducerSideEffects.f(urlHash),
)
