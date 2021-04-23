let f = (definition: Definition.t): bool =>
  switch definition.implementation {
  | UnionTypeImplementation(_) => true
  | _ => false
  }
