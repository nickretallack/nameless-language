type t = {
  scopes: Belt.Map.String.t<Scope.t>,
  references: Belt.Map.String.t<Value.t>,
  stack: list<StackFrame.t>,
  result: option<Value.t>,
  reactKey: string,
  debug: bool,
}
