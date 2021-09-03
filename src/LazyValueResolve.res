let f = (
  lazyValue: Value.lazyValue,
  definitions: DefinitionMap.t,
  scopes: Belt.Map.String.t<Nameless.Scope.t>,
): option<Value.t> => {
  let scope = Belt.Map.String.getExn(scopes, lazyValue.scopeID)
  let source = if lazyValue.explicitConnectionSide.isSource {
    lazyValue.explicitConnectionSide.connectionSide
  } else {
    let definition = Belt.Map.String.getExn(definitions, scope.definitionID)
    let graphImplementation = switch definition.implementation {
    | GraphImplementation(graphImplementation) => graphImplementation
    | _ => raise(Exception.ShouldntHappen("lazy values must come from graphs"))
    }
    Belt.Map.getExn(
      graphImplementation.connections,
      lazyValue.explicitConnectionSide.connectionSide,
    )
  }
  Belt.Map.get(scope.sourceValues, source)
}
