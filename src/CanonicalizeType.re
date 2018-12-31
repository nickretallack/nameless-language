open Definition;

let primitiveValueTypeToString =
    (primitiveValueType: primitiveValueType)
    : string =>
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
    }
  );

let encodeTypedFields = (fields: Belt.List.t(publishingValueType)) =>
  Json.Encode.list(encodeValueType, fields);

let canonicalizeTypedFields =
    (
      typedFields: typedFields,
      dependencies: publishingDependencies,
      fieldOrdering: Belt.List.t(nibID),
    )
    : Belt.List.t(publishingValueType) =>
  Belt.List.map(fieldOrdering, nibID =>
    switch (Belt.Map.String.getExn(typedFields, nibID)) {
    | DefinedValueType(definitionID) =>
      PublishingDefinedValueType(
        Belt.Map.String.getExn(dependencies, definitionID).contentID,
      )
    | PrimitiveValueType(primitiveValueType) =>
      PublishingPrimitiveValueType(primitiveValueType)
    }
  );

let encodeCanonicalRecord =
    (
      typedFields: typedFields,
      dependencies: publishingDependencies,
      fieldOrdering: Belt.List.t(nibID),
    )
    : Js.Json.t =>
  encodeTypedFields(
    canonicalizeTypedFields(typedFields, dependencies, fieldOrdering),
  );
