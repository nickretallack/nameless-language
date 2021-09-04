let f = (webView, urlHash, action: AppAction.t, state: AppState.t): ReactUpdate.update<
  AppAction.t,
  AppState.t,
> =>
  switch action {
  | SetAutoSave(value) => ReactUpdate.Update({...state, autoSave: value})
  | ResetData =>
    ReactUpdate.UpdateWithSideEffects(
      {...state, autoSave: false, definitions: DefinitionExamples.v},
      _ => {
        {
          open Dom.Storage
          clear(localStorage)
        }

        None
      },
    )
  | SetDefinitions(definitions) =>
    ReactUpdate.Update({
      ...state,
      definitions: definitions,
    })
  | CreateDefinition(definition) =>
    let newDefinitionID = RandomIDMake.f()
    ReactUpdate.UpdateWithSideEffects(
      {
        ...state,
        definitions: Belt.Map.String.set(state.definitions, newDefinitionID, definition),
      },
      _ => {
        RescriptReactRouter.push(`#${newDefinitionID}/documentation`)
        None
      },
    )
  | DefinitionAction(definitionAction) => DefinitionReducer.f(definitionAction, state)
  | QueueEvaluation({definitionID, inlineScope, connectionNib, inputs}) =>
    switch state.execution {
    | None => ReactUpdate.NoUpdate
    | Some(execution) =>
      let scopeID = RandomIDMake.f()
      let scope = {
        ...ScopeMake.f(definitionID, None, InlineScope(inlineScope)),
        sourceValues: inputs,
      }
      open StackFrame
      let newFrame = {
        scopeID: scopeID,
        explicitConnectionSide: {
          isSource: false,
          connectionSide: {
            nib: connectionNib,
            node: NodeConnection(inlineScope.nodeID),
          },
        },
        action: Evaluating,
      }
      ReactUpdate.UpdateWithSideEffects(
        {
          ...state,
          execution: Some({
            ...execution,
            scopes: Belt.Map.String.set(execution.scopes, scopeID, scope),
            stack: Belt.List.concat(execution.stack, list{newFrame}),
          }),
        },
        ({send}) => {
          // If we're running, make sure we keep running.
          if Belt.List.length(execution.stack) == 0 && !execution.debug {
            send(Step)
          }
          None
        },
      )
    }
  | Stop => ReactUpdate.Update({...state, execution: None})
  | Step => ExecutionReducer.f(state, webView, urlHash)
  }
