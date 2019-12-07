let f = (valueType: ValueType.t, definitionID: DefinitionID.t) =>
  switch (valueType) {
  | PrimitiveValueType(_)
  | AnyType => false
  | DefinedValueType(theDefinitionID) => definitionID == theDefinitionID
  };
