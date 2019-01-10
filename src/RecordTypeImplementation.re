open Definition;

let component = ReasonReact.statelessComponent("RecordTypeImplementation");

let make =
    (
      ~definitions: definitions,
      ~implementation: typedFields,
      ~documentation: documentation,
      ~display: display,
      ~emit,
      _children,
    ) => {
  ...component,
  render: _self => {
    <div>
      <h1> {ReasonReact.string("Record")} </h1>
      <h2> {ReasonReact.string("Fields:")} </h2>
      <TypedFields
        typedFields=implementation
        nibDocumentations={documentation.inputs}
        isInput=true
        ordering={display.inputOrdering}
        definitions
        emit
      />
    </div>;
  },
};
