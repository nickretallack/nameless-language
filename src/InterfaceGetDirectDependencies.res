let f = (interface: Interface.t): Belt.Set.String.t =>
  Belt.Set.String.union(
    RecordTypeGetDirectDependencies.f(interface.input),
    RecordTypeGetDirectDependencies.f(interface.output),
  )
