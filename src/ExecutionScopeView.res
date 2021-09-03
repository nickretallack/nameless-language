@react.component
let make = (
  ~scopeID: ScopeID.t,
  ~scope: Scope.t,
  ~definitions: DefinitionMap.t,
  ~languageName: LanguageName.t,
  ~execution: Execution.t,
) => {
  switch scope.scopeType {
  | GraphScope => <ExecutionGraphScopeView scopeID scope definitions languageName execution />
  | InlineScope({nodeID}) =>
    <ExecutionInlineScopeView scopeID scope definitions languageName execution nodeID />
  }
}
