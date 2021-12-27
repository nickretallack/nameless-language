let f = (definition: Definition.t): GraphImplementation.t =>
  switch definition.implementation {
  | GraphImplementation(graphImplementation) => graphImplementation
  | _ => raise(Exception.ShouldntHappen("expected a graph"))
  }
