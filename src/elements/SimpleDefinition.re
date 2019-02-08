[%%debugger.chrome];
open Definition;

let component = ReasonReact.statelessComponent("SimpleDefinition");

let make =
    (
      ~definitionID,
      ~definition: definition,
      ~definitions: definitions,
      ~error: AppActions.appError,
      ~emit,
      _children,
    ) => {
  ...component,
  render: _self => {
    let {documentation, display, implementation} = definition;
    <div>
      <DefinitionHeader
        definitionID
        definitions
        documentation
        emit
        error
        placeholder={
          "(nameless " ++ implementationName(implementation) ++ ")"
        }
      />
      {switch (implementation) {
       | ConstantImplementation(implementation) =>
         <ConstantImplementation implementation emit />
       | RecordTypeImplementation(implementation) =>
         <RecordTypeImplementation
           definitions
           implementation
           documentation
           display
           emit
         />
       | InterfaceImplementation(implementation) =>
         <InterfaceImplementation
           definitions
           implementation
           documentation
           display
           emit
         />
       | _ => ReasonReact.string("TODO")
       }}
    </div>;
  },
};
