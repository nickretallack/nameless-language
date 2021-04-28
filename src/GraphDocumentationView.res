@react.component
let make = (
  ~definitionID: DefinitionID.t,
  ~definitions: DefinitionMap.t,
  ~implementation: GraphImplementation.t,
  ~display: DefinitionDisplay.t,
  ~documentation: Documentation.t,
  ~languageName: LanguageName.t,
  ~emit: DefinitionAction.t => unit,
  ~error: AppError.t,
) =>
  <div id="documentation">
    <DefinitionHeaderView
      definitionID
      definitions
      documentation
      languageName
      emit
      error
      placeholder="(nameless function)"
    />
    <h2> {React.string("Interface")} </h2>
    <InterfaceView
      definitions interface=implementation.interface documentation display emit languageName
    />
  </div>
