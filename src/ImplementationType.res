type t =
  | GraphImplementationType
  | ExternalImplementationType
  | LabeledTypeImplementationType
  | RecordTypeImplementationType
  | InterfaceImplementationType
  | UnionTypeImplementationType
  | ConstantImplementationType
  | SymbolTypeImplementationType

let list = list{
  GraphImplementationType,
  ExternalImplementationType,
  LabeledTypeImplementationType,
  RecordTypeImplementationType,
  InterfaceImplementationType,
  UnionTypeImplementationType,
  ConstantImplementationType,
  SymbolTypeImplementationType,
}
