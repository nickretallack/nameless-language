[@react.component]
let make =
    (
      ~definitionID,
      ~definition: Definition.t,
      ~definitions: DefinitionMap.t,
      ~languageName: LanguageName.t,
      ~error: AppError.t,
      ~emit,
    ) => {
  let Definition.{documentation, display, implementation} = definition;
  <div>
    <DefinitionHeaderView
      definitionID
      definitions
      documentation
      languageName
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
         languageName
         emit
       />
     | InterfaceImplementation(implementation) =>
       <InterfaceImplementationView
         definitions
         implementation
         documentation
         display
         languageName
         emit
       />
     | LabeledTypeImplementation(wrappedType) =>
       <LabeledTypeImplementationView
         definitions
         wrappedType
         languageName
         emit
       />
     | _ => ReasonReact.string("TODO")
     }}
  </div>;
};
