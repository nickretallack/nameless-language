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
  let urlHash = Js.String.split("/", url.hash)
  let {AppState.languageName: languageName, definitions, error, execution} = state
  <>
    <NavView
      url=urlHash
      autoSave=state.autoSave
      execution=state.execution
      definitions
      languageName
      emit=dispatch
    />
    <div id="content">
      {switch urlHash {
      | [""] => <DefinitionListView definitions languageName />
      | ["+definition"] => <DefinitionAddView languageName emit=dispatch />
      | _ =>
        let definitionID = urlHash[0]
        let urlHashRest = Js.Array2.slice(~start=1, ~end_=Js.Array.length(urlHash), urlHash)
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
              urlHashRest
            />
          | _ =>
            <SimpleDefinitionView definitionID definition definitions languageName emit error />
          }
        }
      }}
    </div>
  </>
}
