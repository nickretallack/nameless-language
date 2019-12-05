let f = (implementation: Implementation.t): string =>
  switch (implementation) {
  | ConstantImplementation(_) => "constant"
  | InterfaceImplementation(_) => "interface"
  | ExternalImplementation(_) => "external"
  | GraphImplementation(_) => "function"
  | RecordTypeImplementation(_) => "record type"
  | LabeledTypeImplementation(_) => "labeled type"
  | UnionTypeImplementation(_) => "union type"
  };
