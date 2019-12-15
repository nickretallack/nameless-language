[@react.component]
let make =
    (
      ~definitions: DefinitionMap.t,
      ~implementation: RecordType.t,
      ~documentation: Documentation.t,
      ~display: DefinitionDisplay.t,
      ~languageName: LanguageName.t,
      ~emit,
    ) => {
  <div>
    <h1> {ReasonReact.string("Record")} </h1>
    <h2> {ReasonReact.string("Fields:")} </h2>
    <RecordTypeView
      typedFields=implementation
      nibDocumentations={documentation.inputs}
      isInput=true
      ordering={display.inputOrdering}
      definitions
      languageName
      emit
    />
    <a onClick={_event => emit(AddInput)}>
      {ReasonReact.string("Add Field")}
    </a>
  </div>;
};
