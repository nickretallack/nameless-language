let f = (implementation: Implementation.t): Js.Json.t => {
  open Json.Encode
  switch implementation {
  | ConstantImplementation(primitiveValue) =>
    object_(list{("type", string("constant")), ("value", PrimitiveValueToJson.f(primitiveValue))})
  | InterfaceImplementation(interface) => InterfaceToJson.f(interface)
  | ExternalImplementation({name, interface}) =>
    object_(list{
      // can I avoid doing this?
      ("type", string("external")),
      ("name", string(name)),
      ("interface", InterfaceToJson.f(interface)),
    })
  | GraphImplementation(graphImplementation) => GraphImplementationToJson.f(graphImplementation)
  | RecordTypeImplementation(recordType) =>
    object_(list{("type", string("recordType")), ("fields", TypedFieldsToJson.f(recordType))})
  | LabeledTypeImplementation(valueType) =>
    object_(list{("type", string("label")), ("wrapped", ValueTypeToJson.f(valueType))})
  | UnionTypeImplementation(typeSet) =>
    object_(list{
      ("type", string("union")),
      ("types", array(ValueTypeToJson.f, Belt.Set.toArray(typeSet))),
    })
  | SymbolImplementation => object_(list{("type", string("symbol"))})
  }
}
