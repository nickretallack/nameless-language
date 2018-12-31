open Definition;

/* let encodeRecord = (typedFields: typedFields, dependencies: publishingDependencies, fieldOrdering: Belt.List.t(nibID)) => */

/* let canonicalizeRecord = (typedFields: typedFields) => {
     let (canonical, nibOrdering) =
   } */

/* let canonicalizeInterface = (interface: interface) => */

let canonicalizeImplementation =
    (
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
      CanonicalizeType.encodeCanonicalRecord(
        typedFields,
        dependencies,
        display.inputOrdering,
      )
    /* | InterfaceImplementation(interface) =>
       canonicalizeInterface(interface) */
    | _ => Json.Encode.string("todo")
    },
  );
