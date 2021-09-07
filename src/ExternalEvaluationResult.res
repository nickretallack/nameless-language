type t =
  | EvaluationResult(Value.t)
  | EvaluationRequired(list<NibID.t>)
  | SideEffect(
      Value.t,
      (
        React.ref<Js.Nullable.t<Dom.element>>,
        ReactUpdate.self<AppAction.t, AppState.t>,
      ) => option<unit => unit>,
    )
