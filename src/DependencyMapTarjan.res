type state = {
  indexes: Belt.Map.String.t<int>,
  lowlinks: Belt.Map.String.t<int>,
  onStack: Belt.Set.String.t,
  stack: list<DefinitionID.t>,
  index: int,
  components: list<array<DefinitionID.t>>,
}

let setMinLowlink = (state: state, definitionID: DefinitionID.t, comparisonValue: int) => {
  let lowlink = Belt.Map.String.getExn(state.lowlinks, definitionID)
  if comparisonValue < lowlink {
    {
      ...state,
      lowlinks: Belt.Map.String.set(state.lowlinks, definitionID, comparisonValue),
    }
  } else {
    state
  }
}

let rec worker = (
  definitionID: DefinitionID.t,
  dependencyMap: DependencyMap.t,
  state: state,
): state => {
  let dependencies = Belt.Map.String.getExn(dependencyMap, definitionID)
  let state = {
    ...state,
    indexes: Belt.Map.String.set(state.indexes, definitionID, state.index),
    lowlinks: Belt.Map.String.set(state.lowlinks, definitionID, state.index),
    onStack: Belt.Set.String.add(state.onStack, definitionID),
    index: state.index + 1,
    stack: list{definitionID, ...state.stack},
  }

  let state = Belt.Set.String.reduce(dependencies, state, (state, dependencyDefinitionID) => {
    switch Belt.Map.String.get(state.indexes, dependencyDefinitionID) {
    | None =>
      let state = worker(dependencyDefinitionID, dependencyMap, state)
      setMinLowlink(
        state,
        definitionID,
        Belt.Map.String.getExn(state.lowlinks, dependencyDefinitionID),
      )
    | Some(_) =>
      if Belt.Set.String.has(state.onStack, dependencyDefinitionID) {
        setMinLowlink(
          state,
          definitionID,
          Belt.Map.String.getExn(state.indexes, dependencyDefinitionID),
        )
      } else {
        state
      }
    }
  })

  // this is a root node for a component
  if (
    Belt.Map.String.getExn(state.lowlinks, definitionID) ==
      Belt.Map.String.getExn(state.indexes, definitionID)
  ) {
    let (component, stack) = ListTakeUntilFound.f(state.stack, definitionID)
    let component = Belt.List.toArray(component)
    let onStack = Belt.Set.String.removeMany(state.onStack, component)
    {
      ...state,
      components: list{component, ...state.components},
      stack: stack,
      onStack: onStack,
    }
  } else {
    state
  }
}

let f = (dependencyMap: DependencyMap.t): array<array<DefinitionID.t>> => {
  let state = {
    indexes: Belt.Map.String.empty,
    lowlinks: Belt.Map.String.empty,
    onStack: Belt.Set.String.empty,
    stack: list{},
    index: 0,
    components: list{},
  }
  // let indexes = Belt.Map.String.empty
  // let lowlinks = Belt.Map.String.empty
  // let onStack = Belt.Map.String.empty
  // let stack = list{}
  // let index = 0
  let state = Belt.Map.String.reduce(dependencyMap, state, (state, definitionID, _dependencies) => {
    switch Belt.Map.String.get(state.indexes, definitionID) {
    | Some(_) => state
    | None => worker(definitionID, dependencyMap, state)
    }
  })
  Belt.List.toArray(state.components)
}
