@react.component
let make = (
  ~scopeID: ScopeID.t,
  ~scope: Scope.t,
  ~definitions: DefinitionMap.t,
  ~languageName: LanguageName.t,
  ~execution: Execution.t,
) => {
  let definition = Belt.Map.String.getExn(definitions, scope.definitionID)
  let name = DefinitionGetDisplayName.f(definition, languageName)
  <div>
    <a href={`#${scope.definitionID}/implementation/${scopeID}`}> {React.string(name)} </a>
    <div className="interface">
      <div> <h2> {React.string("Outputs:")} </h2> </div>
      <div>
        <h2> {React.string("Inputs:")} </h2>
        <ExecutionScopeValues
          definitions
          languageName
          ordering=definition.display.inputOrdering
          nibDocumentations=definition.documentation.inputs
          sourceValues=scope.sourceValues
          execution
        />
      </div>
    </div>
  </div>
}
