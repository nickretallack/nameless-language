@react.component
let make = (~scope: Scope.t, ~definitions: DefinitionMap.t, ~languageName: LanguageName.t) => {
  let definition = Belt.Map.String.getExn(definitions, scope.definitionID)
  let name = DefinitionGetDisplayName.f(definition, languageName)
  <div> <a href={`#${scope.definitionID}`}> {React.string(name)} </a> </div>
}
