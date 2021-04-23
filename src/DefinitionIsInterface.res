let f = (definition: Definition.t): bool =>
  switch definition.implementation {
  | InterfaceImplementation(_) => true
  | _ => false
  }
