let f = (inputs: Belt.Map.String.t<option<Value.t>>, outputID: string) => {
  if outputID != "result" {
    raise(Not_found)
  }

  EvaluateWithAllValues.f(inputs, values => {
    let left = Belt.Map.String.getExn(values, "left")
    let right = Belt.Map.String.getExn(values, "right")
    let equal = left == right
    DefinedValue({
      definitionID: equal ? "yes" : "no",
      value: Value.SymbolValue,
    })
  })
}
