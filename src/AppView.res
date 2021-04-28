@react.component
let make = () => {
  let (state, dispatch) = ReactUpdate.useReducer(AppGetInitialState.f(), AppReducer.f)

  React.useEffect(() => {
    if state.autoSave {
      open Dom.Storage
      setItem(AppStateName.v, Json.stringify(AppStateToPersistenceJson.f(state)), localStorage)
    }
    None
  })

  let url = RescriptReactRouter.useUrl()
  let definitionID = url.hash
  let {AppState.languageName: languageName, definitions, error, execution} = state
  <>
    <NavView autoSave=state.autoSave emit=dispatch />
    <div id="content">
      {switch definitionID {
      | "" => <DefinitionListView definitions languageName />
      | "+definition" => <DefinitionAddView languageName emit=dispatch />
      | _ =>
        // debug
        switch execution {
        | None => ()
        | Some(execution) =>
          Js.log(
            Belt.List.toArray(
              Belt.List.reverse(
                Belt.List.map(execution.stack, frame =>
                  ExplicitConnectionSideKey.f(frame.explicitConnectionSide)
                ),
              ),
            ),
          )
        }
        switch Belt.Map.String.get(definitions, definitionID) {
        | None => React.string("Not found")
        | Some(definition) =>
          let {Definition.implementation: implementation, display, documentation} = definition
          let emit = (action: DefinitionAction.t) =>
            dispatch(DefinitionAction({definitionID: definitionID, action: action}))
          let stackFrame = switch execution {
          | Some(execution) =>
            let {StackFrame.scopeID: scopeID, explicitConnectionSide, action} = Belt.List.headExn(
              execution.stack,
            )
            let scope = Belt.Map.String.getExn(execution.scopes, scopeID)
            Some({
              open MaterializedStackFrame
              {scope: scope, explicitConnectionSide: explicitConnectionSide, action: action}
            })
          | None => None
          }
          switch implementation {
          | GraphImplementation(implementation) =>
            <GraphView
              key=definitionID
              definitionID
              definitions
              definition
              implementation
              display
              documentation
              emit
              error
              stackFrame
              languageName
            />
          | _ =>
            <SimpleDefinitionView definitionID definition definitions languageName emit error />
          }
        }
      }}
      {switch execution {
      | None => React.null
      | Some(execution) =>
        <div>
          <button onClick={_ => dispatch(Step)}> {React.string("step")} </button>
          {switch execution.result {
          | None => React.null
          | Some(value) =>
            React.string("Result: " ++ ValueDisplay.f(value, definitions, languageName))
          }}
        </div>
      }}
    </div>
  </>
}
