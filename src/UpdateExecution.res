let f = (state: AppState.t, urlHash, execution: Execution.t) => ReactUpdate.UpdateWithSideEffects(
  {
    ...state,
    AppState.execution: Some(execution),
  },
  ExecutionReducerSideEffects.f(urlHash),
)
