let f = (
  typedFields: RecordType.t,
  dependencies: PublishingDependencyMap.t,
  fieldOrdering: list<NibID.t>,
): list<PublishingValueType.t> =>
  Belt.List.map(fieldOrdering, nibID =>
    ValueTypeToPublishing.f(Belt.Map.String.getExn(typedFields, nibID), dependencies)
  )
