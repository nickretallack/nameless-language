let rec f = (execution: Execution.t, scopeID: ScopeID.t): DefinitionID.t => {
    let scope = Belt.Map.String.getExn(execution.scopes, scopeID)
    switch scope.scopeType {
        | GraphScope => scope.definitionID
        | InlineScope({parentScope: scopeID}) => f(execution, scopeID)
    }
}


