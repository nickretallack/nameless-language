let f =
    (typeSet: TypeSet.t, dependencies: PublishingDependencyMap.t)
    : list(PublishingValueType.t) =>
  Belt.List.map(
    ListSortBy.f(Belt.List.fromArray(Belt.Set.toArray(typeSet)), x => x),
    valueType =>
    ValueTypeToPublishing.f(valueType, dependencies)
  );
