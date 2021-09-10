let f = (
  typedFields: RecordType.t,
  dependencies: PublishingDependencyMap.t,
  fieldOrdering: array<NibID.t>,
): array<PublishingValueType.t> =>
  Belt.Array.map(fieldOrdering, nibID =>
    ValueTypeToPublishing.f(Belt.Map.String.getExn(typedFields, nibID), dependencies)
  )
