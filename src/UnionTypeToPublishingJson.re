let f =
    (typeSet: TypeSet.t, dependencies: PublishingDependencyMap.t): Js.Json.t =>
  PublishingUnionTypeToJson.f(TypeSetToPublishing.f(typeSet, dependencies));
