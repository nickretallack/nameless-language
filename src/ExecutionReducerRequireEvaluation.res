let f = (
  state: AppState.t,
  execution: Execution.t,
  nibIDs: list<NibID.t>,
  frame: StackFrame.t,
  source: ConnectionSide.t,
  urlHash: array<string>,
) => ReactUpdate.UpdateWithSideEffects(
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
