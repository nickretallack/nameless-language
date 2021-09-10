let f = (recordType: RecordType.t): Belt.Set.String.t =>
  Belt.Map.String.reduce(recordType, Belt.Set.String.empty, (
    dependencies,
    _nibID,
    valueType: ValueType.t,
  ) => Belt.Set.String.union(dependencies, ValueTypeGetDirectDependencies.f(valueType)))
