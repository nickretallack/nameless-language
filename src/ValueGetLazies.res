let rec f = (value: Value.t, execution: Execution.t, definitions: DefinitionMap.t): list<
  Value.lazyValue,
> =>
  switch value {
  | PrimitiveValue(_)
  | Prerequisite
  | ScheduledEventIdentifier(_)
  | HTMLElement(_)
  | InlineFunction(_) => list{}
  | LazyValue(lazyValue) =>
    switch ValueResolve.resolveLazyValue(lazyValue, definitions, execution.scopes) {
    | Some(value) => f(value, execution, definitions)
    | None => list{lazyValue}
    }
  | DefinedValue(definedValue) =>
    switch definedValue.value {
    | SymbolValue
    | FunctionPointerValue => list{}
    | LabeledValue(value) => f(value, execution, definitions)
    | RecordValue(values) =>
      Belt.List.reduce(Belt.Map.String.toList(values), list{}, (result, (_, value)) =>
        Belt.List.concat(result, f(value, execution, definitions))
      )
    }
  | Reference(referenceID) =>
    f(Belt.Map.String.getExn(execution.references, referenceID), execution, definitions)
  }
