let f = (typeSet: TypeSet.t, dependencies: PublishingDependencyMap.t): array<
  PublishingValueType.t,
> =>
  Belt.List.toArray(
    Belt.List.map(ListSortBy.f(Belt.List.fromArray(Belt.Set.toArray(typeSet)), x => x), valueType =>
      ValueTypeToPublishing.f(valueType, dependencies)
    ),
  )
