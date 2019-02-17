open Definition;

let primitiveValueTypeToString =
    (primitiveValueType: primitiveValueType): string =>
  switch (primitiveValueType) {
  | IntegerType => "integer"
  | NumberType => "number"
  | TextType => "text"
  };

let encodeValueType = (valueType: publishingValueType) =>
  Json.Encode.(
    switch (valueType) {
    | PublishingPrimitiveValueType(primitiveValueType) =>
      object_([
        ("type", string("primitive")),
        (
          "primitiveType",
          string(primitiveValueTypeToString(primitiveValueType)),
        ),
      ])
    | PublishingDefinedValueType(contentID) =>
      object_([
        ("type", string("defined")),
        ("contentID", string(contentID)),
      ])
    | PublishingAnyType => object_([("type", string("any"))])
    }
  );

let encodeTypedFields = (fields: Belt.List.t(publishingValueType)) =>
  Json.Encode.list(encodeValueType, fields);

let canonicalizeType =
    (valueType: valueType, dependencies: publishingDependencies)
    : publishingValueType =>
  switch (valueType) {
  | DefinedValueType(definitionID) =>
    PublishingDefinedValueType(
      Belt.Map.String.getExn(dependencies, definitionID).contentID,
    )
  | PrimitiveValueType(primitiveValueType) =>
    PublishingPrimitiveValueType(primitiveValueType)
  | AnyType => PublishingAnyType
  };

let canonicalizeTypedFields =
    (
      typedFields: typedFields,
      dependencies: publishingDependencies,
      fieldOrdering: Belt.List.t(nibID),
    )
    : Belt.List.t(publishingValueType) =>
  Belt.List.map(fieldOrdering, nibID =>
    canonicalizeType(
      Belt.Map.String.getExn(typedFields, nibID),
      dependencies,
    )
  );

let encodeRecordType = (fields: Belt.List.t(publishingValueType)): Js.Json.t =>
  Json.Encode.(
    object_([
      ("type", string("record")),
      ("fields", encodeTypedFields(fields)),
    ])
  );

let encodeUnionType = (fields: Belt.List.t(publishingValueType)): Js.Json.t =>
  Json.Encode.(
    object_([
      ("type", string("union")),
      ("types", encodeTypedFields(fields)),
    ])
  );

let encodeLabeledType =
    (id: definitionID, valueType: option(publishingValueType)): Js.Json.t =>
  Json.Encode.(
    object_([
      ("type", string("label")),
      ("id", string(id)),
      (
        "wrapped",
        switch (valueType) {
        | Some(valueType) => encodeValueType(valueType)
        | None => null
        },
      ),
    ])
  );

let encodeCanonicalRecordType =
    (
      typedFields: typedFields,
      dependencies: publishingDependencies,
      fieldOrdering: Belt.List.t(nibID),
    )
    : Js.Json.t =>
  encodeRecordType(
    canonicalizeTypedFields(typedFields, dependencies, fieldOrdering),
  );

let encodeCanonicalUnionType =
    (
      typedFields: typedFields,
      dependencies: publishingDependencies,
      fieldOrdering: Belt.List.t(nibID),
    )
    : Js.Json.t =>
  encodeUnionType(
    canonicalizeTypedFields(typedFields, dependencies, fieldOrdering),
  );

let encodeCanonicalLabeledType =
    (
      id: definitionID,
      maybeValueType: option(valueType),
      dependencies: publishingDependencies,
    )
    : Js.Json.t =>
  encodeLabeledType(
    id,
    switch (maybeValueType) {
    | None => None
    | Some(valueType) => Some(canonicalizeType(valueType, dependencies))
    },
  );

let canonicalizeInterface =
    (
      interface: interface,
      dependencies: publishingDependencies,
      display: display,
    )
    : publishingInterface => {
  inputs:
    canonicalizeTypedFields(
      interface.inputTypes,
      dependencies,
      display.inputOrdering,
    ),
  outputs:
    canonicalizeTypedFields(
      interface.outputTypes,
      dependencies,
      display.outputOrdering,
    ),
};

let encodeInterface = (interface: publishingInterface): Js.Json.t =>
  Json.Encode.(
    object_([
      ("type", string("interface")),
      ("inputs", encodeTypedFields(interface.inputs)),
      ("outputs", encodeTypedFields(interface.outputs)),
    ])
  );

let encodeCanonicalInterface =
    (
      interface: interface,
      dependencies: publishingDependencies,
      display: display,
    )
    : Js.Json.t =>
  encodeInterface(canonicalizeInterface(interface, dependencies, display));

let encodeExternal = (publishingExternal: publishingExternal): Js.Json.t =>
  Json.Encode.(
    object_([
      ("type", string("interface")),
      ("inputs", encodeTypedFields(publishingExternal.interface.inputs)),
      ("outputs", encodeTypedFields(publishingExternal.interface.outputs)),
    ])
  );

let encodeCanonicalExternal =
    (
      externalImplementation: externalImplementation,
      dependencies: publishingDependencies,
      display: display,
    )
    : Js.Json.t =>
  encodeExternal({
    name: externalImplementation.name,
    interface:
      canonicalizeInterface(
        externalImplementation.interface,
        dependencies,
        display,
      ),
  });
