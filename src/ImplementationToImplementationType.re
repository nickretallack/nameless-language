let f = (implementation: Implementation.t): ImplementationType.t =>
  switch (implementation) {
  | ConstantImplementation(_) => ImplementationType.ConstantImplementationType
  | InterfaceImplementation(_) => ImplementationType.InterfaceImplementationType
  | ExternalImplementation(_) => ImplementationType.ExternalImplementationType
  | GraphImplementation(_) => ImplementationType.GraphImplementationType
  | RecordTypeImplementation(_) => ImplementationType.RecordTypeImplementationType
  | LabeledTypeImplementation(_) => ImplementationType.LabeledTypeImplementationType
  | UnionTypeImplementation(_) => ImplementationType.UnionTypeImplementationType
  };
