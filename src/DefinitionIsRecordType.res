let f = (definition: Definition.t): bool =>
  switch definition.implementation {
  | RecordTypeImplementation(_) => true
  | _ => false
  }
