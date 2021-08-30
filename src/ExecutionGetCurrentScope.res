let f = (execution: Execution.t, scopeID: option<ScopeID.t>): Scope.t =>
  Belt.Map.String.getExn(
    execution.scopes,
    switch scopeID {
    | Some(scopeID) => scopeID
    | None => Belt.List.headExn(execution.stack).scopeID
    },
  )
