@react.component
let rec make = (
  ~execution: Execution.t,
  ~definitions: DefinitionMap.t,
  ~languageName: LanguageName.t,
  ~scopeID: ScopeID.t,
  ~scope: Scope.t,
   ~scopeByParent: Belt.Map.String.t<Belt.List.t<(ScopeID.t, Scope.t)>>,
) => {
  <div>
    <ExecutionScopeView scopeID scope definitions languageName execution />
    <div className="child-scopes">
      {RenderList.f(Belt.Map.String.getWithDefault(scopeByParent, scopeID, list{}), (
        _,
        (scopeID, scope),
      ) => {
        React.createElement(make, makeProps(
            ~scopeID=scopeID, ~scope=scope, ~definitions=definitions, ~languageName=languageName, ~execution=execution, 
            ~scopeByParent=scopeByParent, ~key=scopeID, ()))
      })}
    </div>
  </div>
}
