let rec definedValueDisplay = (
  definedValue: Value.definedValueRecord,
  definitions: DefinitionMap.t,
  languageName: LanguageName.t,
): string => {
  let definition = Belt.Map.String.getExn(definitions, definedValue.definitionID)
  TranslatableGetText.f(definition.documentation.name, languageName) ++
  switch definedValue.value {
  | SymbolValue => ""
  | LabeledValue(value) => " (" ++ (f(value, definitions, languageName) ++ ")")
  | FunctionPointerValue => " pointer"
  | RecordValue(values) =>
    "{" ++
    (Js.Array.joinWith(
      ", \n",
      Belt.List.toArray(
        Belt.List.map(Belt.Map.String.toList(values), ((nibID, value)) =>
          TranslatableGetText.f(
            Belt.Map.String.getExn(definition.documentation.inputs, nibID),
            languageName,
          ) ++
          (": " ++
          f(value, definitions, languageName))
        ),
      ),
    ) ++
    "}")
  }
}
and f = (value: Value.t, definitions: DefinitionMap.t, language: LanguageName.t): string =>
  switch value {
  | PrimitiveValue(primitiveValue) => PrimitiveValueDisplay.f(primitiveValue)
  | DefinedValue(definedValue) => definedValueDisplay(definedValue, definitions, language)
  | LazyValue(_) => "(not computed yet)"
  }
