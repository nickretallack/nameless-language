@react.component
let make = (
  ~execution: option<Execution.t>,
  ~definitions: DefinitionMap.t,
  ~languageName: LanguageName.t,
) => {
  switch execution {
  | Some(execution) =>
    <div>
      {RenderList.f(Belt.Map.String.toList(execution.scopes), (_, (scopeID, scope)) => {
        <ExecutionScopeView scopeID scope definitions languageName key=scopeID />
      })}
    </div>
  | None => React.null
  }
}
