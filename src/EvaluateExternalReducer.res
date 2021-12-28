let f = (
  execution: Execution.t,
  definitions: DefinitionMap.t,
  scope: Scope.t,
  frame: StackFrame.t,
  source: ConnectionSide.t,
  connections: ConnectionMap.t,
  externalImplementation: ExternalImplementation.t,
  outputID: NibID.t,
  state: AppState.t,
  webView,
  urlHash: array<string>,
): ReactUpdate.update<AppAction.t, AppState.t> => {
  // prerequisite
  let evaluatePrerequisite = switch Belt.Map.get(
    connections,
    {node: source.node, nib: NibConnection(";")},
  ) {
  | None => false
  | Some(connectionSide) =>
    switch ValueResolve.resolveSource(scope, connectionSide, execution.scopes, definitions) {
    | None => true
    | Some(_) => false
    }
  }

  if evaluatePrerequisite {
    ExecutionReducerRequireEvaluation.f(state, execution, list{";"}, frame, source, urlHash)
  } else {
    let nonPrerequisiteInputs = Belt.Map.String.keep(externalImplementation.interface.input, (
      key,
      _value,
    ) => key != ";")

    let inputs = Belt.Map.String.mapWithKey(nonPrerequisiteInputs, (nibID, _) => {
      switch Belt.Map.get(connections, {node: source.node, nib: NibConnection(nibID)}) {
      | None => None
      | Some(connectionSide) =>
        ValueResolve.resolveSource(scope, connectionSide, execution.scopes, definitions)
      }
    })
    switch externalImplementation.name {
    | "makeReference" =>
      let value = switch Belt.Map.String.getExn(inputs, "value") {
      | None =>
        Value.LazyValue({
          scopeID: frame.scopeID,
          explicitConnectionSide: {
            isSource: false,
            connectionSide: {node: source.node, nib: NibConnection("value")},
          },
        })
      | Some(value) => value
      }
      let referenceID = RandomIDMake.f()
      let newExecution = {
        ...execution,
        references: Belt.Map.String.set(execution.references, referenceID, value),
      }
      let reference = Value.Reference(referenceID)
      ExecutionReducerReturn.f(reference, newExecution, source, state, urlHash)
    | "setReference" =>
      switch Belt.Map.String.getExn(inputs, "reference") {
      | None =>
        ExecutionReducerRequireEvaluation.f(
          state,
          execution,
          list{"reference"},
          frame,
          source,
          urlHash,
        )
      | Some(value) =>
        switch value {
        | Reference(referenceID) =>
          let value = switch Belt.Map.String.getExn(inputs, "newValue") {
          | None =>
            Value.LazyValue({
              scopeID: frame.scopeID,
              explicitConnectionSide: {
                isSource: false,
                connectionSide: {node: source.node, nib: NibConnection("newValue")},
              },
            })
          | Some(value) => value
          }

          let newExecution = {
            ...execution,
            references: Belt.Map.String.set(execution.references, referenceID, value),
          }
          ExecutionReducerReturn.f(Prerequisite, newExecution, source, state, urlHash)

        | _ => raise(Exception.TypeError("setReference expected a reference"))
        }
      }

    | "getReference" =>
      switch Belt.Map.String.getExn(inputs, "reference") {
      | None =>
        ExecutionReducerRequireEvaluation.f(
          state,
          execution,
          list{"reference"},
          frame,
          source,
          urlHash,
        )
      | Some(value) =>
        switch value {
        | Reference(referenceID) =>
          let value = Belt.Map.String.getExn(execution.references, referenceID)
          ExecutionReducerReturn.f(value, execution, source, state, urlHash)
        | _ => raise(Exception.TypeError("getReference expected a reference"))
        }
      }
    | _ =>
      switch EvaluateExternalFunction.f(
        execution,
        definitions,
        state.languageName,
        externalImplementation.name,
        outputID,
        inputs,
      ) {
      | EvaluationResult(value) =>
        ExecutionReducerReturn.f(value, execution, source, state, urlHash)
      | SideEffect(optionalValue, sideEffect) =>
        let doSideEffects = arg => {
          let _ = ExecutionReducerSideEffects.f(urlHash, arg)
          sideEffect(webView, arg)
        }

        switch optionalValue {
        | Some(value) =>
          ReactUpdate.UpdateWithSideEffects(
            ExecutionReducerReturnState.f(value, execution, source, state),
            doSideEffects,
          )
        | None => ReactUpdate.SideEffects(doSideEffects)
        }
      | EvaluationRequired(nibIDs) =>
        ExecutionReducerRequireEvaluation.f(state, execution, nibIDs, frame, source, urlHash)
      | MultiValue(values) =>
        ExecutionReducerReturnMultiple.f(source, values, execution, state, urlHash)
      }
    }
  }
}
