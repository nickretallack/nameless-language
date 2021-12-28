let rec definedValueDisplay = (
  definedValue: Value.definedValueRecord,
  execution: Execution.t,
  definitions: DefinitionMap.t,
  languageName: LanguageName.t,
): string => {
  let definition = Belt.Map.String.getExn(definitions, definedValue.definitionID)
  TranslatableGetText.f(definition.documentation.name, languageName) ++
  switch definedValue.value {
  | SymbolValue => ""
  | LabeledValue(value) => " (" ++ (f(value, execution, definitions, languageName) ++ ")")
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
          f(value, execution, definitions, languageName))
        ),
      ),
    ) ++
    "}")
  }
}
and f = (
  value: Value.t,
  execution: Execution.t,
  definitions: DefinitionMap.t,
  language: LanguageName.t,
): string =>
  switch value {
  | PrimitiveValue(primitiveValue) => PrimitiveValueDisplay.f(primitiveValue)
  | DefinedValue(definedValue) =>
    definedValueDisplay(definedValue, execution, definitions, language)
  | LazyValue(lazyValue) =>
    switch ValueResolve.resolveLazyValue(lazyValue, definitions, execution.scopes) {
    | Some(value) => f(value, execution, definitions, language)
    | None => "(not computed yet)"
    }
  | Prerequisite => "(nothing)"
  | InlineFunction(_) => "(inline function)"
  | Reference(referenceID) =>
    `Reference to ${f(
        Belt.Map.String.getExn(execution.references, referenceID),
        execution,
        definitions,
        language,
      )}`
  | HTMLElement(_) => "(HTML element)"
  | ScheduledEventIdentifier(_) => "(scheduled event identifier)"
  }
