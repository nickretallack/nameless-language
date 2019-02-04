[%%debugger.chrome];
open Definition;

let component = ReasonReact.statelessComponent("Interface");

let make =
    (
      ~definitions: definitions,
      ~interface: interface,
      ~documentation: documentation,
      ~display: display,
      ~emit,
      _children,
    ) => {
  ...component,
  render: _self => {
    <div>
      <h2> {ReasonReact.string("Outputs:")} </h2>
      <TypedFields
        typedFields={interface.outputTypes}
        nibDocumentations={documentation.outputs}
        isInput=false
        ordering={display.outputOrdering}
        definitions
        emit
      />
      <a onClick={_event => emit(AddOutput)}>
        {ReasonReact.string("Add Output")}
      </a>
      <h2> {ReasonReact.string("Inputs:")} </h2>
      <TypedFields
        typedFields={interface.inputTypes}
        nibDocumentations={documentation.inputs}
        isInput=true
        ordering={display.inputOrdering}
        definitions
        emit
      />
      <a onClick={_event => emit(AddInput)}>
        {ReasonReact.string("Add Input")}
      </a>
    </div>;
  },
};
