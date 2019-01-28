open Definition;

let canonicalizeImplementation =
    (
      id: definitionID, /* only used for labels */
      implementation: implementation,
      display: display,
      dependencies: publishingDependencies,
    )
    : string =>
  Json.stringify(
    switch (implementation) {
    | GraphImplementation(graphImplementation) =>
      CanonicalizeGraph.encodeCanonicalGraph(
        graphImplementation,
        dependencies,
        display,
      )
    | ConstantImplementation(primitiveValue) =>
      CanonicalizeConstant.encodeCanonicalConstant(primitiveValue)
    | RecordTypeImplementation(typedFields) =>
      CanonicalizeType.encodeCanonicalRecordType(
        typedFields,
        dependencies,
        display.inputOrdering,
      )
    | UnionTypeImplementation(typedFields) =>
      CanonicalizeType.encodeCanonicalUnionType(
        typedFields,
        dependencies,
        display.inputOrdering,
      )
    | InterfaceImplementation(interface) =>
      CanonicalizeType.encodeCanonicalInterface(
        interface,
        dependencies,
        display,
      )
    | ExternalImplementation(externalImplementation) =>
      CanonicalizeType.encodeCanonicalExternal(
        externalImplementation,
        dependencies,
        display,
      )
    | LabeledTypeImplementation(valueType) =>
      CanonicalizeType.encodeCanonicalLabeledType(id, valueType, dependencies)
    },
  );
