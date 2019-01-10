open Definition;

let component = ReasonReact.statelessComponent("InterfaceImplementation");

let make =
    (
      ~implementation: interface,
      ~documentation: documentation,
      ~display: display,
      ~emit,
      _children,
    ) => {
  ...component,
  render: _self => {
    <div>
      <h1> {ReasonReact.string("Interface")} </h1>
      <h2> {ReasonReact.string("Inputs:")} </h2>
      <TypedFields
        typedFields={implementation.inputTypes}
        nibDocumentations={documentation.inputs}
        isInput=true
        ordering={display.inputOrdering}
        emit
      />
      <h2> {ReasonReact.string("Outputs:")} </h2>
      <TypedFields
        typedFields={implementation.outputTypes}
        nibDocumentations={documentation.outputs}
        isInput=false
        ordering={display.outputOrdering}
        emit
      />
    </div>;
  },
};
