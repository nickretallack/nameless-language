open ReactUpdate
let f = (urlHash: array<string>, {send, state}: ReactUpdate.self<AppAction.t, AppState.t>): option<
  unit => unit,
> => {
  switch state.execution {
  | None => None
  | Some(execution) =>
    if execution.debug {
      switch Belt.List.head(execution.stack) {
      | None => ()
      | Some(frame) =>
        let definitionID = ScopeGetGraphDefinitionID.f(execution, frame.scopeID)
        if urlHash != [definitionID, "implementation", frame.scopeID] {
          RescriptReactRouter.push(`#${definitionID}/implementation/${frame.scopeID}`)
        }
      }
    } else {
      let _ = Js.Global.setTimeout(() => {
        send(Step)
      }, 0)
    }
    None
  }
}
