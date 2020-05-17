let f = (definition: Definition.t): bool =>
  switch (definition.implementation) {
  | LabeledTypeImplementation(wrappedType) => Belt.Option.isSome(wrappedType)
  | _ => false
  };
