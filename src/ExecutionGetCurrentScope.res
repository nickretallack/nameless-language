let f = (execution: Execution.t): Scope.t =>
  Belt.Map.String.getExn(execution.scopes, Belt.List.headExn(execution.stack).scopeID)
