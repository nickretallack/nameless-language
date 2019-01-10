open Definition;

let component = ReasonReact.statelessComponent("SimpleDefinition");

let make = (~definition: definition, ~emit, _children) => {
  ...component,
  render: _self => {
    let {documentation, display, implementation} = definition;
    <div>
      <DefinitionHeader documentation emit />
      {switch (implementation) {
       | ConstantImplementation(implementation) =>
         <ConstantImplementation implementation documentation emit />
       | RecordTypeImplementation(implementation) =>
         <RecordTypeImplementation implementation documentation display emit />
       | InterfaceImplementation(implementation) =>
         <InterfaceImplementation implementation documentation display emit />
       | _ => ReasonReact.string("TODO")
       }}
    </div>;
  },
};
