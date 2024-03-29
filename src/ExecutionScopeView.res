@react.component
let make = (
  ~scopeID: ScopeID.t,
  ~scope: Scope.t,
  ~definitions: DefinitionMap.t,
  ~languageName: LanguageName.t,
  ~execution: Execution.t,
) => {
  let nodeConnection = switch scope.scopeType {
  | GraphScope => ConnectionNode.GraphConnection
  | InlineScope({nodeID}) => NodeConnection(nodeID)
  }

  let graphDefinitionID = ScopeGetGraphDefinitionID.f(execution, scopeID)
  let graphDefinition = Belt.Map.String.getExn(definitions, graphDefinitionID)
  let graphImplementation = switch graphDefinition.implementation {
  | GraphImplementation(graphImplementation) => graphImplementation
  | _ => raise(Exception.ShouldntHappen("Scope view only works for graphs"))
  }
  let definition = Belt.Map.String.getExn(definitions, scope.definitionID)
  let name = DefinitionGetDisplayName.f(definition, languageName)
  <div>
    <a href={`#${graphDefinitionID}/implementation/${scopeID}`}> {React.string(name)} </a>
    <div className="interface">
      <div>
        <h2> {React.string("Outputs:")} </h2>
        <ExecutionScopeValues
          definitions
          languageName
          ordering=definition.display.outputOrdering
          nibDocumentations=definition.documentation.outputs
          scope
          execution
          nibToSource={nibID =>
            Belt.Map.getExn(
              graphImplementation.connections,
              {node: nodeConnection, nib: NibConnection(nibID)},
            )}
        />
      </div>
      <div>
        <h2> {React.string("Inputs:")} </h2>
        <ExecutionScopeValues
          definitions
          languageName
          ordering=definition.display.inputOrdering
          nibDocumentations=definition.documentation.inputs
          scope
          execution
          nibToSource={nibID => {node: nodeConnection, nib: NibConnection(nibID)}}
        />
      </div>
    </div>
  </div>
}
