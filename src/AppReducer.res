let f = (webView, urlHash, state: AppState.t, action: AppAction.t): ReactUpdate.update<
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
  | QueueEvaluation({definitionID, inlineFunctionContext, callingContext, connectionNib, inputs}) =>
    switch state.execution {
    | None => ReactUpdate.NoUpdate
    | Some(execution) =>
      let scopeID = RandomIDMake.f()
      let scope = {
        ...ScopeMake.f(definitionID, callingContext, InlineScope(inlineFunctionContext)),
        sourceValues: inputs,
      }
      open StackFrame
      let newFrame = {
        scopeID: scopeID,
        explicitConnectionSide: {
          isSource: false,
          connectionSide: {
            nib: connectionNib,
            node: NodeConnection(inlineFunctionContext.nodeID),
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
  | Stop =>
    switch state.execution {
    | Some(execution) =>
      Belt.Set.forEach(execution.scheduledEvents, eventId => {
        switch eventId {
        | IntervalID(intervalID) => Js.Global.clearInterval(intervalID)
        | TimeoutID(timeoutID) => Js.Global.clearTimeout(timeoutID)
        | AnimationFrameRequestID(requestID) => AnimationFrame.cancel(requestID)
        }
      })
    | None => ()
    }
    ReactUpdate.Update({...state, execution: None})
  | Step => ExecutionReducer.f(state, webView, urlHash)
  | AddScheduledEvent({eventID, values}) =>
    switch state.execution {
    | None => ReactUpdate.NoUpdate
    | Some(execution) => {
        let stack = Belt.List.tailExn(execution.stack)
        let frame = Belt.List.headExn(stack)
        let scope = Belt.Map.String.getExn(execution.scopes, frame.scopeID)
        let source = Belt.Option.getExn(
          FollowConnection.f(
            frame.explicitConnectionSide.connectionSide,
            frame.scopeID,
            execution.scopes,
            state.definitions,
          ),
        )
        let newSourceValues = ValuesMerge.f(source, values, scope.sourceValues)
        ReactUpdate.Update({
          ...state,
          execution: Some({
            ...execution,
            scheduledEvents: Belt.Set.add(execution.scheduledEvents, eventID),
            stack: list{{...frame, action: Returning}, ...Belt.List.tailExn(stack)},
            scopes: Belt.Map.String.set(
              execution.scopes,
              frame.scopeID,
              {
                ...scope,
                sourceValues: newSourceValues,
              },
            ),
          }),
        })
      }
    }
  }
