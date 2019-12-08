let f =
    (
      id: DefinitionID.t, /* only used for labels */
      implementation: Implementation.t,
      display: DefinitionDisplay.t,
      dependencies: PublishingDependencyMap.t,
    )
    : string =>
  Json.stringify(
    switch (implementation) {
    | GraphImplementation(graphImplementation) =>
      GraphImplementationToCanonicalString.f(
        graphImplementation,
        dependencies,
        display,
      )
    | ConstantImplementation(primitiveValue) =>
      ConstantImplementationToPublishingJson.f(primitiveValue)
    | RecordTypeImplementation(typedFields) =>
      RecordTypeToPublishingJson.f(
        typedFields,
        dependencies,
        display.inputOrdering,
      )
    | UnionTypeImplementation(typedFields) =>
      UnionTypeToPublishingJson.f(typedFields, dependencies)
    | InterfaceImplementation(interface) =>
      InterfaceToPublishingJson.f(interface, dependencies, display)
    | ExternalImplementation(externalImplementation) =>
      ExternalImplementationToPublishingJson.f(
        externalImplementation,
        dependencies,
        display,
      )
    | LabeledTypeImplementation(valueType) =>
      LabelTypeToPublishingJson.f(id, valueType, dependencies)
    },
  );
