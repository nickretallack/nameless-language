let f = (definition: Definition.t, language: LanguageName.t): string =>
  TranslatableGetText.f(definition.documentation.name, language)
  ++ " "
  ++ (
    switch (definition.implementation) {
    | RecordTypeImplementation(_) => "Record"
    | InterfaceImplementation(_) => "Function"
    | _ => raise(Not_found)
    }
  );
