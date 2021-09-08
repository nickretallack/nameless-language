let f = (
  inputs: Belt.Map.String.t<option<Value.t>>,
  _outputID: string,
): ExternalEvaluationResult.t =>
  EvaluateWithAllValues.f(inputs, values => {
    let element = switch Belt.Map.String.getExn(values, "element") {
    | HTMLElement(element) => element
    | _ => raise(Exception.TypeError("htmlSetCssProperty.element expected an HTML element"))
    }
    let property = switch Belt.Map.String.getExn(values, "property") {
    | PrimitiveValue(primitive) => PrimitiveValueToString.f(primitive)
    | _ => raise(Exception.TypeError("htmlSetCssProperty.property expected text"))
    }
    let value = switch Belt.Map.String.getExn(values, "value") {
    | PrimitiveValue(primitive) => PrimitiveValueToString.f(primitive)
    | _ => raise(Exception.TypeError("htmlSetCssProperty.value expected text"))
    }
    let style = ElementGetStyleDict.f(element)
    Js.Dict.set(style, property, value)
    EvaluationResult(Prerequisite)
  })
