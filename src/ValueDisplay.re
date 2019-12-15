let rec definedValueDisplay =
        (definedValue: Value.definedValueRecord, definitions: DefinitionMap.t)
        : string => {
  let definition =
    Belt.Map.String.getExn(definitions, definedValue.definitionID);
  TranslatableGetText.f(definition.documentation.name, "en")
  ++ (
    switch (definedValue.value) {
    | LabeledValue(None) => ""
    | LabeledValue(Some(value)) => " (" ++ f(value, definitions) ++ ")"
    | FunctionPointerValue => " pointer"
    | _ => "TODO"
    }
  );
}
and f = (value: Value.t, definitions: DefinitionMap.t): string =>
  switch (value) {
  | PrimitiveValue(primitiveValue) => PrimitiveValueDisplay.f(primitiveValue)
  | DefinedValue(definedValue) =>
    definedValueDisplay(definedValue, definitions)
  | LazyValue(_) => "(not computed yet)"
  };
