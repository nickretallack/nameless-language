let f = (valueType: ValueType.t, definitions: DefinitionMap.t): TypeSelectorCategory.t =>
  switch valueType {
  | PrimitiveValueType(primitiveValueType) =>
    switch primitiveValueType {
    | NumberType => NumberCategory
    | IntegerType => NumberCategory
    | TextType => TextCategory
    }
  | DefinedValueType(definitionID) =>
    let definition = Belt.Map.String.getExn(definitions, definitionID)
    switch definition.implementation {
    | InterfaceImplementation(_) => FunctionCategory
    | RecordTypeImplementation(_) => RecordCategory
    | LabeledTypeImplementation(_) => LabelCategory
    | SymbolImplementation => SymbolCategory
    | UnionTypeImplementation(_) => UnionCategory
    | _ => raise(Exception.TODO("TypeToCategory: unknown cateory"))
    }
  | AnyType => AnyCategory
  | SequencerType => AnyCategory // TODO
  }
