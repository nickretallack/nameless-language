type sideEffect = (
  React.ref<Js.Nullable.t<Dom.element>>,
  option<CallingContext.t>,
  ReactUpdate.self<AppAction.t, AppState.t>,
) => option<unit => unit>

type t =
  | EvaluationResult(Value.t)
  | MultiValue(Belt.Map.String.t<Value.t>)
  | EvaluationRequired(list<NibID.t>)
  | SideEffect(option<Value.t>, sideEffect)
