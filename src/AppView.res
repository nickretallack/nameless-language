@react.component
let make = () => {
  let webView = React.useRef(Js.Nullable.null)
  let url = RescriptReactRouter.useUrl()
  let urlHash = Js.String.split("/", url.hash)
  let (state, dispatch) = ReactUpdate.useReducer(
    AppGetInitialState.f(),
    AppReducer.f(webView, urlHash),
  )

  React.useEffect(() => {
    if state.autoSave {
      open Dom.Storage
      setItem(AppStateName.v, Json.stringify(AppStateToPersistenceJson.f(state)), localStorage)
    }
    None
  })

  let {AppState.languageName: languageName, definitions, error, execution} = state

  let webViewNode = switch state.execution {
  | Some(execution) =>
    <div
      id="webview"
      key=execution.reactKey
      tabIndex=0
      style={ReactDOM.Style.make(~visibility=urlHash == ["html"] ? "visible" : "hidden", ())}
      ref={ReactDOM.Ref.domRef(webView)}
    />
  | None => <> </>
  }

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
      {webViewNode}
      {switch urlHash {
      | [""] => <DefinitionListView definitions languageName />
      | ["+definition"] => <DefinitionAddView languageName emit=dispatch />
      | ["html"] => <> </>
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
              execution
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
