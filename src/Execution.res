type t = {
  scopes: Belt.Map.String.t<Scope.t>,
  stack: list<StackFrame.t>,
  result: option<Value.t>,
  reactKey: string,
  debug: bool,
}
