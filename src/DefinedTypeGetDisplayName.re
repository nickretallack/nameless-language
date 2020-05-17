let f = (definition: Definition.t, language: LanguageName.t): string =>
  TranslatableGetText.f(definition.documentation.name, language)
  ++ " "
  ++ (
    switch (definition.implementation) {
    | RecordTypeImplementation(_) => "Record"
    | InterfaceImplementation(_) => "Function"
    | SymbolImplementation => "Symbol"
    | LabeledTypeImplementation(_) => "Label"
    | UnionTypeImplementation(_) => "Union"
    | _ => raise(Exception.ShouldntHappen("DefinedTypeGetDisplayName called on unknown type"))
    }
  );
