let f = (definition: Definition.t): bool =>
  switch (definition.implementation) {
  | LabeledTypeImplementation(_) => true
  | _ => false
  };
