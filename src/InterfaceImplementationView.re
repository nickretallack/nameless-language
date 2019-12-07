[@react.component]
let make =
    (
      ~definitions: DefinitionMap.t,
      ~implementation: Interface.t,
      ~documentation: Documentation.t,
      ~display: DefinitionDisplay.t,
      ~emit,
    ) => {
  <div>
    <h1> {ReasonReact.string("Interface")} </h1>
    <InterfaceView
      interface=implementation
      definitions
      documentation
      display
      emit
    />
  </div>;
};
