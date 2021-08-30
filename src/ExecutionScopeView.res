@react.component
let make = (
  ~scopeID: ScopeID.t,
  ~scope: Scope.t,
  ~definitions: DefinitionMap.t,
  ~languageName: LanguageName.t,
) => {
  let definition = Belt.Map.String.getExn(definitions, scope.definitionID)
  let name = DefinitionGetDisplayName.f(definition, languageName)
  <div>
    <a href={`#${scope.definitionID}/implementation/${scopeID}`}> {React.string(name)} </a>
  </div>
}
