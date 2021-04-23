[@react.component]
let make =
    (
      ~definitions: DefinitionMap.t,
      ~implementation: Interface.t,
      ~documentation: Documentation.t,
      ~display: DefinitionDisplay.t,
      ~languageName: LanguageName.t,
      ~emit,
    ) => {
  <div>
    <h1> {React.string("Interface")} </h1>
    <InterfaceView
      interface=implementation
      definitions
      documentation
      display
      emit
      languageName
    />
  </div>;
};
