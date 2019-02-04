[%%debugger.chrome];
open Definition;

let component = ReasonReact.statelessComponent("InterfaceImplementation");

let make =
    (
      ~definitions: definitions,
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
      <Interface
        interface=implementation
        definitions
        documentation
        display
        emit
      />
    </div>;
  },
};
