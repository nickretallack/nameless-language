type t =
  | ConstantImplementation(PrimitiveValue.t)
  | InterfaceImplementation(Interface.t)
  | ExternalImplementation(ExternalImplementation.t)
  | GraphImplementation(GraphImplementation.t)
  | RecordTypeImplementation(Record.t)
  | LabeledTypeImplementation(option(ValueType.t))
  | UnionTypeImplementation(TypeSet.t);
