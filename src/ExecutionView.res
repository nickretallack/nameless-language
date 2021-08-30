@react.component
let make = (
  ~execution: option<Execution.t>,
  ~definitions: DefinitionMap.t,
  ~languageName: LanguageName.t,
) => {
  switch execution {
  | Some(execution) =>
    <div>
      {RenderList.f(Belt.Map.String.toList(execution.scopes), (_, (_scopeID, scope)) => {
        <ExecutionScopeView scope definitions languageName />
      })}
    </div>
  | None => React.null
  }
}
