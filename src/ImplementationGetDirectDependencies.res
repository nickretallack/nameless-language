let f = (definition: Definition.t): Belt.Set.String.t =>
  switch definition.implementation {
  | ConstantImplementation(_)
  | SymbolImplementation => Belt.Set.String.empty
  | LabeledTypeImplementation(valueType) => ValueTypeGetDirectDependencies.f(valueType)
  | UnionTypeImplementation(typeSet) =>
    Belt.Set.reduce(typeSet, Belt.Set.String.empty, (dependencies, valueType) =>
      Belt.Set.String.union(dependencies, ValueTypeGetDirectDependencies.f(valueType))
    )
  | RecordTypeImplementation(recordType) => RecordTypeGetDirectDependencies.f(recordType)
  | InterfaceImplementation(interface) => InterfaceGetDirectDependencies.f(interface)
  | ExternalImplementation({interface}) => InterfaceGetDirectDependencies.f(interface)
  | GraphImplementation({interface, nodes}) =>
    Belt.Map.String.reduce(nodes, InterfaceGetDirectDependencies.f(interface), (
      dependencies,
      _nodeID,
      node,
    ) => {
      switch node.kind {
      | DefinedNode({definitionID}) => Belt.Set.String.add(dependencies, definitionID)
      | ListNode(_) => dependencies
      }
    })
  }
