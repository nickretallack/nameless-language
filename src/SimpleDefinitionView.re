[@react.component]
let make =
    (
      ~definitionID,
      ~definition: Definition.t,
      ~definitions: DefinitionMap.t,
      ~error: AppError.t,
      ~emit,
    ) => {
  let Definition.{documentation, display, implementation} = definition;
  <div>
    <DefinitionHeaderView
      definitionID
      definitions
      documentation
      emit
      error
      placeholder={
        "(nameless " ++ ImplementationGetName.f(implementation) ++ ")"
      }
    />
    {switch (implementation) {
     | ConstantImplementation(implementation) =>
       <ConstantImplementationView implementation emit />
     | RecordTypeImplementation(implementation) =>
       <RecordTypeImplementationView
         definitions
         implementation
         documentation
         display
         emit
       />
     | InterfaceImplementation(implementation) =>
       <InterfaceImplementationView
         definitions
         implementation
         documentation
         display
         emit
       />
     | _ => ReasonReact.string("TODO")
     }}
  </div>;
};
