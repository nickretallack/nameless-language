@react.component
let make = (
  ~definitions: DefinitionMap.t,
  ~interface: Interface.t,
  ~documentation: Documentation.t,
  ~display: DefinitionDisplay.t,
  ~languageName: LanguageName.t,
  ~emit,
) =>
  <div className="interface">
    <div>
      <h2> {React.string("Outputs:")} </h2>
      <RecordTypeView
        typedFields=interface.output
        nibDocumentations=documentation.outputs
        isInput=false
        ordering=display.outputOrdering
        definitions
        emit
        languageName
      />
      <a onClick={_event => emit(AddOutput)}> {React.string("Add Output")} </a>
    </div>
    <div>
      <h2> {React.string("Inputs:")} </h2>
      <RecordTypeView
        typedFields=interface.input
        nibDocumentations=documentation.inputs
        isInput=true
        ordering=display.inputOrdering
        definitions
        emit
        languageName
      />
      <a onClick={_event => emit(AddInput)}> {React.string("Add Input")} </a>
    </div>
  </div>
