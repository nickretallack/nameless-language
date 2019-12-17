[%%debugger.chrome];
let f =
    (action: AppAction.t, state: AppState.t)
    : ReactUpdate.update(AppAction.t, AppState.t) =>
  switch (action) {
  | CreateDefinition(definition) =>
    let newDefinitionID = RandomIDMake.f();
    ReactUpdate.UpdateWithSideEffects(
      {
        ...state,
        definitions:
          Belt.Map.String.set(state.definitions, newDefinitionID, definition),
      },
      _ => {
        ReasonReact.Router.push("#" ++ newDefinitionID);
        None;
      },
    );
  | DefinitionAction(definitionAction) =>
    DefinitionReducer.f(definitionAction, state)

  | Step =>
    ReactUpdate.Update({
      ...state,
      execution:
        switch (state.execution) {
        | None => None
        | Some(execution) =>
          let frame = Belt.List.headExn(execution.stack);
          Some(
            switch (frame.action) {
            | Evaluating => ExecutionReducer.f(execution, state.definitions)
            | Returning(value) =>
              if (Belt.List.length(execution.stack) == 1) {
                {...execution, result: Some(value)};
              } else {
                let frames = Belt.List.tailExn(execution.stack);
                let nextFrame = Belt.List.headExn(frames);
                if (frame.scopeID != nextFrame.scopeID) {
                  let scope =
                    Belt.Map.String.getExn(
                      execution.scopes,
                      nextFrame.scopeID,
                    );
                  let definition =
                    Belt.Map.String.getExn(
                      state.definitions,
                      scope.definitionID,
                    );
                  let connectionSide =
                    switch (definition.implementation) {
                    | GraphImplementation(graphImplementation) =>
                      ExplicitConnecttionSideGetSource.f(
                        nextFrame.explicitConnectionSide,
                        graphImplementation.connections,
                      )
                    | _ => raise(Not_found)
                    };
                  {
                    ...execution,
                    stack: [
                      {...nextFrame, action: Returning(value)},
                      ...Belt.List.tailExn(frames),
                    ],
                    scopes:
                      MapStringUpdate.f(
                        execution.scopes, nextFrame.scopeID, (scope: Scope.t) =>
                        {
                          ...scope,
                          sourceValues:
                            Belt.Map.set(
                              scope.sourceValues,
                              connectionSide,
                              value,
                            ),
                        }
                      ),
                  };
                } else {
                  ExecutionReducer.f(
                    {
                      ...execution,
                      stack: Belt.List.tailExn(execution.stack),
                    },
                    state.definitions,
                  );
                };
              }
            },
          );
        },
    })
  };
