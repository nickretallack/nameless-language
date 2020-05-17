type t =
  | ConstantImplementation(PrimitiveValue.t)
  | InterfaceImplementation(Interface.t)
  | ExternalImplementation(ExternalImplementation.t)
  | GraphImplementation(GraphImplementation.t)
  | RecordTypeImplementation(RecordType.t)
  | SymbolImplementation
  | LabeledTypeImplementation(ValueType.t)
  | UnionTypeImplementation(TypeSet.t);
