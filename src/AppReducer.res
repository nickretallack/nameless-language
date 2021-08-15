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
  | CreateDefinition(definition) =>
    let newDefinitionID = RandomIDMake.f()
    ReactUpdate.UpdateWithSideEffects(
      {
        ...state,
        definitions: Belt.Map.String.set(state.definitions, newDefinitionID, definition),
      },
      _ => {
        RescriptReactRouter.push("#" ++ newDefinitionID)
        None
      },
    )
  | DefinitionAction(definitionAction) => DefinitionReducer.f(definitionAction, state)
  | Stop => ReactUpdate.Update({...state, execution: None})
  | Step =>
    switch state.execution {
    | None => ReactUpdate.NoUpdate
    | Some(execution) =>
      let frame = Belt.List.headExn(execution.stack)
      let newExecution = switch frame.action {
      | Evaluating => ExecutionReducer.f(execution, state.definitions, webView)
      | Returning(value) =>
        if Belt.List.length(execution.stack) == 1 {
          {
            ...execution,
            result: Some(value),
          }
        } else {
          let frames = Belt.List.tailExn(execution.stack)
          let nextFrame = Belt.List.headExn(frames)
          if frame.scopeID != nextFrame.scopeID {
            let scope = Belt.Map.String.getExn(execution.scopes, nextFrame.scopeID)
            let definition = Belt.Map.String.getExn(state.definitions, scope.definitionID)
            let connectionSide = switch definition.implementation {
            | GraphImplementation(graphImplementation) =>
              ExplicitConnecttionSideGetSource.f(
                nextFrame.explicitConnectionSide,
                graphImplementation.connections,
              )
            | _ => raise(Exception.ShouldntHappen("Connection in non-graph"))
            }
            {
              ...execution,
              stack: list{{...nextFrame, action: Returning(value)}, ...Belt.List.tailExn(frames)},
              scopes: MapStringUpdate.f(execution.scopes, nextFrame.scopeID, (scope: Scope.t) => {
                ...scope,
                sourceValues: Belt.Map.set(scope.sourceValues, connectionSide, value),
              }),
            }
          } else {
            ExecutionReducer.f(
              {
                ...execution,
                stack: Belt.List.tailExn(execution.stack),
              },
              state.definitions,
              webView,
            )
          }
        }
      }
      ReactUpdate.UpdateWithSideEffects(
        {
          ...state,
          execution: Some(newExecution),
        },
        _ => {
          if newExecution.debug {
            let newFrame = Belt.List.headExn(newExecution.stack)
            let newScope = Belt.Map.String.getExn(newExecution.scopes, newFrame.scopeID)
            if urlHash[0] != newScope.definitionID {
              RescriptReactRouter.push("#" ++ newScope.definitionID)
            }
          }
          None
        },
      )
    }
  }
